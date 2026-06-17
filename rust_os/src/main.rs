#![no_std]
#![no_main]

use core::panic::PanicInfo;
use core::ptr::write_volatile;

static HELLO: &[u8] = b"Hello World! This is a Rust OS without a desktop.";

#[unsafe(no_mangle)]
pub extern "C" fn _start() -> ! {
    let vga_buffer = 0xb8000 as *mut u8;

    for (i, &byte) in HELLO.iter().enumerate() {
        unsafe {
            write_volatile(vga_buffer.offset(i as isize * 2), byte);
            write_volatile(vga_buffer.offset(i as isize * 2 + 1), 0x0f); // White on black
        }
    }

    loop {}
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
