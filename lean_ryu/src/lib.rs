#![no_std]
#[no_mangle]
pub unsafe extern "C"  fn lean_hovercraft_format64(f : f64, result: *mut u8) -> usize {
    ryu::raw::format64(f, result)
}

#[cfg(not(test))]
#[no_mangle]
extern "C" fn rust_eh_personality() {}

#[cfg(not(test))]
extern "C" {
    fn lean_internal_panic(ptr: *const u8) -> !;
}

#[cfg(not(test))]
#[panic_handler]
unsafe fn panic_handler(_info: &core::panic::PanicInfo) -> ! {
    let info = b"liblean_hashbrown rust ffi panics\n\0";
    lean_internal_panic(info.as_ptr() as _)
}
