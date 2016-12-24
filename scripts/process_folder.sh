#!/bin/bash
#FILES=/home/jfoytik/jake_dev/calibrate/0/*
FOLDER=$1
FILES=$1/*
for f in $FILES
do
   ./locate_target $f >> roi.txt  
   ./get_sharpness $f roi.txt >> $FOLDER/data.txt
done
