use rayon::prelude::*;
use rdst::RadixSort;
use std::io::{self, BufRead};
use std::thread;

const THREADS: usize = 8;
const UNIT: usize = std::usize::MAX / THREADS + 1;

#[inline]
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

#[inline]
fn xor(x: usize, y: usize) -> usize {
    x ^ y
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

    let gen1 = params[0];
    let gen2 = params[1];
    let gen3 = params[2];
    let n = params[3];

    let mut handles = vec![];

    for tid in 0..THREADS {
        let handle = thread::spawn(move || {
            let mut a: Vec<usize> = Vec::with_capacity(n);
            let mut g1 = gen1;
            let mut g2 = gen2;
            let mut g3 = gen3;
            for _ in 0..n {
                gen_next(&mut g1, &mut g2, &mut g3);
                if g1 / UNIT == tid {
                    a.push(g1);
                }
            }
            a.radix_sort_builder().sort();
            a
        });
        handles.push(handle);
    }

    let mut cnt = 0;
    let mut ans = 0;
    for handle in handles {
        let res = handle.join().unwrap();
        let sub = res
            .par_iter()
            .enumerate()
            .map(|(i, num)| (i + cnt) * num)
            .reduce_with(xor)
            .unwrap();

        cnt += res.len();
        ans ^= sub;
    }

    println!("{}", ans);
}
