# interactive-fiction-game

To run the game just extract and cd to where your project folder is

After that using the dtekv toolchain type:
```
make
```

This will give you a main.bin file that you can either run using the emulator or on the dtekv board.
To run on the dtekv board you type:
```
dtekv-run main.bin
```
in the terminal
Note you may need to type 
``` jtagd --user-start ``` or ``` jtagconfig ```
 if you get the ``` Unable to connect to local JTAG server ``` error.
