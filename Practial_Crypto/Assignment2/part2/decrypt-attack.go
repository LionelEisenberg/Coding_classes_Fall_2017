package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
	"strings"
)

func main() {
	checkArgs()
	inputArg := getArgNums()
	cipherText, err := ioutil.ReadFile(os.Args[inputArg])
	checkForError(err)
	filename := "output"
	plainText := make([]byte, len(cipherText))
	for i := len(cipherText)/16 - 5; i >= 0; i-- {
		copyCipher := make([]byte, (i+2)*16)
		copy(copyCipher, cipherText[0:(i+2)*16])
		for k := 15; k >= 0; k-- {
			for h := k + 1; h < 16; h++ {
				copyCipher[i*16+h] = plainText[(i+1)*16+h] ^ cipherText[i*16+h] ^ byte(16-k)
			}
			for j := 0x00; j <= 0xff; j++ {
				copyCipher[i*16+k] = byte(j) ^ cipherText[i*16+k] ^ byte(16-k)
				err = ioutil.WriteFile(filename, copyCipher, 0644)
				checkForError(err)
				if !strings.Contains(getDecrypt(filename), "INVALID PADDING") {
					plainText[(i+1)*16+k] = byte(j)
				}
			}
		}
	}
	fmt.Println(string(plainText[16:]))
}

func getDecrypt(filename string) string {
	cmd := exec.Command("./decrypt-test", "-i", filename)
	stdout, err := cmd.Output()
	checkForError(err)
	cmd.Run()
	return string(stdout)
}

func checkArgs() {
	if len(os.Args) != 3 {
		panic("Number of arguments is incorrect")
	}
}

func getArgNums() int {
	var inputArg int
	for i := range os.Args {
		if os.Args[i] == "-i" {
			inputArg = i + 1
		}
	}
	return inputArg
}

func checkForError(err error) {
	if err != nil {
		panic(err)
	}
}

func testEq(a, b []byte) bool {

	if a == nil && b == nil {
		return true
	}

	if a == nil || b == nil {
		return false
	}

	if len(a) != len(b) {
		return false
	}

	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}

	return true
}
