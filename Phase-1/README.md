# Phase 1 — X11 and Processes

For Phase 1 of my Operating Systems class, I worked with a C program that uses the X Window System on Linux. The assignment introduced X11 window handling and process creation.

The program creates a parent window and can open child windows using `fork()` and `execve()`. It also handles mouse movement, keyboard input, and child-process signals.

## Controls in the code

- **A:** Change the window color.
- **C:** Create a child window.
- **X:** Open the X11 `xeyes` program.
- **Esc:** Exit. In the parent window, the code also sends termination signals to tracked child processes.

## Files

- [`xwin89.c`](xwin89.c) — Phase 1 C source code.

## Building

The original instructions call for a provided Makefile, which I haven't added here. On a Linux system with the X11 development library installed, the source lists this compile command:

```bash
gcc xwin89.c -Wall -Wextra -Werror -pedantic -ansi -lX11
```

Then run `./a.out` in an X11 graphical session. I haven't independently verified this uploaded copy builds with those strict flags.

## Credit

The starter program credits **Gordon Griesel** as the original author (January 23, 2025). I've kept that attribution in the source file. This repository contains the version I used for class; I'm not claiming to have written the entire starter program.

I'll add the later phases separately as I organize them.
