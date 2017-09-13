package main

import (
    "os"
    "io/ioutil"
    "strconv"
    "fmt"
    "regexp"
    "strings"
)

 //This is an array for the frequencies of letters in the english language
 //the array maps letters (in byte form) to frequencies
var englishLetterFrequency = map[byte]float64 {
    65: 0.082, 66: 0.014, 67: 0.028, 68: 0.038, 69: 0.131, 70: 0.029, 71: 0.020, 72: 0.053, 73: 0.064, 74: 0.001, 75: 0.004, 76: 0.034, 77: 0.025, 78: 0.071,
    79: 0.080, 80: 0.020, 81: 0.001, 82: 0.068, 83: 0.061, 84: 0.105, 85: 0.025, 86: 0.009, 87: 0.015, 88: 0.002, 89: 0.020, 90: 0.001,
}

//function checks that there are the right number of arguments
func checkArgs() {
	if len(os.Args) != 3 {
		panic("THe number of arguments is incorrect")
	}
}

//checks that the file that was asked exists
func checkForFile(err error) {
    if err != nil {
        panic(err);
    }
}

//checks that the key is less than 32 chars, adn that the keylength given is an integer
func checkForKeySize(KL int, err error) {
    if KL > 32 {
        panic("the keylength is too big, please reenter key");
    }
    if err != nil {
        panic("key is not and Integer, please try again")
    }
}

//function gets the chiSquared value for a coset
func getChiSquared(coset []byte) float64 {
    m := make(map[byte]float64); //map maps frequencies of letters in the coset
    var chiSquare float64;
    for _,v := range coset {
        m[v]++;
    }
    for i := 65; i < 91; i++ {
        m[byte(i)] = m[byte(i)] / float64(len(coset)) //turn raw amounts to frequencies
    }
    for i := 65; i < 91; i++ { //calculates the chiSquare for the coset
        chiSquare += (m[byte(i)] - englishLetterFrequency[byte(i)]) * (m[byte(i)] - englishLetterFrequency[byte(i)]) / englishLetterFrequency[byte(i)]
    }
    return chiSquare
}

//Function gets the index associated to the minimum value of an array.
func getMin(arr []float64) int {
    min := 1000.0
    minIndex := -1;
    for i,v := range arr {
        if v < min {
            min = v;
            minIndex = i;
        }
    }
    return minIndex;
}

//creates a guess for the key from the minimum chiSquared value of each coset
func getKey(arr []int) string {
    var key []byte;
    for _,v := range arr {
        //ad 65 to get back to ASCII code
        key = append(key, byte(v + 65))
    }
    return string(key)
}

func main() {
    checkArgs();

    //Get filename and keylength from arguments
    cipherFileName := os.Args[1];
    keyLength, err1 := strconv.Atoi(os.Args[2]);
    cipherText, err2 := ioutil.ReadFile(cipherFileName)
	checkForKeySize(keyLength, err1)
    checkForFile(err2)

    //gets rid of any non alphabetic characters and whitespace
    regExp,err := regexp.Compile("[^a-zA-Z]")
    if err != nil {
        panic(err)
    }
    cipherText = []byte(strings.ToUpper(regExp.ReplaceAllString(string(cipherText), "")))
    cipherLength := len(cipherText)
    var chiSquaredMinArray []int; //array of all chiSquared minimum values

    // We want to create cosets, smaller groups of text that are the keys seperated by keyLength
    for j := 0; j < keyLength; j++ {
        var coset []byte;
        var chiSquaredArray []float64;
        for k := j; k < cipherLength; k += keyLength {
            coset = append(coset, cipherText[k] - 65) //make the coset
        }
        //Do 25 shifts and calculate chiSquared for each
        for i := 0; i < 26; i++ {
            var shiftedCoset []byte;
            //shift coset
            for _,v := range coset {
                var shiftedLetter byte;
                if v-byte(i) > 26 {//deal with overflow
        			shiftedLetter = v-byte(i) + 26
        		} else {
        			shiftedLetter = v-byte(i)
        		}
                shiftedCoset = append(shiftedCoset, shiftedLetter + 65)
            }
            chiSquared := getChiSquared(shiftedCoset);
            chiSquaredArray = append(chiSquaredArray, chiSquared);
        }
        minForCoset := getMin(chiSquaredArray);
        chiSquaredMinArray = append(chiSquaredMinArray, minForCoset)
    }
    fmt.Println(getKey(chiSquaredMinArray))
}
