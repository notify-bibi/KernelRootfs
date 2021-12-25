#!/bin/bash

# ./moduledeps.sh ~/source/linux-5.12-rc2/install drm_xen_front.ko
version=5.12.0-rc2
InstallPath=./rootfs
dirname=`realpath $1`
NL="
"
IFS="$NL"


mkdir -p $InstallPath/lib/modules/$version/
#cp $MD/modules.* $InstallPath/lib/modules/$Krelease/

dep_list=()

targetMod=`echo $2 | cut -d'.' -f1`
echo "targetMod: $targetMod"
for from_path in `modprobe --dirname=$dirname --set-version=$version --show-depends $targetMod | awk -F' ' '{print $2}' `; do
	to_path=`echo $from_path | sed ''s#$dirname#$InstallPath#g''`
	echo "$from_path >> $to_path"
	mkdir -p `dirname $to_path`
	cp $from_path $to_path
done;

