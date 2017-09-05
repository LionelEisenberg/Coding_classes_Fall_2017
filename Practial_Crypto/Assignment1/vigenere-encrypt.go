package main

import (
    "os"
    //"io"
    //"io/ioutil"
    "fmt"
)

func checkArgs() bool{
    if (len(os.Args) != 3) {
        return false
    }
    return true
}

func main() {
    if (!checkArgs()) {
        panic("Wrong number of command line arguments");
    }

    var key, fileName string
    key = os.Args[1];
    fileName = os.Args[2]

    fmt.Println("the key is ", key, " and the file name is ", fileName);
}
