package main

import (
	"fmt"
	"io/ioutil"
	"math/big"
	"os"
)

func main() {
	checkArgs()
	publicKey, err := ioutil.ReadFile(os.Args[1])
	m := new(big.Int)
	m.SetString(os.Args[2], 10)
	checkForError(err)
	var modulus, exponent []byte
	var set bool
	for _, v := range publicKey {
		if v == byte('(') || v == byte(' ') || v == byte(')') {
			continue
		}
		if v == byte(',') {
			set = true
			continue
		}
		if set {
			exponent = append(exponent, v)
		} else {
			modulus = append(modulus, v)
		}
	}
	n := getNum(modulus)
	e := getNum(exponent)
	fmt.Println(powMod(e, m, n))
}

func getNum(modulus []byte) *big.Int {
	var mod = new(big.Int)
	for i := len(modulus) - 1; i >= 0; i-- {
		var pow = big.NewInt(0)
		ten := big.NewInt(10)
		j := big.NewInt(int64(i))
		pow.Exp(ten, j, nil)
		mod.Add(mod, pow.Mul(pow, big.NewInt(int64(modulus[len(modulus)-1-i]-48))))
	}
	return mod
}

func powMod(a, b, p *big.Int) *big.Int {
	result := big.NewInt(1)
	temp := big.NewInt(1)
	b.Mod(b, p)
	for a.Cmp(big.NewInt(0)) > 0 {
		temp.Mod(a, big.NewInt(2))
		if temp.Cmp(big.NewInt(1)) == 0 {
			t := new(big.Int)
			t.Mul(b, result)
			result.Mod(t, p)
		}
		a.Div(a, big.NewInt(2))
		b.Mul(b, b)
		b.Mod(b, p)
	}
	return result
}

func checkArgs() {
	if len(os.Args) != 3 {
		panic("number of arguments is wrong")
	}
}

func checkForError(e error) {
	if e != nil {
		panic(e)
	}
}
