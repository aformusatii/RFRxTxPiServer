#!/bin/bash
# pico2wave -l en-US -w=/tmp/test.wav "$1"
# aplay /tmp/test.wav
# rm /tmp/test.wav
mpg123 "http://127.0.0.1:8080/ivona-server-1.0.0/say?data=$1,&voiceName=Amy&useCache=$2"
