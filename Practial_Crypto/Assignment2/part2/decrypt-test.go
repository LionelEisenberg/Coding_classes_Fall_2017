package main

import (
	"crypto/aes"
	"crypto/sha256"
	"encoding/hex"
	"fmt"
	"io/ioutil" //For File Reading
	"os"        //Package os provides a platform-independent interface to operating system functionality.
)

func main() {
	var output string
	checkArgs()
	inputArg := getArgNums()
	keyAndMAC, err := hex.DecodeString("43651000510098f746e154f0c9c02bc8f5a9f3bf6b503d20612d7906f6386f8f")
	checkForError(err)

	key := keyAndMAC[0:16]
	MAC := keyAndMAC[16:32]
	cipherText, err := ioutil.ReadFile(os.Args[inputArg])
	checkForError(err)
	output = decrypt(key, MAC, cipherText)
	fmt.Print(output)
}

func decrypt(key []byte, MAC []byte, msg []byte) string {
	IV := msg[0:16]
	msg = msg[16:]
	msg = decAesCbc(key, IV, msg)
	padByte := msg[len(msg)-1]
	for i := len(msg) - 1; i >= len(msg)-int(padByte); i-- {
		if msg[i] != padByte {
			return "INVALID PADDING"
		}
	}
	msg = msg[0:(len(msg) - int(padByte))]
	tag := msg[len(msg)-32:]
	msg = msg[:len(msg)-32]
	tagPrime := hmac(MAC, msg)
	for i := range tag {
		if tag[i] != tagPrime[i] {
			return "INVALID MAC"
		}
	}
	return "SUCCESS"
}

func decAesCbc(key []byte, IV []byte, msg []byte) []byte {
	block, err := aes.NewCipher(key)
	checkForError(err)
	var prevEncBlock []byte
	var plain []byte
	for i := 0; i < len(msg); i += 16 {
		encryptedBlock := msg[i : i+16]
		plainBlock := make([]byte, 16)
		block.Decrypt(plainBlock, encryptedBlock)
		if i == 0 {
			for ind := range plainBlock {
				plainBlock[ind] = plainBlock[ind] ^ IV[ind]
			}
		} else {
			for ind := range plainBlock {
				plainBlock[ind] = plainBlock[ind] ^ prevEncBlock[ind]
			}
		}
		prevEncBlock = encryptedBlock
		plain = append(plain, plainBlock...)
	}
	return plain
}

func hmac(MAC []byte, msg []byte) []byte {
	const blocksize = 64
	for len(MAC) < blocksize {
		MAC = append(MAC, 0x00)
	}

	ipad := make([]byte, blocksize)
	opad := make([]byte, blocksize)
	for i := 0; i < blocksize; i++ {
		ipad[i] = 0x36 ^ MAC[i]
		opad[i] = 0x5c ^ MAC[i]
	}

	hasher1 := sha256.New()
	hasher2 := sha256.New()
	ipad = append(ipad, msg...)
	hasher1.Write(ipad)
	hashedipad := hasher1.Sum(nil)
	msgToHash := append(opad, hashedipad...)
	hasher2.Write(msgToHash)
	final := hasher2.Sum(nil)
	return final
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
