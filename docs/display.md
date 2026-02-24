### Display

A display of 2048 resolution (64*32) with a default scaling factor of 10, mapped
to an array of 2048 elements horizontally. The mapping continues to next row by
increasing the number with 1. So:

```
0  1  2  3  4  .... 63
64 65 66 67 68 .... 127
...
1984 1985 1986 .... 2047
```

Say we wanna reach (5, 6), we will calculate the index for `gfx`, we use

`cell = ((y % 32) * 64) + (x % 64)`

This ensures that the sprite is wrapped around when the co-ordinates are within
the display. Clipping of sprite is enabled as of now.

