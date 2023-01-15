use rayon::prelude::*;
use rdst::RadixSort;
use std::io::{self, BufRead};

#[link(name = "gen")]
extern "C" {
    fn genfunc1(gen1: usize, gen2: usize, gen3: usize, n: usize, ptr: *mut usize);

    fn genfunc2(gen1: usize, gen2: usize, gen3: usize, n: usize) -> *mut usize;
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

    let mut arr = vec![0usize; n];
    let ptr = arr.as_mut_ptr();
    unsafe { genfunc1(gen1, gen2, gen3, n, ptr) };

    // let ptr = unsafe { genfunc2(gen1, gen2, gen3, n) };
    // let mut arr = unsafe {
    //     std::slice::from_raw_parts_mut(ptr as *mut _, n)
    // };

    arr.radix_sort_builder().sort();

    let ans = arr
        .par_iter()
        .enumerate()
        .map(|(i, num)| i * num)
        .reduce_with(|x, y| x ^ y)
        .unwrap();

    println!("{}", ans);
}
