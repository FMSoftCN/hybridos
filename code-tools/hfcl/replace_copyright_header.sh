#!/bin/bash

for dstdir in common activity view animation components drawable mgcl resource graphics services
do
    for file in include/$dstdir/*.h
    do
        cat header.template.h > /tmp/tmp.h
        cat $file | awk -f code-tools/copyright-script/remove_header.awk >> /tmp/tmp.h
        mv /tmp/tmp.h $file
    done

    for file in src/$dstdir/*.cc
    do
        cat header.template.cc > /tmp/tmp.cc
        cat $file | awk -f code-tools/copyright-script/remove_header.awk >> /tmp/tmp.cc
        mv /tmp/tmp.cc $file
    done
done
