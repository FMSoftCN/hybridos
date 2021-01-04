#!/bin/bash

BASE_DIR=$(dirname $(readlink -f "$0"))
SOURCE_DIR="$BASE_DIR/../../Source/"
WEBKIT_OUTPUTDIR="$BASE_DIR/../../Output/HBD/"

# LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$WEBKIT_OUTPUTDIR/Root/lib/:$LD_LIBRARY_PATH 

# mgncs.cfg
export NCS_CFG_PATH=$WEBKIT_OUTPUTDIR/Root/etc/
