# Othello (C + SDL2)

A two-player Othello/Reversi game written in C with SDL2.

## Features
- Click to place black/white stones, with legal-move checking and flipping
- Custom board size: `./othello -w 8 -h 8`
- Press `O` to skip a turn, `Q` to quit
- Winner printed in the console
- Board state saved to `field.txt`

## Build
Requires SDL2.

    gcc GUI.c -o othello $(sdl2-config --cflags --libs)

To play, run GUI.c. The code was entirely written in C.
