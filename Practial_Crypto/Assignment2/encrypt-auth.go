package main

import (
  "fmt"
  "os" //Package os provides a platform-independent interface to operating system functionality.
  "encoding/hex"
  //"reflect"
  // "io"
  // "io/ioutil" //For File Reading
)

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

func main() {
  checkArgs();
  checkKey();
  keyAndMAC, err := hex.DecodeString(os.Args[3]);
  checkForError(err);
  key := keyAndMAC[0:16];
  MAC := keyAndMAC[16:32];

  fmt.Println(string(keyAndMAC));
  fmt.Println(key, "\n", MAC);
}
