package main

import (
	"fmt"
	"io/ioutil"
	"math/big"
	"os"
	"os/exec"
)

func main() {
	TWO := big.NewInt(2)
	checkArgs()
	publicKey, err := ioutil.ReadFile(os.Args[1])
	var modulus []byte
	m, mod, sub := new(big.Int), new(big.Int), new(big.Int)
	mPrime, negM := big.NewInt(0), big.NewInt(2)
	m.SetString(os.Args[2], 10)
	checkForError(err)
	for _, v := range publicKey {
		if v == byte('(') || v == byte(' ') || v == byte(')') {
			continue
		}
		modulus = append(modulus, v)
	}
	n := getNum(modulus)
	cipher := powMod(TWO, m, n)
	m.SetString(os.Args[2], 10)
	for mPrime.Cmp(mod) == 0 || mPrime.Cmp(negM) == 0 {
		mod.Mod(m, n)
		out := getCrack(cipher)
		mPrime.SetString(out, 10)
		negM.Neg(m)
		negM.Mod(negM, n)
	}
	sub.Sub(m, mPrime)
	_, _, gcd := extendedEuclideanAlgorithm(sub, n)
	fmt.Println(gcd)
	q := new(big.Int)
	q.Div(n, gcd)
	fmt.Println(q)
}

func getCrack(cipher *big.Int) string {
	s := cipher.String()
	out, err := exec.Command("./rabin-crack", os.Args[1], s).Output()
	checkForError(err)
	return string(out)
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
