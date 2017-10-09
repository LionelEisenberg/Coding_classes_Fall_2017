package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
)

func main() {
	checkArgs()
	inputArg := getArgNums()
	cipherText, err := ioutil.ReadFile(os.Args[inputArg])
	checkForError(err)
	for i := len(cipherText)/16 - 3; i >= 0; i++ {
		copyCipher := cipherText[0 : (i+2)*16]
		for j := 0x00; j <= 0xff; j++ {
			cipherText, _ = ioutil.ReadFile(os.Args[inputArg])
			copyCipher[i*16+15] = byte(j) ^ cipherText[i*16+15] ^ 0x01
			filename := "output1.txt"
			err = ioutil.WriteFile(filename, copyCipher, 0644)
			checkForError(err)
			println(j)
			if getDecrypt(filename) != "INVALID PADDING" {
				fmt.Println(byte(j))
			}
		}
		break
	}
}

func getDecrypt(filename string) string {
	cmd := exec.Command("go", "run", "decrypt-test.go", "-i", filename)
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
