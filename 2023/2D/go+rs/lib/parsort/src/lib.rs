use rayon::prelude::*;
use rdst::RadixSort;

#[no_mangle]
pub extern "C" fn parsort(size: u64, ptr: *mut u64) {
    let arr = unsafe {
        std::slice::from_raw_parts_mut(ptr as *mut _, size as usize)
    };
    arr.radix_sort_builder().sort();
}

#[no_mangle]
pub extern "C" fn parsort_and_output(size: u64, ptr: *mut u64) {
    let arr = unsafe {
        std::slice::from_raw_parts_mut(ptr as *mut _, size as usize)
    };
    arr.radix_sort_builder().sort();

    let ans = arr
            .par_iter()
            .enumerate()
            .map(|(i, num)| i * num)
            .reduce_with(|x, y| x ^ y)
            .unwrap();
    
    println!("{}", ans);
}
