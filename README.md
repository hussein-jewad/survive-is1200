# interactive-fiction-game
The simplest way to run the game is to download main.bin in the folder "project" and loading it to the emulator (see link below).

To compile the game just extract the zip and cd to where your project folder is.

After that assuming you have the dtekv toolchain type:
```
make
```

This will give you a main.bin file that you can either run using the emulator or on the dtekv board.

To run on the emulator just go to this link:
https://dtekv.fritiof.dev/
and load the main.bin file.

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

The 9th (last) switch is used for pausing the game, this also shows a map and tells you the movement controls.

We use the button for interacting with NPCS, items, and doors.
