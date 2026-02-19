NES Emulator Playground
-----------------------

:construction: Work in progress...

NES emulator written in C++ as a hobby project. The main goal is to better understand NES hardware internals and experiment with new C++ language features. This emulator is not cycle-accurate. Emulation quality is enough to play many real NES games. Only two mappers are implemented. To compile, you'll need a compiler with C++23 support and SFML 2 installed.

Since the NES is based on the 6502 CPU, this project also includes an emulator for Microsoft BASIC (https://github.com/mist64/msbasic) running on the 6502-based Tangerine MicroTAN computer.
