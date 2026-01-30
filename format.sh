#!/bin/sh
clang-format -i $1
V1=$(sed 's/ \([*+-/%&|^><]\|>>\|<<\|>=\|<=\|==\|!=\) /\1/g' "$1" | clang-format)
V2=$(cat "$1")
if [ "$V1" = "$V2" ]; then
	sed -i 's/ \([*+-/%&|^><]\|>>\|<<\|>=\|<=\|==\|!=\) /\1/g' $1
else
	echo "Could not sed because it does not preserve the code :("
	echo "WRONG:"
	echo "$V1"
	echo "CORRECT:"
	echo "$V2"
fi;

