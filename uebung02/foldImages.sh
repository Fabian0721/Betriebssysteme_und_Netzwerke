#! /bin/bash

if [ $# -gt 1 ] 
then
    echo "Too many arguments"
    exit 0;
fi 

if [ -d $1 ]
then
    echo "Directory $1 exists"

    originalDir=$PWD

    cd $1
    dirNames=$(ls -lt --full-time | grep -E '*.jpg|*.png|*.jpeg' | grep -Eo '[[:digit:]]{4}-[[:digit:]]{2}' | uniq)

    for dir in $dirNames
    do
        mkdir -p $dir
    done

    ls -lt --full-time |  grep -E '*.jpg|*.jpeg|*.png' | while read -r line
    do
        for dir in $dirNames
        do
            lineDate=$( echo $line | grep -Eo '[[:digit:]]{4}-[[:digit:]]{2}')
            if [ $lineDate = $dir ] && [ -d $dir ]
            then
                fileName=$( echo $line | grep -Eo '\w*.png|\w*.jpg|\w*.jpeg')
                mv $fileName $dir
            fi
        done
    done
    cd $originalDir
    exit 0
fi

echo "The directory does not exist"
exit 1