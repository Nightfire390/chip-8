### Keypad
The keypad implmentation keeps track of what keys are being pressed down and which ones aren't via a single array of 16 elements i.e., `keys[16]`.

The keypad has been mapped to the following keys:

```
Chip-8:     Keyboard:

1 2 3 C     1 2 3 4
4 5 6 D     Q W E R
7 8 9 E     A S D F
A 0 B F     Z X C V
```

The original hardware was single threaded, and so is the emulator. Due to the limitation, the emulator cycles through the keyboard to match the key\_down and key\_up event with the actual value.
Any other keys pressed are ignored and the keyboard event handler functions return `0xff` which is ignored by the emulator.
