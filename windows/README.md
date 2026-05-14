# Setting up a disk for the Windows VM

Currently, fetching the Windows ISO and booting that in a VM on LionsOS
doesn't 'just work'.

The main reason is that to boot an ISO directly and install Windows within
the virtual machine would involve us emulating a CD-ROM better and having an
emulated block device other than virtIO block.

Windows does not ship with virtIO drivers which means to make use of a
virtIO block device for the Windows installation, you would need to install
the virtIO drivers first so that it actually recognises it.

So, for now, we install 'locally' (directly on QEMU, no seL4) and then take
that Windows installation and boot with that for the LionsOS Windows VM.

Below are the steps on how to do the Windows installation.

## 1. Download ISOs

Download the Windows ISO for the version you want:
* [Windows 10](https://www.microsoft.com/en-us/software-download/windows10ISO)
* [Windows 11](https://www.microsoft.com/en-us/software-download/windows11)

Download the virtIO drivers:
```sh
wget https://fedorapeople.org/groups/virt/virtio-win/direct-downloads/archive-virtio/virtio-win-0.1.285-1/virtio-win-0.1.285.iso
```

You can find other versions/downloads of the virtIO drivers [here](https://fedorapeople.org/groups/virt/virtio-win/direct-downloads/archive-virtio/).

## 2. Install Windows on QEMU

This script below will create the virtual disk and launch QEMU with the
Windows ISO.
```sh
./install.sh [Windows ISO]
```

The install is largely automated using an [Autounattend script](./unattended/Autounattend.xml),
so the only thing you should have to do is click 'next' once or twice to start the installation.

Once you reach the Windows desktop, you can shutdown Windows.

## 3. Convert disk for LionsOS

For the disk that LionsOS sees, we need to have the Windows VM be on a partition,
this command will convert the QEMU disk:
```sh
./lionsos_disk.sh
```

Now you will have `lionsos_windows_disk.img`.
This is the disk to use for running the LionsOS system.
