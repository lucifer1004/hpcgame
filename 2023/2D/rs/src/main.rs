use rdst::RadixSort;
use std::io::{self, BufRead};

fn gen_next(gen1: &mut usize, gen2: &mut usize, gen3: &mut usize) -> usize {
    *gen1 ^= *gen1 << 31;
    *gen1 ^= *gen1 >> 17;
    *gen2 ^= *gen2 << 13;
    *gen2 ^= *gen2 >> 5;
    *gen3 += 1;
    *gen1 ^= *gen2;
    *gen2 ^= *gen3;
    *gen1
}

fn main() {
    let mut line = String::new();
    let stdin = io::stdin();
    stdin.lock().read_line(&mut line).unwrap();
    let params = line
        .trim()
        .split(" ")
        .map(|x| x.parse::<usize>().unwrap())
        .collect::<Vec<usize>>();

    let mut gen1 = params[0];
    let mut gen2 = params[1];
    let mut gen3 = params[2];
    let n = params[3];

    let mut a = vec![0usize; n];
    for i in 0..n {
        a[i] = gen_next(&mut gen1, &mut gen2, &mut gen3);
    }

    // a.radix_sort_builder().with_low_mem_tuner().sort();
    // let mut ans = 0;
    // for i in 0..n {
    //     ans ^= a[i] * i;
    // }

    // println!("{}", ans);
}
