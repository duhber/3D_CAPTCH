#!/bin/sh
mogrify -flip -format jpg ./frame/frame_*.ppm
rm -f ./frame/*.ppm
