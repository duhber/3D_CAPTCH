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
            prev=$?
            mogrify -flip -format pgm $dir2/*.ppm
            
            if [ $? -eq 0 -a $prev -eq 0 ]
            then
                rm -f $dir2/*.ppm
                break
            fi

        done

        img="$dir2/frame_0000.pgm"
        keypoint="$dir2/temp"
        temp1="$dir2/temp.key"

        frame0="$dir2/frame_0000.p"
        
        descfile="$dir2/frame_0000.desc"

        ./sift <$img> $temp1 

        ./parseSiftKey $temp1 $keypoint $descfile 

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
