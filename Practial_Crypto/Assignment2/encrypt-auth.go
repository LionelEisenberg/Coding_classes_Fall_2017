package main

import (
  "fmt"
  "os" //Package os provides a platform-independent interface to operating system functionality.
  "encoding/hex"
  "crypto/rand"
  "crypto/aes"
  "io/ioutil" //For File Reading
  "crypto/sha256"
)

func main() {
  var output []byte
  checkArgs();
  keyArg, inputArg, outputArg := getArgNums();
  _ = outputArg;
  checkKey();

  keyAndMAC, err := hex.DecodeString(os.Args[keyArg]);
  checkForError(err);
  key := keyAndMAC[0:16];
  MAC := keyAndMAC[16:32];
  msg, err := ioutil.ReadFile(os.Args[inputArg]);
  checkForError(err);
  if os.Args[1] == "encrypt" {
    output = encrypt(key, MAC, msg)
    _ = output;
  } else if os.Args[1] == "decrypt"{
    output = decrypt(key, MAC, msg)
    _ = output;
  }
  err = ioutil.WriteFile(os.Args[outputArg], output, 0644);
  checkForError(err);
}

func decrypt(key []byte, MAC []byte, msg []byte) ([]byte) {
  IV := msg[0:16];
  msg = msg[16:];
  msg = decAesCbc(key, IV, msg);
  fmt.Println("after decrypt: ", msg);
  padByte := msg[len(msg) - 1];
  for i := len(msg) - 1; i >= len(msg) - int(padByte) ;i-- {
    if msg[i] != padByte {
      panic("INVALID PADDING");
    }
  }
  msg = msg[0:(len(msg) - int(padByte))];
  tag := msg[len(msg) - 32:];
  tagPrime := hmac(MAC, msg);
  for i := range tag {
    if tag[i] != tagPrime[i] {
      panic("INVALID TAG")
    }
  }
  fmt.Println(string(msg[0:len(msg) - 32]));

  return msg;
}

func decAesCbc(key []byte, IV []byte, msg []byte) []byte {
  block,err := aes.NewCipher(key)
  checkForError(err);
  var plain []byte;
  for i := 0; i < len(msg); i += 16 {
    encryptedBlock := msg[i:i+16];
    plainBlock := make([]byte, 16);
    block.Decrypt(plainBlock, encryptedBlock);
    if i == 0 {
      for i := range plainBlock {
        plainBlock[i] = plainBlock[i] ^ IV[i];
      }
    } else {
      prevEncBlock := msg[i-16:i];
      for i := range plainBlock {
        plainBlock[i] = plainBlock[i] ^ prevEncBlock[i];
      }
    }
    fmt.Println(plainBlock)
    plain = append(plain, plainBlock...);
  }
  return plain;
}

func encrypt(key []byte, MAC []byte, msg []byte) ([]byte) {
  tag := hmac(MAC, msg);
  msg = append(msg, tag...);
  n := len(msg) % 16;
  if n == 0 {
    for i := 0; i < 16; i++ {
      msg = append(msg, 0x10);
    }
  } else {
    for i := 0; i < 16 - n; i++ {
      msg = append(msg, byte(16-n))
    }
  }

  fmt.Println("b4 encrypt: ", msg);
  IV := make([]byte, 16);
  rand.Read(IV);
  // IV = []byte {0x4f, 0xe0, 0xd2, 0x42, 0x31, 0xb6, 0x30, 0x9c, 0x90, 0xa7, 0x8e, 0xeb, 0x8d, 0xc6, 0xff, 0x2c};
  output := encAesCbc(key, IV, msg);
  return output;
}

func encAesCbc(key []byte, IV []byte, msg []byte) []byte{
  block,err := aes.NewCipher(key)
  if err != nil {
    panic(err);
  }
  previousEncrytedBlock := make([]byte, 16);
  var cipher []byte;
  for i := 0; i < len(msg); i += 16 {
    plainBlock := msg[i:i+16];
    fmt.Println(plainBlock);
    if i == 0 {
      for ind := range plainBlock {
        plainBlock[ind] = plainBlock[ind] ^ IV[ind];
      }
    } else {
      for ind := range plainBlock {
        plainBlock[ind] = plainBlock[ind] ^ previousEncrytedBlock[ind];
      }
    }
    block.Encrypt(previousEncrytedBlock, plainBlock);
    cipher = append(cipher, previousEncrytedBlock...)
  }
  return cipher;
}

func hmac(MAC []byte, msg []byte) ([]byte) {
  const blocksize = 64;
  for len(MAC) < blocksize {
    MAC = append(MAC, 0x00);
  }

  ipad := make([]byte, blocksize);
  opad := make([]byte, blocksize);
  for i := 0; i < blocksize; i++ {
    ipad[i] = 0x36 ^ MAC[i];
    opad[i] = 0x5c ^ MAC[i];
  }

  hasher1 := sha256.New();
  hasher2 := sha256.New();
  ipad = append(ipad, msg...);
  hasher1.Write(ipad);
  hashedipad := hasher1.Sum(nil);
  msgToHash := append(opad, hashedipad...)
  hasher2.Write(msgToHash);
  final := hasher2.Sum(nil);
  return final
}

func checkArgs() {
  if len(os.Args) != 8 {
    panic("Number of arguments is incorrect")
  } else if os.Args[1] != "encrypt" && os.Args[1] != "decrypt" {
    panic("Unrecognized mode");
  } else if os.Args[2] != "-k" {
    panic("unrecognized key flag");
  } else if os.Args[4] != "-i" {
    panic("unrecognized input flag");
  } else if os.Args[6] != "-o" {
    panic("unrecgnized output flag");
  }
}

func getArgNums() (int, int, int){
  var keyArg, inputArg, outputArg int;
  for i := 0; i < len(os.Args); i++ {
    if os.Args[i] == "-k" {
      keyArg = i + 1;
      if keyArg == len(os.Args) {
        panic("Argument input error")
      }
    }
    if os.Args[i] == "-i" {
      inputArg = i + 1;
      if inputArg == len(os.Args) {
        panic("Argument input error")
      }
    }
    if os.Args[i] == "-o" {
      outputArg = i + 1;
      if outputArg == len(os.Args) {
        panic("Argument input error")
      }
    }
  }
  return keyArg, inputArg, outputArg;
}

func checkKey() {
  key := os.Args[3];
  if len(key) != 64 {
    panic("Length of inputed key is less than 32.")
  }
}

func checkForError(err error) {
  if err != nil {
    panic(err)
  }
}
