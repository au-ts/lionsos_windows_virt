#!/bin/bash

set -ex

if [[ $# -lt 1 ]]; then
  echo "usage: ./install.sh [Windows ISO]"
  exit
fi

WINDOWS_ISO=$1

qemu-img create -f qcow2 windows_disk.img 64G
mkisofs -quiet -J -o unattended.iso unattended/

qemu-system-x86_64 \
  -accel kvm \
  -smp 4 \
  -m 8G \
  -cpu host \
  -drive file=windows_disk.img,if=virtio -bios ../libvmm/examples/uefi/board/x86_64_generic_vtx/OVMF.fd \
  -global virtio-blk-pci.disable-legacy=on \
  -drive file="${WINDOWS_ISO}",format=raw,media=cdrom,index=0 \
  -drive file=virtio-win-0.1.285.iso,format=raw,media=cdrom,index=1 \
  -drive file=unattended.iso,format=raw,media=cdrom,index=2 \
  -serial mon:stdio

