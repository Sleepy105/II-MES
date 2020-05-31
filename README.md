# II-MES

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