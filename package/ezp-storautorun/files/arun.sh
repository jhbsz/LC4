#!/bin/sh

stpath=$STORPATH

[ -f "$stpath/firmware.bin" ] && {
    upgraded -a
}

