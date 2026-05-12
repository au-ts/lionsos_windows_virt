#!/bin/bash

set -e
set -x

INPUT_DISK="windows_disk.img"
INPUT_DISK_RAW="windows_disk.raw"
OUTPUT_DISK="lionsos_windows_disk.img"

rm -f "${OUTPUT_DISK}"
rm -f "${INPUT_DISK_RAW}"

# First convert QEMU QCOW2 to raw disk
qemu-img convert "${INPUT_DISK}" "${INPUT_DISK_RAW}"

truncate -s 65G "${OUTPUT_DISK}"

sfdisk --no-reread --no-tell-kernel "${OUTPUT_DISK}" <<EOF
label: dos

start=2048,size=64G
EOF

# it is extremely important that bs=512 and seek=2048, as seek is in unit of `bs`.
dd if="${INPUT_DISK_RAW}" of="${OUTPUT_DISK}" bs=4096 conv=notrunc,sync,sparse seek=256 status=progress
