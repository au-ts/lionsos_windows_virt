# Windows virtualisation on seL4/LionsOS

Work in progress, only runs on QEMU at the moment.

Windows 10 works, Windows 11 with TPM/Secure Boot checks bypassed also works.

Clone the source:
```
git clone https://github.com/au-ts/libvmm.git
cd libvmm
git checkout windows_mark_ii
git submodule update --init
cd examples/windows
```

Then please see https://github.com/au-ts/libvmm/tree/windows_mark_ii/examples/windows for more details.
