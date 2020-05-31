# II-MES
## Operating System

1. Linux.

Note: Use Windows Subsystem for Linux (WSL) if using Windows.

## Installation Instructions

1. Make sure you have downloaded all submodules. (```$ git submodules --help```)
2. ```$ make setup```
3. Edit the ```opc-ua-conf.txt``` file to match your configuration. (OPC-UA ID and network config)

Note: Install clang or edit the Makefile to use GCC.

## Compiling

1. ```$ make```

## Executing

1. ```$ make run```
2. Send your XML commands to the 54321 UDP port. Expect to receive responses on this same port.

## DataBase

1. Local dB (SQLiteStudio that uses sqlite3 module).
2. Tables are created by MES if they do not exist.

## Statistics

1. ```$ ./tabela``` in cmd to run the statistics script.
