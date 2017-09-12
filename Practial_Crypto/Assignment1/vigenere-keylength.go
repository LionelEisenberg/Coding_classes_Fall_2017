package main

import (
    "os"
    "io/ioutil"
    "fmt"
    "strings"
    "regexp"
)

func checkArgs() { //checks that the correct number of arguments are given
	if len(os.Args) != 2 { //if the number of arguments is not 2, then stop the program.
		panic("The number of arguments is incorrect")
	}
}

//checks that the file that was asked exists
func checkForFile(err error) {
    if err != nil {
        panic(err);
    }
}

//This function gets the first number that is above 0.064 (95% of English Index of Coincidence)
//it then returns the index of that number in order to know the number of characters in the key
func getAbove(slice []float64) int {
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

//gets the average IOC of all cosets associated to a certain cipher and keylength
func getIOC(keyLength int, cipher string) float64{
    cipherLength := len([]rune(cipher))
    var IOCSum, cosetCount float64;
    IOCSum = 0;
    //this double for loop creates all the cosets of the cipher in order to get their IOCs
    for i := 0; i < keyLength; i++ {
        var coset []byte;
        //make cosets from characters of the cipher ceperated by the keylength to get the same shift for the whole coset
        for j := i; j < cipherLength; j += keyLength {
            coset = append(coset, cipher[j])
        }
        //get the IOCs for all cosets and sum them up to get an average
        IOCSum += IOCForCoset(string(coset), float64(cipherLength))
        cosetCount++;
    }
    //returns an average
    return IOCSum/cosetCount;
}

//gets the IOC of a certain coset using the formula taken from the website:
//http://pages.mtu.edu/~shene/NSF-4/Tutorial/VIG/Vig-IOC.html
func IOCForCoset(coset string, cipherLength float64) float64 {
    m := getFrequencies(coset)
    var frequencySum float64;
    for i := range m {
        frequencySum += m[i] * (m[i] - 1);
    }
    //formula for IOC calculation
    IOC := 1/(float64(len([]rune(coset))) * (float64(len([]rune(coset))) - 1)) * frequencySum
    return IOC;
}

//this functions gets the frequencies (raw not in percentage) of each letter in a certain coset
//it returns a map that maps characters to frequencies.
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

    //clean the cipher in case there are non-alphabetic characters present.
    regExp, err := regexp.Compile("[^a-zA-Z]")
	cipherTextContent = []byte(strings.ToUpper(regExp.ReplaceAllString(string(cipherTextContent), "")))

    //The maximum keylength we should be able to handle is 20, but if the cipher is smaller than that,
    //we need to reduce the maxKeyLength
    var maxKeyLength = 21
    if len(cipherTextContent) < 20 {
        maxKeyLength = len(cipherTextContent)
    }

    var arrayOfIOC []float64;
    //get average IOC for keylengths from 1 to 20
    for keyLength := 1; keyLength < maxKeyLength; keyLength++ {
        arrayOfIOC = append(arrayOfIOC, getIOC(keyLength, string(cipherTextContent)))
    }
    bestGuess := getAbove(arrayOfIOC) + 1 

    fmt.Println("The key is of length: ", bestGuess)
}
