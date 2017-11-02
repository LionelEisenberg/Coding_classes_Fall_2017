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
	c := new(big.Int)
	c.SetString(os.Args[2], 10)
	checkForError(err)
	var modulus, privateKey1, privateKey2 []byte
	var temp, temp1, temp2, x, y, negX, negY = big.NewInt(1), big.NewInt(1), big.NewInt(1), big.NewInt(1), big.NewInt(1), big.NewInt(1), big.NewInt(1)
	set := 0
	for _, v := range publicKey {
		if set == 3 {
			break
		}
		if v == byte('(') || v == byte(' ') || v == byte(')') {
			continue
		}
		if v == byte(',') {
			set++
			continue
		}
		if set == 1 {
			privateKey1 = append(privateKey1, v)
		} else if set == 0 {
			modulus = append(modulus, v)
		} else if set == 2 {
			privateKey2 = append(privateKey2, v)
		}
	}
	n := getNum(modulus)
	p := getNum(privateKey1)
	q := getNum(privateKey2)
	// a and b are such that a * p + b * q = 1
	a, b, _ := extendedEuclideanAlgorithm(p, q)
	temp.Add(p, big.NewInt(1))
	temp.Div(temp, big.NewInt(4))
	r := powMod(temp, c, p)
	c.SetString(os.Args[2], 10)
	temp.Add(q, big.NewInt(1))
	temp.Div(temp, big.NewInt(4))
	s := powMod(temp, c, q)
	//x = (a * p * s + b * q * r) mod n
	//y = (a * p * s - b * q * r) mod n
	temp1.Mul(a, p)
	temp1.Mul(temp1, s)
	temp2.Mul(b, q)
	temp2.Mul(temp2, r)
	x.Add(temp1, temp2)
	y.Sub(temp1, temp2)
	x.Mod(x, n)
	y.Mod(y, n)
	// This gives us 4 messages, x, y, -x and -y
	negX.Neg(x)
	negY.Neg(y)
	negX.Add(negX, n)
	negY.Add(negY, n)
	m := []*big.Int{x, y, negX, negY}
	for _, v := range m {
		if len(v.Bytes()) < 16 {
			if checkPrepend(v, 0) {
				l := v.Bytes()[8:]
				v.SetBytes(l)
				fmt.Println(v)
			}
		} else {
			if checkPrepend(v, 1) {
				l := v.Bytes()[8:]
				v.SetBytes(l)
				fmt.Println(v)
			}
		}
	}
}

//check tells function if checking for 255 prepend or for prepend same as first 8 bytes;
func checkPrepend(v *big.Int, check int) bool {
	prepend := v.Bytes()[:8]
	lastEight := v.Bytes()[len(v.Bytes())-8:]
	if check == 0 {
		for _, val := range prepend {
			if val != 255 {
				return false
			}
		}
		return true
	} else if check == 1 {
		for i := range prepend {
			if prepend[i] != lastEight[i] {
				return false
			}
		}
		return true
	}
	return false
}

func extendedEuclideanAlgorithm(a, b *big.Int) (*big.Int, *big.Int, *big.Int) {
	x, y, u, v := big.NewInt(0), big.NewInt(1), big.NewInt(1), big.NewInt(0)
	for a.Cmp(big.NewInt(0)) != 0 {
		var quotient, remainder, i, j = big.NewInt(1), big.NewInt(1), big.NewInt(1), big.NewInt(1)
		quotient.Div(b, a)
		remainder.Mod(b, a)
		i.Mul(u, quotient)
		i.Sub(x, i)
		j.Mul(v, quotient)
		j.Sub(y, j)
		b = a
		a = remainder
		x = u
		y = v
		u = i
		v = j
	}
	g := b
	return x, y, g
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
		// y must be even now
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
