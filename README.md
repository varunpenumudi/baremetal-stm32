# Baremetal Stm32 Projects

This Repository contains Barmetal Projects for the Stm32F103 Bluepill board, All these projects use the libopencm3 library and i compiled all of these on `arm-none-eabi-gcc` on windows `wsl`

## Requirements

- Stm32F103 Blue pill or Evaluation Board.
- Your System should run windows wsl or any linux distro.
- arm gcc toolchain should be installed on your system.
- st-link tools should be intalled for flashing and openocd should also be installed if debugging is needed.

## Steps to use

- Clone the Repository
- `cd` into any project folder
- `make all` to compile and generate bin files
- `make flash-bin` to make and flash the file

## Debugging

- Debugging Stm32 is also possible in vscode but openocd should be installed.
- open the project folder in a new window.
- Go to debug pane in left side bar
- Click debug it will start debugging.