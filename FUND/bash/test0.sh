#!/bin/bash
rm -rf Tree
mkdir Tree
pushd "$PWD" >/dev/null
cd Tree
mkdir F && cd F
pushd "$PWD" >/dev/null
mkdir B && mkdir G
cd B
mkdir A && mkdir D
cd D
mkdir C && mkdir E
popd >/dev/null
pushd "$PWD" >/dev/null
cd G
mkdir I
cd I
mkdir H
popd >/dev/null
popd >/dev/null
cd Tree
rm -rf trueResult.txt
echo '/Tree/F/B/A
/Tree/F/B/D/C
/Tree/F/B/D/E
/Tree/F/B/D
/Tree/F/B
/Tree/F/G/I/H
/Tree/F/G/I
/Tree/F/G
/Tree/F' > trueResult.txt

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

#rm -rf result.txt
#rm -rf trueResult.txt
#rm -rf difference.txt
#rm -rf final.txt
#cd ..
#rm -rf Tree
