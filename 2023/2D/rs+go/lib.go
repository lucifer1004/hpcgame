package main

import "C"
import "unsafe"

//export genfunc1
func genfunc1(gen1 uint64, gen2 uint64, gen3 uint64, n uint64, ptr *C.ulong) {
	a := (*[1 << 31 - 1] uint64)(unsafe.Pointer(ptr))[:n:n]

	for i := uint64(0); i < n; i++ {
		gen1 ^= gen1 << 31
		gen1 ^= gen1 >> 17
		gen2 ^= gen2 << 13
		gen2 ^= gen2 >> 5
		gen3++
		gen1 ^= gen2
		gen2 ^= gen3
		a[i] = gen1
	}
}

//export genfunc2
func genfunc2(gen1 uint64, gen2 uint64, gen3 uint64, n uint64) *C.ulong {
	p := C.malloc(C.size_t(n) * C.size_t(unsafe.Sizeof(C.ulong(0))))
	a := (*[1<<31 - 1] uint64)(p)[:n:n]

	for i := uint64(0); i < n; i++ {
		gen1 ^= gen1 << 31
		gen1 ^= gen1 >> 17
		gen2 ^= gen2 << 13
		gen2 ^= gen2 >> 5
		gen3++
		gen1 ^= gen2
		gen2 ^= gen3
		a[i] = gen1
	}

	return (*C.ulong)(p)
}

func main() {}
