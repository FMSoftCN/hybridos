#!/usr/bin/env bash
[ -n "$TOPDIR" ] && cd $TOPDIR
if [ "$1" == "-id" ]; then
	REV=`date "+%s"`
	echo "$REV"
else
	date -d "$(($1 -`date '+%s'`)) sec " +%y-%-m-%-d
fi
