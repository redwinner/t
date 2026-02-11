// so.go
// go build -buildmode=c-shared -o libhello.so so.go
package main

/*
#include <stdlib.h>
*/
import "C"

import "fmt"

//export Hello
func Hello(name *C.char) {
	goName := C.GoString(name)
	fmt.Println("Hello,", goName)
}

//export Add
func Add(a, b C.int) C.int {
	return a + b
}

func main() {
	// 必须存在，但可以是空
}
