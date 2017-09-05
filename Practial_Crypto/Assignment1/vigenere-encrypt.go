package main

import (
	"fmt"
	"io/ioutil"
	"math"
	"os"
	"regexp"
	"strings"
)

func checkArgs() {
	if len(os.Args) != 3 {
		panic("THe number of arguments is incorrect")
	}
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

	inputFileContent, err := ioutil.ReadFile(fileName)
	checkForFile(err)
	checkForKeySize(keyLength)

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
		cipherText[i] = byte(math.Mod(float64(byteInputContent[i]+m[i]), 26))
		cipherText[i] += 65
	}

	fmt.Println(string(cipherText))

}
