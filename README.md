# interactive-fiction-game

To run the game just extract and cd to where your project folder is.

After that assuming you have the dtekv toolchain type:
```
make
```

This will give you a main.bin file that you can either run using the emulator or on the dtekv board.

To run on the dtekv board you type:
```
dtekv-run main.bin
```
in the terminal.

Note that you may need to type
``` jtagd --user-start ``` or ``` jtagconfig ```
 if you get the ``` Unable to connect to local JTAG server ``` error.

# Game controls

We use switch 0-3 for movement where switch 0 moves left, 1 is right, 2 is up and 3 is down.

The 9th (last) switch is used for pausing the game, this also shows a map and tells you the controls.

We use the button for interacting with NPCS, items, and using the doors.
