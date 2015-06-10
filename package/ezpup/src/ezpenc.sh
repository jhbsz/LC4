#!/bin/sh

# $1: key
# $2: infile
# $3: outfile

key="$1"
infile="$2"
outfile="$3"

keyhash()
{
    echo "key='$key'" 1>&2
    xstr=$(echo -n "$key" | md5sum | cut -d' ' -f1 | sed -n -e 's/\(..\)/\\x\1/gp')
    echo "xstr='$xstr'" 1>&2
    printf "$xstr"
}

if [ ! -x encrypt ];then
    echo Cannot find 'encrypt' program ; 
    exit 1
fi

if [ $# != 3 ];then 
    echo Usage: $(basename $0) key infile outfile; 
    exit 1
fi

export PATH=$PATH:$PWD

keyhash | encrypt 3des "$infile" "$outfile"

