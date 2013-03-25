Here I plan to put some projects which may not be useful, per se, but implement some mathematical ideas. For now, the only thing I have is an implementation of the Euclidean algorithm for finding the greatest common denominator of two numbers. The code prints each step of the algorithm. After it has found the greatest common denominator, it "unzips" the computation, expressing the greatest common denominator as a linear combination of the two inputs. The curious should check out more information on this algorithm from Wikipedia or Wolfram MathWorld.

The code can be compiled by an ANSI-compatible C compiler (C89 standard). On unix this might mean

`> cc -o euclid euclid.c`

Subsequently it can be run by passing in two positive integer arguments:

```
> ./euclid 97135 45293
97135 = 2 * 45293 + 6549
45293 = 6 * 6549 + 5999
6549 = 1 * 5999 + 550
5999 = 10 * 550 + 499
550 = 1 * 499 + 51
499 = 9 * 51 + 40
51 = 1 * 40 + 11
40 = 3 * 11 + 7
11 = 1 * 7 + 4
7 = 1 * 4 + 3
4 = 1 * 3 + 1
3 = 3 * 1 + 0
gcd(97135,45293) = 1
Expressing 1 as a linear combination of 97135 and 45293:
1 = 1 * 4 - 1 * 3
1 = 2 * 4 - 1 * 7
1 = 2 * 11 - 3 * 7
1 = 11 * 11 - 3 * 40
1 = 11 * 51 - 14 * 40
1 = 137 * 51 - 14 * 499
1 = 137 * 550 - 151 * 499
1 = 1647 * 550 - 151 * 5999
1 = 1647 * 6549 - 1798 * 5999
1 = 12435 * 6549 - 1798 * 45293
1 = 12435 * 97135 - 26668 * 45293
```
