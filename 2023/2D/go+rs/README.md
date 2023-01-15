# Go + Rust

Archived versions:

```go
package main

/*
#cgo LDFLAGS: libparsort.a -ldl -lm
#include "./lib/parsort.h"
*/
import "C"
import (
    "fmt"
    "sync"
    "unsafe"
)

const THREADS = uint64(8)
const UNIT = ^uint64(0) / THREADS + 1

func gen_next(gen1 *uint64, gen2 *uint64, gen3 *uint64) {
    *gen1 ^= *gen1 << 31
    *gen1 ^= *gen1 >> 17
    *gen2 ^= *gen2 << 13
    *gen2 ^= *gen2 >> 5
    *gen3++
    *gen1 ^= *gen2
    *gen2 ^= *gen3
}

func gen_batch(gen1 uint64, gen2 uint64, gen3 uint64, n uint64, id uint64, a *[]uint64, wg *sync.WaitGroup) {
    for i := uint64(0); i < n; i++ {
        gen_next(&gen1, &gen2, &gen3)
        if gen1 / UNIT == id {
            *a = append(*a, gen1)
        }
    }

    k := len(*a)
    if k > 0 { 
        ptr := unsafe.Pointer(&(*a)[0])
        C.parsort((C.ulong)(n), (*C.ulong)(ptr))
        *a = (*[1<<31 - 1] uint64)(ptr)[n-uint64(k):n:n]
    }

    wg.Done()
}

func main() {
    var gen1, gen2, gen3, n uint64
    _, err := fmt.Scanf("%d %d %d %d", &gen1, &gen2, &gen3, &n)
    if err != nil {
        panic(err)
    }

    arr := make([][]uint64, THREADS)
    for i := uint64(0); i < THREADS; i++ {
        arr[i] = make([]uint64, 0, n)
    }

    var wg sync.WaitGroup
    for i := uint64(0); i < THREADS; i++ {
        wg.Add(1)
        go gen_batch(gen1, gen2, gen3, n, i, &arr[i], &wg)
    }

    wg.Wait()

    cnt := uint64(0);
    ans := uint64(0);
    for i := uint64(0); i < THREADS; i++ {
        for _, val := range arr[i] {
            ans ^= cnt * val
            cnt++
        }
    }

    fmt.Println(ans)
}
```
