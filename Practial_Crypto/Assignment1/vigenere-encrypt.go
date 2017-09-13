package main

import (
	"fmt"
	"io/ioutil"
	"math"
	"os"
	"regexp"
	"strings"
)

//function checks that there are the right number of arguments
func checkArgs() {
	if len(os.Args) != 3 {
		panic("The number of arguments is incorrect")
	}
}

//checks that the file that was asked exists
func checkForFile(err error) {
	if err != nil {
		panic("the name of the file is incorrect")
	}
}

func checkRegErr(err error) {
	if err != nil {
		panic(err)
	}
}

//makes sure the key size is correct
func checkForKeySize(keyLength float64) {
	if keyLength > 32 {
		panic("key is too big, please try with a key smaller than 32 characters")
	}
}

func main() {
	checkArgs()

	var key, fileName string
	var keyLength float64
	key = strings.ToUpper(os.Args[1]) //get key
	fileName = os.Args[2] //get filename
	keyLength = float64(len(key))

	inputFileContent, err := ioutil.ReadFile(fileName) //read file into variable
	checkForFile(err)
	checkForKeySize(keyLength)

	//get rid of all non-alphabetic charactes in the plainText file and capitalize it all
	regExp, err := regexp.Compile("[^a-zA-Z]")
	checkRegErr(err)
	processedContent := strings.ToUpper(regExp.ReplaceAllString(string(inputFileContent), ""))
	byteInputContent := []byte(processedContent)

	m := make(map[int]byte)

	for i := range byteInputContent {
		keyPlace := int(math.Mod(float64(i), keyLength))
		m[i] = key[keyPlace]      // map position of plaintext to position of key
		byteInputContent[i] -= 65 // reduce to 0 - 25 code
		m[i] -= 65                // reduce to 0 - 25 code
	}

	cipherText := make([]byte, len(byteInputContent))

	for i := range byteInputContent {
		cipherText[i] = byte(math.Mod(float64(byteInputContent[i]+m[i]), 26)) //for every letter shift it by m[i] and mod it by 26 so that if it overflows (>26) it wraps around
		cipherText[i] += 65 //add 65 back to bring to ASCII Code
	}

	//print out cipherText
	fmt.Println(string(cipherText))
}
