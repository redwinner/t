package main

import (
    "io"
    "net/http"
	"fmt"
)

func hello(w http.ResponseWriter, r *http.Request) {
    io.WriteString(w, "Hello world!")
}

func main() {
	fmt.Println("Hello world.")
    http.HandleFunc("/", hello)
    http.ListenAndServe(":8000", nil)
}