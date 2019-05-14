#!/bin/bash

file=$1

#check if header file
if [ ${file: -2} ==  ".h" ];then
	base=$(basename $file)
	toInsert="#include \"${base:: -2}.generated.h\""
	if grep -Fxq "$toInsert" $file;then
		echo "$file already contains $toInsert not Inserting again !"
	else
		echo "Inserting $toInsert into $file"
		echo "command: sed '1h;1!H;$!d;x;s/.*$toInsert[^\n]*/&\nfoo/' $file"
		sed -i "1h;1!H;\$!d;x;s/.*#include[^\n]*/&\n$toInsert/" $file
		echo "Inserting reflection macro into all classes"
		sed -i -E '/class/{N;s/\{/&\n\tGENERATED_ZCLASS/}' $file
	fi
#check if source file
elif [ ${file: -4} == ".cpp" ];then
	base=$(basename $file)
	toInsert="#include \"${base:: -4}.generated.cpp\""
	if grep -Fxq "$toInsert" $file;then
		echo "$file already contains $toInsert not Inserting again !"
	else
		echo "Inserting $toInsert into $file"
		echo "command: sed '1h;1!H;$!d;x;s/.*$toInsert[^\n]*/&\nfoo/' $file"
		sed -i "1h;1!H;\$!d;x;s/.*#include[^\n]*/&\n$toInsert/" $file
	fi
else
	echo "Not Supported File $file"
fi
