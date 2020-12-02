Architecture
Person
	Each person one exit
	Search for closest exit
	Each person has their own speed
	If somebody is blocking the path, another person who is back has decelerate

Required data
	Number of people and exits and timeout

Showed data
	Safe and trapped people

Steps
1 Set map
2 Set exit positions randomly
3 Set people, give random speed and initial position 
4 For each person find the path for the closest exit
5 The time starts
6 Every person starts to execute their own path

Data representation

Map: 
	It is represented with a matrix, where 1 is a wall, 0 is a path, 2 is a person, and 3 is an exit

Structs

Person
 Initial position
 Speed
 Current position
 Path
 finished

Coordinates 
 X
 Y
