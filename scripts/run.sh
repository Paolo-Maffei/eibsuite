#!/bin/bash
if [ -z "$1" ]
then 
	echo 'usage: $0 <program name>'
	exit
fi

export LD_LIBRARY_PATH=`pwd`
./$1

