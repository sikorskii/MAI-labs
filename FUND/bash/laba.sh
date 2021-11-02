#!/bin/bash
function goThrough {
cd $1
files=`ls`
i=0
for file in $files
do

	if [ -d "$file" ]
	then
		i=$[$i+1]
	else continue
	fi 
done

if [ $i -ne 0 ]; then

for file in $files
do
	if [ -d "$file" ]
	then
	goThrough $file	
	echo "$PWD"
	cd ..
	continue
       	else
       	       continue
	fi	
done

else	echo "$PWD" >/dev/null
	
fi
}
if [ "$1" = "--help" ]
then 
echo 'This script goes through your directory tree and prints full path to every directory in tree. 

Moving begins in directory whose name is put into the var $1. 

For example: "./laba.sh /home/aldes/Tree".

You can run this script without putting any path in $1.

In this case script will begin its travel from its own directory.

Add "--help" flag to see this message again.'
else
exec 1>result.txt
goThrough $1
fi
