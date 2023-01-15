// Example custom build script.
fn main() {
    println!("cargo:rustc-link-search={}", ".");
    println!("cargo:rustc-link-lib=dylib=gen");
}
