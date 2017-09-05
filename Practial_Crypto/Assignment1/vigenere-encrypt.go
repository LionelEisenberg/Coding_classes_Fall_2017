package main

import (
	"fmt"
	"io/ioutil"
	"math"
	"os"
	"regexp"
	"strings"
)

func checkArgs() bool {
	if len(os.Args) != 3 {
		panic("THe number of arguments is incorrect")
	}
	return true
}

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

func checkForKeySize(keyLength float64) {
	if keyLength > 32 {
		panic("key is too big, please try with a key smaller than 32 characters")
	}
}

func main() {
	checkArgs()

	var key, fileName string
	var keyLength float64
	key = strings.ToUpper(os.Args[1])
	fileName = os.Args[2]
	keyLength = float64(len(key))

	fileContent, err := ioutil.ReadFile(fileName)
	checkForFile(err)
	checkForKeySize(keyLength)
	fmt.Println(fileContent)
	fmt.Println(key)

	regExp, err := regexp.Compile("[^a-zA-Z]+")
	checkRegErr(err)
	processedString := strings.ToUpper(regExp.ReplaceAllString(string(fileContent), ""))
	byteString := []byte(processedString)
	fmt.Println(processedString)

	m := make(map[int]byte)

	for i := range byteString {
		keyPlace := int(math.Mod(float64(i), keyLength))
		m[i] = key[keyPlace] // map position of plaintext to position of key
		byteString[i] -= 65  // reduce to 0 - 25 code
		m[i] -= 65           // reduce to 0 - 25 code
	}

	for i := range byteString {
		byteString[i] = byte(math.Mod(float64(byteString[i]+m[i]), 26))
		byteString[i] += 65
	}

	fmt.Println(string(byteString))
}
