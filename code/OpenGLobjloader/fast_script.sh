#!/bin/bash

dir=$1
stopPoint=$2
modeldir="model"

for i in $(ls $modeldir|grep '\.obj$');do
    
        dir=$((dir+1))

        if [ -d "../frame/$dir" ] && [ $stopPoint -eq 1 ]
        then
            continue
        fi

        while  [ -d "../frame/$dir" ]
        do
            dir=$((dir+1))
        done
        
        dir2="../frame/$dir"

        modelname="./$modeldir/$i"

        repeat=true

        while true;do

            mkdir $dir2

    		while true;do
                ./a.out $modelname $dir2 "1"
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

            #unproject all the FAST points : 2D to 3D

            ./a.out $modelname $dir2 "2" $frame0
            
            selPointFile="$dir2/challengepoint"

            number=$(sed -n "1"p $selPointFile)

            #echo "$number"

            line=$(sed -n "${number}p" "$keypoint")
            
            x=$(echo "$line"|awk 'BEGIN{FS=" "}{print $1}')
            y=$(echo "$line"|awk 'BEGIN{FS=" "}{print $2}')
            
            #echo "$line"
            #echo "$x"
            #echo "$y"
            
            sizex=25
            sizey=25
            sizexx=51
            sizeyy=51

            startx=$(($x-$sizex))
            starty=$(($y-$sizey))

            if [ $startx -lt 1 ]
            then
                startx=1
            fi
            
            if [ $starty -lt 1 ]
            then 
                starty=1
            fi 

            ty=$(($starty+$sizeyy-1))
            tx=$(($startx+$sizexx-1))

            if [ $ty -gt 399 ]
            then
                height=$(($sizeyy-$ty+399))
            else
                height=$sizeyy
            fi

            if [ $tx -gt 599 ]
            then 
                width=$(($sizexx-$tx+599))
            else
                width=$sizexx

            fi

            #echo "$height"x

            convert $img -crop "$height"x"$width"+"$startx"+"$starty" $bgtex
            mogrify -format bmp $bgtex

            if [ $? -ne 0 ]
            then
                continue
            fi


            unpfile="$dir2/frame_0000.u"
            bgtex="$dir2/bgtexture.bmp"
            #bgtex1="./texture/tex13.bmp"
            
            while true;do
                ./a.out $modelname $dir2 "3" $unpfile $bgtex $number
                if [ $? -eq 1 ]
                then
                    rm -rf $dir2
                    #$dir=$((dir-1))
                    break  
                fi
                mogrify -flip -format jpg $dir2/*.ppm
                if [ $? -eq 0 ]
                then
                    rm -f $dir2/*.ppm
                    repeat=false
                    break
                fi

            done
            
            if [ "$repeat" = false ]
            then
                break
            fi
	          
        done
done
