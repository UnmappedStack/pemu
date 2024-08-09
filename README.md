# PEmu
A PDP-8 emulator in C++

This project was the result of my initial interest in the PDP-8, how it could function with such few instructions (only 8!), and general old technologies (it's from 1965). It has grown into a rather painful project to develop for, since it's a 12 bit machine, which caused a lot of issues with alignation.

## Quickstart
Compile with:
```
g++ main.cpp -o pemu
```
then run a binary with:
```
./pemu [binary file name]
```
You may notice that everything is currently in one file. That's because the project is still in *very very very* early stages, and has been in development for only a few days at this point.

## Programs
You can find some programs in the `programs` directory, written in PDP-8 assembly, which you can compile with [this assembler](https://github.com/Dcraftbg/Pdp8).

Also, you can write your own programs, however very few instructions have actually been implemented so far, so you're better off waiting until the emulator is more advanced before you try write your own program for it.

Note that the IOT instruction is currently implemented as a LDI instruction for ease of development. This will be changed later for accuracy.

## Contributions
This repository is not open to contributions, as it is my hobby project, and for my education. You can open an issue if you have any ideas or find any bugs, and I'll try implement/fix it.

## License
This project is under the MIT license. See the LICENSE file for more information.
