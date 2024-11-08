#!/bin/bash
# if [ "$1" = "-d" ]; then
#     qemu-system-x86_64 -s -d int -S -hda Image/x64BareBonesImage.qcow2 -m 512 #-soundhw pcspk
# else
#    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 #-soundhw pcspk
# fi

if [ "$1" = "-d" ]; then
qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker
else
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker
fi
