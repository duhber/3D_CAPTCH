#!/bin/bash

dir=$1
stopPoint=$2
for i in $(ls model|grep '\.obj$');do
    
        dir=$((dir+1))

        if [ $dir -le $stopPoint ]
        then
            continue
        fi


        while  [ -d "../frame/$dir" ]
        do
            dir=$((dir+1))
        done
        dir2="../frame/$dir"

        mkdir $dir2

        modelname="./model/$i"

        while true;do
            ./a.out $modelname $dir2
            mogrify -flip -format jpg $dir2/*.ppm
            if [ $? -eq 0 ]
            then
                rm -f $dir2/*.ppm
                break
            fi

        done
        img="$dir2/frame_0000.jpg"
        keypoint="$dir2/temp"

        frame0="$dir2/frame_0000.p"
        keyimg="$dir2/frame_0000_keypts.jpg"

        ./fast-Linux-x86_64  $img $keyimg
        ./fast-Linux-x86_64 -l $img $keypoint

        cat $keypoint >> $frame0


        while true;do
            ./a.out $modelname $dir2 $frame0
            mogrify -flip -format jpg $dir2/*.ppm
            if [ $? -eq 0 ]
            then
                rm -f $dir2/*.ppm
                break
            fi

        done
done
