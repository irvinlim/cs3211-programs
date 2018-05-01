# Odd-Even Transposition Sort

This is an implementation of [odd-even transposition sort](https://en.wikipedia.org/wiki/Odd%E2%80%93even_sort), or specifically the original algorithm by Habermann (1972).

This sorting algorithm uses n processors to sort a list of n elements, which gives a best-case runtime of O(n).

## Usage

```sh
make clean && make
mpirun -np 8 --oversubscribe odd-even
```
