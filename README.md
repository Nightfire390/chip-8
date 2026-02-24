# CHIP-8 Emulator

CHIP-8 is a virtual machine designed to run ch8 bytecode. It is designed to be smaller and simpler than languages like BASIC. It ran on COSMAC VIP, an 8 bit microcomputer.
`chip-8` is an emulator that runs CHIP-8 binary (rom) on modern hardware.

### Compilation

This emulator was created with linux in mind. It should work on other operating systems, given the dependencies are installed, but it is only tested on linux.
To compile the emulator, you need to have SDL3 installed for the display. Compiler used here was `gcc`:

```
git clone https://github.com/Nightfire390/chip-8.git
cd chip-8
make
```

### Usage

To run the emulator, execute the binary and provide rom path as an argument to the executable:

```
./chip-8 [rom]
```

Several roms are provided in the `roms` directory which were used to test the emulator.

### General

The frame rate has been capped to ~60 fps, with instructions running at ~1KHz.
The keypad has been mapped to the following keys:

```
Chip-8:     Keyboard:

1 2 3 C     1 2 3 4
4 5 6 D     Q W E R
7 8 9 E     A S D F
A 0 B F     Z X C V
```

Audio is not currently implemented.

### Compatibility

Roms with chip-8 quirks (i.e., no chip-48 or SUPER-CHIP) appear to work correctly.
The emulator passes [Timendus Test Suite](https://github.com/Timendus/chip8-test-suite), which has been very helpful for testing all the flags and quirks.
One exception is the `0xFX0A` instruction, where the test suite expects to halt all the execution, while the emulator tries to follow the technical specsheet, which differs from the test.

### References

- https://chip-8.github.io/extensions
- https://github.com/mattmikolay/chip-8/wiki/CHIP-8-Technical-Reference
- http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
- https://github.com/Timendus/chip8-test-suite
