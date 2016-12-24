#!/bin/bash

PROCESS_FOLDER_EXEC=./process_folder.sh

# Get each folder name
ADJUSTMENT_FOLDERS=$(ls calibrate)

rm aggregated_results.dat

for ADJUSTMENT_FOLDER in $ADJUSTMENT_FOLDERS
do
    echo "Process adjustment ($ADJUSTMENT_FOLDER)"

    echo " - Remove existing data files."
    rm calibrate/${ADJUSTMENT_FOLDER}/data.txt

    echo " - Generate sharpness data."
    $PROCESS_FOLDER_EXEC calibrate/${ADJUSTMENT_FOLDER}/

    sed -i -e "s/^/${ADJUSTMENT_FOLDER}    /" calibrate/${ADJUSTMENT_FOLDER}/data.txt
    cat calibrate/${ADJUSTMENT_FOLDER}/data.txt >> aggregated_results.dat
done

exit 0

# Remove existing data files.
rm calibrate/-20/data.txt
rm calibrate/-15/data.txt
rm calibrate/-10/data.txt
rm calibrate/-5/data.txt
rm calibrate/0/data.txt
rm calibrate/5/data.txt
rm calibrate/10/data.txt
rm calibrate/15/data.txt
rm calibrate/20/data.txt

# Generate sharpness data
./process_folder.sh calibrate/-20/
./process_folder.sh calibrate/-15/
./process_folder.sh calibrate/-10/
./process_folder.sh calibrate/-5/
./process_folder.sh calibrate/0/
./process_folder.sh calibrate/5/
./process_folder.sh calibrate/10/
./process_folder.sh calibrate/15/
./process_folder.sh calibrate/20/

# Aggregate data
sed -i -e 's/^/-20    /' calibrate/-20/data.txt
sed -i -e 's/^/-15    /' calibrate/-15/data.txt
sed -i -e 's/^/-10    /' calibrate/-10/data.txt
sed -i -e 's/^/-5    /' calibrate/-5/data.txt
sed -i -e 's/^/0    /' calibrate/0/data.txt
sed -i -e 's/^/5    /' calibrate/5/data.txt
sed -i -e 's/^/10    /' calibrate/10/data.txt
sed -i -e 's/^/15    /' calibrate/15/data.txt
sed -i -e 's/^/20    /' calibrate/20/data.txt

rm aggregated_results.dat
cat calibrate/-20/data.txt >> aggregated_results.dat
cat calibrate/-15/data.txt >> aggregated_results.dat
cat calibrate/-10/data.txt >> aggregated_results.dat
cat calibrate/-5/data.txt >> aggregated_results.dat
cat calibrate/0/data.txt >> aggregated_results.dat
cat calibrate/5/data.txt >> aggregated_results.dat
cat calibrate/10/data.txt >> aggregated_results.dat
cat calibrate/15/data.txt >> aggregated_results.dat
cat calibrate/20/data.txt >> aggregated_results.dat
