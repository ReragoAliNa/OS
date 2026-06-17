# Rust OS

A very simple, bare-metal operating system kernel written in Rust, without a desktop environment. It prints a message directly to the VGA text buffer.

## Prerequisites

- Rust nightly (or an environment configured to compile `#![no_std]` programs)
- `rust-src` component (if needed for advanced features, though this simple version uses only core)
- Ensure the `x86_64-unknown-none` target is installed:

```bash
rustup target add x86_64-unknown-none
```

## Building

To build the kernel executable for the bare-metal target:

```bash
cargo build --target x86_64-unknown-none
```

This will produce a freestanding Rust executable that can be booted by a bootloader (e.g., using the `bootimage` crate, though that is out of scope for this simplest possible kernel that just provides an entry point).
