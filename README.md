<div align="center">

# AtomOS

![GitHub commit activity](https://img.shields.io/github/commit-activity/m/OfficialExedo/AtomOS)
[![License](https://img.shields.io/github/license/OfficialExedo/AtomOS)](./LICENSE)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/c9a167b5d4cf413e904a343550cf1500)](https://www.codacy.com/gh/OfficialExedo/AtomOS/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=OfficialExedo/AtomOS&amp;utm_campaign=Badge_Grade)


</div>

A hobbyist UNIX-like kernel made with C. The ultimate goal is to make it as POSIX compliant as possible and self-hosting


This project is a hobby OS so it's not perfect. I am trying to make the codebase as clean as possible, however as I'm still very much learning in this field, many parts are not the prettiest / most documented code ever.

Pull requests to help build this OS further are encouraged!

I highly reccomend checking out the osdev wiki if you're new as much of the code here is based on information found on the wiki. I've put some helpful links to the bottom of this README

# Currently supported features:
## CPU-based Features:
- [x] x86
- [ ] x86-64
- [ ] ARM
- [ ] Multitasking
- [ ] Multiprocessing
- [ ] User mode

## Kernel Features: 
- [x] Monolithic kernel
- [ ] Preemptive kernel
- [ ] Virtual filesystem
- [ ] ELF exec driver
- [ ] Memory management
- [ ] Ramdisk support
- [ ] Moduling system

## Supported Video Interface:
- [x] VGA text mode
- [ ] VESA 3.0 graphics

## System Features:
- [x] Multiboot compliant bootloader

# Stuff coming in future updates
- Code cleaning
- User mode
- A proper C standard library
- I/O device drivers
- Filesystem support

# Running AtomOS
## Building AtomOS
In order to build AtomOS, you'll need a GCC crosscompiler (the version of GCC bundled with your OS will **not** work). I'll write a guide on how to create this crosscompiler eventually but for now [this guide from the osdev wiki is the best source](https://wiki.osdev.org/GCC_Cross-Compiler). You'll also need a copy of NASM capable of assembling for i386.

Running `build.sh` will build all of AtomOS's files. You can build an ISO file with `iso.sh` (requires GRUB and Xorriso).

## Running AtomOS
The easiest way to run AtomOS is `qemu.sh`. This will automatically build an ISO from source and run it with QEMU. If you don't have QEMU installed, you can follow the instructions from the [official site](https://www.qemu.org/download) for your operating system.

**NOTE:** It's important to install QEMU with i386 support, on some platforms this may require installing additional packages (such as `qemu-arch-extra` for Arch Linux based distros)

AtomOS currently does not support any other emulators such as Bochs. If you can get this thing to run on real hardware you're a legend.

# Links
**osdev wiki and forum:**
- https://wiki.osdev.org && https://forum.osdev.org

**The osdev subreddit:**
- https://www.reddit.com/r/osdev/

**The James Molley kernel development tutorials**

Important to note that these articles are plagued with problems. I wouldn't reccomend them if you're brand new as they don't generally explain information that deeply, The tutorials also contain a fair amount of bugs. Check [this article](https://wiki.osdev.org/James_Molloy%27s_Tutorial_Known_Bugs) from the osdev wiki for a detailed list. I'm still linking these tutorials because they can help, just err on the side of caution.

- http://www.jamesmolloy.co.uk/tutorial_html/

**Writing a Simple Operating System — from Scratch - Nick Blundell**

A book that explains the beginning of an OS and bootloader from the ground up. Contains a fair amount of detail but stops after writing a simple VGA driver

- https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf

**os-tutorial - cfenollosa**

A very good resource if you're just starting out. Heavily based on the document by Nick Blundell and James M's tutorials.

- https://github.com/cfenollosa/os-tutorial


