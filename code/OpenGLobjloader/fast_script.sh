#!/bin/bash

dir=$1
stopPoint=$2
modeldir="model"
texdir="texture"
for i in $(ls $modeldir|grep '\.obj$');do
    
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

        modelname="./$modeldir/$i"

		while true
		do
			model2=$(ls $modeldir|grep '\.obj$' | shuf -n 1)
			if [ "$i" != "$model2" ]
			then
				break
			fi
		done
		modelname2="./$modeldir/$model2"

		teximg=$(ls $texdir|grep '\.bmp$' | shuf -n 1)
        texfilename="./texture/$teximg"
		while true;do
            ./a.out $modelname $modelname2 $dir2 "sx" $texfilename
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
        
        #if the image is just a black screen 

        numkeys=$(wc -l $keypoint|awk '{print $1}')
        if [ $numkeys -eq 0 ]
        then 
            continue
        else
            echo "OK $numkeys"
        fi


        cat $keypoint >> $frame0


        while true;do
            ./a.out $modelname $modelname2 $dir2 "st" $texfilename $frame0
            mogrify -flip -format jpg $dir2/*.ppm
            if [ $? -eq 0 ]
            then
                rm -f $dir2/*.ppm
                break
            fi

        done
done
