package main

import (
	"crypto/rand"
	"fmt"
	"math/big"
	"os"
)

func main() {
	var modulus, temp1, temp2 = new(big.Int), new(big.Int), new(big.Int)
	checkArgs()
	p, q := randomPQgeneration()
	p = getClosestPrime(p)
	q = getClosestPrime(q)
	temp1.Mod(p, big.NewInt(4))
	temp2.Mod(q, big.NewInt(4))
	for temp1.Cmp(big.NewInt(3)) != 0 || temp2.Cmp(big.NewInt(3)) != 0 {
		p, q = randomPQgeneration()
		p = getClosestPrime(p)
		q = getClosestPrime(q)
		temp1.Mod(p, big.NewInt(4))
		temp2.Mod(q, big.NewInt(4))
	}
	modulus.Mul(p, q)
	f, err := os.Create(os.Args[1])
	checkForError(err)
	fmt.Fprint(f, "(", modulus, ")")
	f.Close()
	f, err = os.Create(os.Args[2])
	checkForError(err)
	fmt.Fprint(f, "(", modulus, ", ", p, ", ", q, ")")
	f.Close()
}

//extendedEuclideanAlgorithm
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

func getED(phi *big.Int) (*big.Int, *big.Int) {
	for {
		var temp = big.NewInt(1)
		e, err := rand.Int(rand.Reader, temp.Sub(phi, big.NewInt(1)))
		checkForError(err)
		e.Add(e, big.NewInt(2))
		x, _, g := extendedEuclideanAlgorithm(e, phi)
		if g.Cmp(big.NewInt(1)) == 0 {
			if x.Cmp(phi) < 0 && x.Cmp(big.NewInt(1)) > 0 {
				return e, x
			}
		}
	}
}

func randomPQgeneration() (*big.Int, *big.Int) {
	var i, e = big.NewInt(2), big.NewInt(512)
	i.Exp(i, e, nil)
	a, err := rand.Int(rand.Reader, i)
	checkForError(err)
	b, err := rand.Int(rand.Reader, i)
	checkForError(err)
	return a, b
}

func getClosestPrime(num *big.Int) *big.Int {
	var i, e = big.NewInt(2), big.NewInt(0)
	e = e.Mod(num, i)
	if e.Cmp(big.NewInt(0)) == 0 {
		num.Add(num, big.NewInt(1))
	}
	isPrime := millerRabinPrimalityTest(num)
	for !isPrime {
		num.Add(num, big.NewInt(2))
		isPrime = millerRabinPrimalityTest(num)

	}
	return num
}

func millerRabinPrimalityTest(num *big.Int) bool {
	if num.Cmp(big.NewInt(3)) == 0 || num.Cmp(big.NewInt(2)) == 0 {
		return true
	} else if num.Cmp(big.NewInt(3)) == -1 {
		return false
	}

	var zero, one, two, nMinus3, nMinus1 = big.NewInt(0), big.NewInt(1), big.NewInt(2), new(big.Int), new(big.Int)
	var s, d, mod = big.NewInt(0), big.NewInt(0), new(big.Int)
	var accuracy int
	accuracy = 100
	d.Sub(num, one)
	mod.Mod(d, two)
	for mod.Cmp(zero) == 0 {
		d.Div(d, two)
		s.Add(s, one)
		mod.Mod(d, two)
	}

	nMinus3.Sub(num, big.NewInt(3))
	nMinus1.Sub(num, one)

	for i := 0; i < accuracy; i++ {
		a, err := rand.Int(rand.Reader, nMinus3)
		a.Add(a, two)
		checkForError(err)
		x := powMod(d, a, num)

		if x.Cmp(one) == 0 || x.Cmp(nMinus1) == 0 {
			continue
		}
		r := new(big.Int)
		r.Sub(s, one)
		for j := zero; j.Cmp(r) < 0; j.Add(j, one) {
			x = powMod(two, x, num)
			if x.Cmp(one) == 0 {
				return false
			}
			if x.Cmp(nMinus1) == 0 {
				continue
			}
		}
		return false
	}
	return true
}

//calculates (b^a) % p;
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
