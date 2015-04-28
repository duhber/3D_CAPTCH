#!/bin/bash

dir=$1
stopPoint=$2
modeldir="testmodel"
modeldir2="ocmodel"
texdir="texture"
texdir2="octexture"
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

		#while true
		#do
			model2=$(ls $modeldir2|grep '\.obj$' | shuf -n 1)
			#if [ "$i" != "$model2" ]
			#then
			#	break
			#fi
		#done
		modelname2="./$modeldir2/$model2"

		teximg=$(ls $texdir|grep '\.bmp$' | shuf -n 1)
        texfilename="./texture/$teximg"
		teximg2=$(ls $texdir2|grep '\.bmp$' | shuf -n 1)
        texfilename2="./octexture/$teximg2"
		while true;do
            ./a.out $modelname $modelname2 $dir2 "sx" $texfilename $texfilename2
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
        bgtex="$dir2/bgtexture.jpg"

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
        
        RANGE=$numkeys

        number=$RANDOM
        let "number %= $RANGE"

        #echo "$number"

        line=$(sed -n "${number}p" "$keypoint")
        
        x=$(echo "$line"|awk 'BEGIN{FS=" "}{print $1}')
        y=$(echo "$line"|awk 'BEGIN{FS=" "}{print $2}')
        
        #echo "$line"
        #echo "$x"
        #echo "$y"
        
        startx=$(($x-50))
        starty=$(($y-50))

        if [ $startx -lt 1 ]
        then
            startx=1
        fi
        
        if [ $starty -lt 1 ]
        then 
            starty=1
        fi 

        ty=$(($starty+100))
        tx=$(($startx+100))

        if [ $ty -gt 399 ]
        then
            height=$((101-ty+399))
        else
            height=101
        fi

        if [ $tx -gt 599 ]
        then 
            width=$((101-tx+599))
        else
            width=101

        fi

        echo "$height"x

        convert $img -crop "$height"x"$width"+"$startx"+"$starty" $bgtex
        mogrify -format bmp $bgtex

        if [ $? -ne 0 ]
        then
            continue
        fi



        while true;do
            ./a.out $modelname $modelname2 $dir2 "st" $texfilename $texfilename2 $frame0
            mogrify -flip -format jpg $dir2/*.ppm
            if [ $? -eq 0 ]
            then
                rm -f $dir2/*.ppm
                break
            fi

        done
	#	break
done
