package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strings"
)

func printTime(dst io.Writer, src io.Reader, city string) {
	s := bufio.NewScanner(src)
	for s.Scan() {
		fmt.Printf("%s\t: %s\n", city, s.Text())
	}
}

func main() {
	for i := 1; i < len(os.Args); i++ {
		arr := strings.Split(os.Args[i], "=")

		if len(arr) != 2 {
			fmt.Println("Invalid format for arguments. Format: <cityName>=<portNumber>")
			return
		}

		conn, err := net.Dial("tcp", arr[1])
		if err != nil {
			log.Fatal(err)
		}
		defer conn.Close()

		go printTime(os.Stdout, conn, arr[0])
	}

	ch := make(chan int)
	<-ch
}