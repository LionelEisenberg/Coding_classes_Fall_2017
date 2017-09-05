package main

import (
    "os"
    "fmt"
)

func main() {
    for _, v := range os.Args {
        fmt.Println(v);
    }
}
