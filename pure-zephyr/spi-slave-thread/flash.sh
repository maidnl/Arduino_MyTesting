#!/bin/bash

PORT="/dev/ttyACM0"


"/home/daniele/.arduino15/packages/arduino/tools/dfu-util/0.11.0-arduino5/dfu-util" --device 0x2341:0x0366 -D "./build/zephyr/zephyr.bin" -a0 --dfuse-address=0x08040000:leave
