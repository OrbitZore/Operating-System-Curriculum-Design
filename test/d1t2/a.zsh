#!/bin/zsh
a=
typeset -A a
while read i; do 
        ((a[$i]++));
done < data
for k v in ${(kv)a}; do
        echo "$k -> $v";
done