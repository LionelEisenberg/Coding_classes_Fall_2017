package main

import (
    "os"
    "math"
    "io/ioutil"
    "strconv"
)

var englishLetterFrequency = map[byte]float64 {
    65: 0.082, 66: 0.014, 67: 0.028, 68: 0.038, 69: 0.131, 70: 0.029, 71: 0.020, 72: 0.053, 74: 0.064, 75: 0.001, 76: 0.004, 77: 0.034, 78: 0.025,
    79: 0.071, 80: 0.080, 81: 0.020, 82: 0.001, 83: 0.068, 84: 0.061, 85: 0.105, 86: 0.025, 87: 0.009, 88: 0.015, 89: 0.002, 90: 0.020, 91: 0.001,
}

func checkArgs() {
	if len(os.Args) != 3 {
		panic("THe number of arguments is incorrect")
	}
}

func checkForFile(err error) {
    if err != nil {
        panic(err);
    }
}

func checkForKeySize(KL int, err error) {
    if KL > 32 {
        panic("the keylength is too big, please reenter key");
    }
    if err != nil {
        panic("key is not and Integer, please try again")
    }
}

func getChiSquared(coset []byte) float64 {
    var m map[byte]float64;
    var chiSquare float64;
    for _,v := range coset {
        m[v]++;
        m[v] = m[v] / 26
    }
    for i,v := range m {
        chiSquare += math.Pow((v - englishLetterFrequency[i]),2)/englishLetterFrequency[i]
    }
    return chiSquare
}

func main() {
    checkArgs();

    cipherFileName := os.Args[1];
    keyLength, err1 := strconv.Atoi(os.Args[2]);

    cipherText, err2 := ioutil.ReadFile(cipherFileName)
    cipherLength := len(cipherText)
	checkForKeySize(keyLength, err1)
    checkForFile(err2)

    for j := 0; j < keyLength; j++ {
        var coset []byte;
        for k := j; k < cipherLength; k += keyLength {
            coset = append(coset, cipherText[j] - 65)
        }
        for i := 0; i < 26; i++ {
            var shiftedCoset []byte;
            for _,v := range coset {
                shiftedCoset = append(shiftedCoset, byte(math.Mod(float64(v + byte(i)), 26)) + 65)
            }
            //chiSquared := getChiSquared(shiftedCoset);
        }
    }
}
