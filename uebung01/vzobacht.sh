#! /bin/bash

if [ $# != 2 ]
then
echo argument $1 invalid
exit -1
fi

while :
do
	echo 5 biggest files in $1
	ls -S $1 | head -5
	echo 5 last modified files starting with $2 in $1
	ls -t $1 | grep ^$2 | head -5
	sleep 5
done


