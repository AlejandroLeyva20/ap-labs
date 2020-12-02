package main

import (
	"fmt"
	"flag"
	"github.com/faiface/pixel"
	"github.com/faiface/pixel/pixelgl"
	"github.com/faiface/pixel/imdraw"
	"golang.org/x/image/colornames"
	"math/rand"
	"time"
	"github.com/faiface/pixel/text"
	"golang.org/x/image/font/basicfont"
)

type coord struct{
	x	int
	y	int
}

type person struct {
	initialPos	coord		//Posicion inicial
	speed 		float32 	//Velociad
	pos 		coord		//Posicion actual
	path 		[]coord		//Camino
	finished 	bool		//Terminó
}

var ( 
	mapa = [][]int{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
		{1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1}, 
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
		{1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1}, 
		{1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1}, 
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
		{1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1}, 
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
		{1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1}, 
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	} 
	numberOfPeople		int
	numberOfExits		int
	people				[]person
	timeOut				int
	exits				[]coord
	auxPath				[]coord
	visited				[12][12]bool
	parar 				bool = false
	queue				[]coord
	followingPath		[12][12]coord
)
func setWindow() *pixelgl.Window {

	cfg := pixelgl.WindowConfig{
		Title:  "Earthquake Simulator",
		Bounds: pixel.R(0, 0, 840, 840),
		VSync:  true,
		Position: pixel.V(1300, 500),
	}

	win, err := pixelgl.NewWindow(cfg)
	if err != nil {
		panic(err)
	}

	win.Clear(colornames.White)
	return win
}
func draw(win *pixelgl.Window) *imdraw.IMDraw {

	floor := imdraw.New(nil)

	floor.Color = colornames.White
	floor.Push(pixel.V(60, 60))
	floor.Push(pixel.V(780, 780))
	floor.Rectangle(0)

	var x = 60.0
	var y = 60.0

	var x2 = 90.0
	var y2 = 90.0 

	for i := len(mapa) - 1; i >= 0; i-- {
		for _, col := range mapa[i] {
			if col == 1 {
				floor.Color = colornames.Gray
				floor.Push(pixel.V(x, y))
				floor.Push(pixel.V(x+60.0, y+60.0))
				floor.Rectangle(0)
			} else if col == 3 {
				floor.Color = colornames.Lightgreen
				floor.Push(pixel.V(x, y))
				floor.Push(pixel.V(x+60.0, y+60.0))
				floor.Rectangle(0)
			} else if col == 2 {
				floor.Color = colornames.Blue
				floor.Push(pixel.V(x2, y2))
				floor.Circle(20, 0)
			}
			x += 60.0 
			x2 += 60.0
		}
		x = 60.0
		y += 60.0

		x2 = 90.0
		y2 += 60.0
	}

	floor.Draw(win)
	win.Update()	
	return floor
}

func printResults(win *pixelgl.Window) {
	txt := text.NewAtlas(basicfont.Face7x13, text.ASCII)

	others := text.New(pixel.V(60, 30), txt)
	others.Color = colornames.Black
	var c int = 0
	for i := 0; i < len(people); i++ {
		if(people[i].finished){
			c++
		}
	}
	fmt.Fprintf(others, "Exited: %d - Trapped: %d", c, len(people)-c)

	others.Draw(win, pixel.IM.Scaled(others.Orig, 1.8))
	win.Update()

}


func generateSpeed() float32{
	s1 := rand.NewSource(time.Now().UnixNano())
	r1 := rand.New(s1)
	return 1 + r1.Float32()*(2-1)	
}

func isValid(x, y int) bool{
	if x < 0 || x > len(mapa){
		return false
	}
	if y < 0 || y > len(mapa){
		return false
	}
	if mapa[x][y] != 0{
		if mapa[x][y] == 3 {
			return true
		}
		return false
	}
	if visited[x][y]{
		return false
	}
	return true
}

func generatePath(x,y int) coord{
	var finished bool
	visited[x][y] = true
	for i := 0; i < len(exits); i++{
		if exits[i].x == x && exits[i].y == y {
			finished = true
			return coord{exits[i].x, exits[i].y}
		}
	}
	if finished {
		auxPath = append(auxPath, coord{x, y})
	} else{
		if isValid(x+1, y){ //move rigth
			queue = append(queue, coord{x+1, y})
			followingPath[x+1][y] = coord{x, y}
		}
		if isValid(x-1, y){ //move left
			queue = append(queue, coord{x-1, y})
			followingPath[x-1][y] = coord{x, y}
		}
		if isValid(x, y+1){ //move down
			queue = append(queue, coord{x, y+1})
			followingPath[x][y+1] = coord{x, y}
		}
		if isValid(x, y-1){ //move up
			queue = append(queue, coord{x, y-1})
			followingPath[x][y-1] = coord{x, y}
		}
	}
	var newX int = queue[0].x
	var newY int = queue[0].y
	queue = queue[1:]
	return generatePath(newX, newY)
}

func setPeople(){
	people = make([]person, numberOfPeople)
	rand.Seed(time.Now().UnixNano())
	var x, y, i int
	i = 0
	for i < numberOfPeople {
		//set position (random)
		x = rand.Intn(len(mapa))
		y = rand.Intn(len(mapa))
		if mapa[x][y] != 0 {
			continue	
		}
		mapa[x][y] = 2
		var posI coord
		posI.x=x
		posI.y=y
		auxPath = make([]coord, 0)
		var auxVisited [12][12]bool
		visited = auxVisited
		var auxFollowing [12][12]coord
		followingPath = auxFollowing
		var pos coord= generatePath(x, y)
		for {
			auxPath = append(auxPath, pos)
			if mapa[pos.x][pos.y] == 2 {
				break
			} else {
				pos.x = followingPath[pos.x][pos.y].x
				pos.y = followingPath[pos.x][pos.y].y
			}
		}
		queue = make([]coord, 0)
		people[i] = person{posI, generateSpeed(), posI, auxPath , false}
		i++
		
	}
}

func setExits(){
	exits = make([]coord, 0)
	rand.Seed(time.Now().UnixNano())
	var x, y, i int
	i=0
	for i < numberOfExits {
		x = rand.Intn(len(mapa))
		y = rand.Intn(len(mapa))
		if x == 0{
			if mapa[x][y] == 1 && mapa[x+1][y] == 0{
				mapa[x][y]=3
				exits = append(exits, coord{x, y})
				i++
			}
		}else if x == len(mapa)-1 {
			if mapa[x][y] == 1 && mapa[x-1][y] == 0{
				mapa[x][y]=3
				exits = append(exits, coord{x, y})
				i++
			}
		}else if y == 0 {
			if mapa[x][y] == 1 && mapa[x][y+1] == 0{
				mapa[x][y]=3
				exits = append(exits, coord{x, y})
				i++
			}
		}else if y == len(mapa)-1 {
			if mapa[x][y] == 1 && mapa[x][y-1] == 0{
				mapa[x][y]=3
				exits = append(exits, coord{x, y})
				i++
			}
		}
	}
}


func executePath(p person, personIndex int, onExit, onMove chan person, ){

	for i:=len(p.path)-1; i>= 0; i--  {
		time.Sleep(time.Duration(p.speed) * time.Second)
		if(parar){
			return
		}
		mapa[p.pos.x][p.pos.y]=0
		p.pos=p.path[i]
		if(mapa[p.pos.x][p.pos.y]==2){
			p.speed /= 2 //reduce velocity
			time.Sleep(time.Duration(p.speed) * time.Second)
		}
		mapa[p.pos.x][p.pos.y]=2
		onMove <- p
		
	}
	mapa[p.pos.x][p.pos.y]=3
	people[personIndex].finished = true
	onExit <- p	
}

func run(){
	window := setWindow()
	onExit := make(chan person)
	onMove := make(chan person)
	done := time.After(time.Second * time.Duration(timeOut))
	for i := 0; i < len(people); i++ {
		go executePath(people[i],i, onExit, onMove)
	}
	go func() {
		for {
			select{
				case <-done:
					fmt.Println("Time is over")
					printResults(window)
					parar = true
					break
				case p := <-onExit:
					fmt.Println("One more person is out",p)
					break
				case p := <-onMove:
					draw(window)
					fmt.Println("One person is in movement",p)
					break	
			}
		}
		
	}()
	draw(window)
	for !window.Closed() {
		window.Update()
	}
}
func main(){
	var fTime = flag.Int("time", 5, "time in seconds")
	var fPeople = flag.Int("people", 5, "number of people")
	var fExits = flag.Int("exits", 5, "number of exits")
	flag.Parse()
	timeOut = *fTime
	numberOfPeople = *fPeople
	numberOfExits = *fExits

	setExits() 		// Set salidas
	setPeople()		// Se people
	pixelgl.Run(run)
	
}