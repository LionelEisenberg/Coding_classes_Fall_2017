package main

import (
    "os"
    "io/ioutil"
    "fmt"
    "strings"
    "regexp"
)

func checkArgs() { //checks that the correct number of arguments are given
	if len(os.Args) != 2 {
		panic("THe number of arguments is incorrect")
	}
}

func checkForFile(err error) {
    if err != nil {
        panic(err);
    }
}

func getMax(slice []float64) int {
    const max = 0.064
    var maxIndex int;
    for i, v := range slice {
        if v > max {
            maxIndex = i;
            break
        }
    }
    return maxIndex;
}

func getIOC(keyLength int, cipher string) float64{
    cipherLength := len([]rune(cipher))
    var IOCSum, cosetCount float64;
    IOCSum = 0;
    for i := 0; i < keyLength; i++ {
        var coset []byte;
        for j := i; j < cipherLength; j += keyLength {
            coset = append(coset, cipher[j])
        }
        IOCSum += IOCForCoset(string(coset), float64(cipherLength))
        cosetCount++;
    }
    return IOCSum/cosetCount;
}

func IOCForCoset(coset string, cipherLength float64) float64 {
    m := getFrequencies(coset)
    var frequencySum float64;
    for i := range m {
        frequencySum += m[i] * (m[i] - 1);
    }
    IOC := 1/(float64(len([]rune(coset))) * (float64(len([]rune(coset))) - 1)) * frequencySum
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

    regExp, err := regexp.Compile("[^a-zA-Z]")
	cipherTextContent = []byte(strings.ToUpper(regExp.ReplaceAllString(string(cipherTextContent), "")))

    var maxKeyLength = 21
    if len(cipherTextContent) < 20 {
        maxKeyLength = len(cipherTextContent)
    }

    var arrayOfIOC []float64;
    maxKeyLength++;
    for keyLength := 1; keyLength < maxKeyLength; keyLength++ {
        arrayOfIOC = append(arrayOfIOC, getIOC(keyLength, string(cipherTextContent)))
    }
    bestGuess := getMax(arrayOfIOC) + 1

    fmt.Println("The key is of length: ", bestGuess)
}
