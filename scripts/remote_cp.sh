#!/bin/bash

TARGET_DIR=/mnt/sdcard/eibsuite

if [ $# -lt 1 ]
then
	echo "Usage: $0 [files]"
	exit 1 
fi

scripts/ecolor "33;1" '*** Copying EIBSuite to Target devuce. (to terminate hit [ CTRL + C ] )***'
read -p "Enter Remote IP address (Empty to cancel): " rem_ip_addr
if [ -z $rem_ip_addr ] 
then
	exit 0
fi
read -p "Enter username (Empty to cancel): " rem_user
if [ -z $rem_user ] 
then
	exit 0
fi

scp -r -p $1 $rem_user@$rem_ip_addr:$TARGET_DIR

