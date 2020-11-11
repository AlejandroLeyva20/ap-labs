package main

import (
	"fmt"
	"time"
)

var chann = make(chan struct{})
var transitTime = time.Now()

func pipeline(f chan struct{}, s chan struct{}, cStages int, mStages int) {
	if cStages <= mStages {
		fmt.Println("Maximum number of pipeline stages   : ", mStages)
		fmt.Println("Time to transit trough the pipeline : ", time.Since(transitTime))
		transitTime = time.Now()
		go pipeline(s, f, cStages+1, mStages)
		s <- <-f
	} else {
		fmt.Println("Maximum number of stages, need to wait")
		chann <- struct{}{}
	}
}
func main() {
	var fPipeline chan (struct{})
	var sPipeline chan (struct{})
	go pipeline(fPipeline, sPipeline, 0, 100)
	<-chann
}
