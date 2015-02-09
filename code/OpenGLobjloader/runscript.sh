#!/bin/sh

dir=$2

while  [ -d "./frame/$dir" ]
do
    dir=$((dir+1))
done

dir2="./frame/$dir"

mkdir $dir2

./a.out $1 $dir2

mogrify -flip -format jpg $dir2/*.ppm
rm -f $dir2/*.ppm

img="$dir2/frame_0000.jpg"
#echo $img
keypoint="$dir2/temp"

frame0="$dir2/frame_0000.p"

keyimg="$dir2/frame_0000_keypts.jpg"

./fast-Linux-x86_64  $img $keyimg

./fast-Linux-x86_64 -l $img $keypoint

cat $keypoint >> $frame0

./a.out $1 $dir2 $frame0


mogrify -flip -format jpg $dir2/*.ppm
rm -f $dir2/*.ppm
