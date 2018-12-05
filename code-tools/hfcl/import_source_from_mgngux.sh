#!/bin/bash

mkdir -p include
mkdir -p src

if test -d $1; then
  dstdir=`basename $1`
  cp $1/src src/$dstdir -a
  cp $1/include include/$dstdir -a
else
  echo $1 is not a directory
  exit 1
fi

cd src/$dstdir
for file in *.cpp
do
    dstfile=`basename $file .cpp`
    dstfile=${dstfile:1}
    dstfile=`echo $dstfile | tr [A-Z] [a-z]`
    dstfile=$dstfile.cc
    mv $file $dstfile
done

exit 0
