package main

import (
    "os"
    "io/ioutil"
    "fmt"
)

func checkArgs() {
	if len(os.Args) != 2 {
		panic("THe number of arguments is incorrect")
	}
}

func checkForFile(err error) {
    if err != nil {
        panic(err);
    }
}

func getIOC(keyLength int, cipher string) float64{
    fmt.Println(keyLength)
    cipherLength := len([]rune(cipher))
    var IOCSum float64;
    for i := 0; i < keyLength; i++ {
        var coset []byte;
        for j := i; j < cipherLength; j += keyLength {
            coset = append(coset, cipher[j])
        }
        fmt.Println(string(coset));
        IOCSum += IOCForCoset(string(coset), float64(cipherLength))
    }
    return IOCSum;
}

func IOCForCoset(coset string, cipherLength float64) float64 {
    m := getFrequencies(coset)
    var frequencySum float64;
    for i := range m {
        frequencySum += m[i] * (m[i] - 1);
    }

    IOC := 1/(cipherLength * (cipherLength - 1)) * frequencySum
    return IOC;
}

func getFrequencies(coset string) map[rune]float64 {
    m := make(map[rune]float64)
    for _,v := range coset {
        m[v]++;
    }
    return m
}

func main() {
    checkArgs()
    cipherFileName := os.Args[1]

    cipherTextContent, err := ioutil.ReadFile(cipherFileName)
	checkForFile(err)
    var maxKeyLength = 21
    if len(cipherTextContent) < 20 {
        maxKeyLength = len(cipherTextContent)
    }

    var arrayOfIOC []float64;
    maxKeyLength++;
    for keyLength := 1; keyLength < 5; keyLength++ {
        arrayOfIOC = append(arrayOfIOC, getIOC(keyLength, string(cipherTextContent)))
    }
    fmt.Println(arrayOfIOC)
    fmt.Println(cipherTextContent)
}
