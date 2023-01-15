package main

// NOTE: There should be NO space between the comments and the `import "C"` line.

/*
#cgo LDFLAGS: libparsort.a -ldl -lm
#include "./lib/parsort.h"
*/
import "C"
import (
	"fmt"
	"unsafe"
)

func gen_next(gen1 *uint64, gen2 *uint64, gen3 *uint64) {
	*gen1 ^= *gen1 << 31
	*gen1 ^= *gen1 >> 17
	*gen2 ^= *gen2 << 13
	*gen2 ^= *gen2 >> 5
	*gen3++
	*gen1 ^= *gen2
	*gen2 ^= *gen3
}

func main() {
	var gen1, gen2, gen3, n uint64
	_, err := fmt.Scanf("%d %d %d %d", &gen1, &gen2, &gen3, &n)
	if err != nil {
		panic(err)
	}

	a := make([]uint64, n)

	for i := uint64(0); i < n; i++ {
		gen_next(&gen1, &gen2, &gen3)
		a[i] = gen1
	}

	C.parsort_and_output((C.ulong)(n), (*C.ulong)(unsafe.Pointer(&a[0])))
}
