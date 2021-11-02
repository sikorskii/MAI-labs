#!/bin/bash
rm -rf Tree
mkdir Tree
pushd "$PWD" >/dev/null
cd Tree
mkdir F
pushd "$PWD" >/dev/null
popd >/dev/null
pushd "$PWD" >/dev/null
popd >/dev/null
popd >/dev/null
cd Tree
rm -rf trueResult.txt
echo '/Tree/F' > trueResult.txt

cd ..
cp -f laba.sh Tree
cd Tree

#"./laba.sh" --help
cd ..
rm -rf result.txt
"./laba.sh" Tree

mv result.txt Tree
cd Tree
strings=$(cat result.txt)
ref=$(cat trueResult.txt)
#IFS=$'\n'

#exec 1>difference.txt

declare -a numbers
a=0
for string in $ref	
do
	numbers[$a]=`expr length $string`
	a=$(($a+1))
done
a=0
for string in $strings
do
	b=${#string}
	c=$(expr $b - ${numbers[$a]})
	#echo $c
	#c=$($b - ${numbers[$a]})
	echo ${string:$c} >> final.txt
	a=$(expr $a + 1)	
done

diff final.txt trueResult.txt > difference.txt
str=$(cat difference.txt)

if [ -z "$str" ]; then
	echo "OK"
elif [ -n "$str" ]; then
	echo "NOT OK"
fi

rm -rf result.txt
rm -rf trueResult.txt
rm -rf difference.txt
rm -rf final.txt
cd ..
rm -rf Tree
