package main

import (
	"fmt"
	"io/ioutil"
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

func checkForKeySize(key string) {
	if len(key) > 32 {
		panic("key is too big, please try with a key smaller than 32 characters")
	}
}

func main() {
	checkArgs()

	var key, fileName string
	key = strings.ToUpper(os.Args[1])
	fileName = os.Args[2]

	fileContent, err := ioutil.ReadFile(fileName)
	checkForFile(err)
	checkForKeySize(key)
	fmt.Println(fileContent)
	fmt.Println(key)

	regExp, err := regexp.Compile("[^a-zA-Z]+")
	checkRegErr(err)
	processedString := strings.ToUpper(regExp.ReplaceAllString(string(fileContent), ""))
	fmt.Println(processedString)

	m := make(map[int]string)

	for i := range []byte(processedString) {

	}

}
