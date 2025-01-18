# CSS Project Part II
- Note that the work on Part I is completely done. The submission is omitted here as Vinod Sir has advised to only submit the Part II.
## Compiling the kernel and booting it in Virtual Box
 - Downloading the kernel
 ```bash
 git clone https://github.com/torvalds/linux.git
 ```
 - Make a Build Folder
 ```bash
 mkdir build
 make O=../build x86_64_defconfig
 make O=../build makeconfig
 # Note make changes to BPF subsystem here.
 make O=../build -j$(nproc)
 ```
 - Once you have the bzImage in the arch/x86_64/boot folder, we have to copy it into the VM. Execute the following command in the Virtual Machine.
 ```bash
 scp hostname@hostip:path/to/bzImage ./
 ```
 - I was running server ubuntu 24.04.01 on the VM you can choose any appropriate versions.
 - Placing the bzImage in the boot folder and generating necessary dependencies
 ```bash
 sudo cp bzImage /boot/vmlinuz-custom
 sudo update-initramfs -c -k all
 sudo update-grub
 sudo reboot now
 ```
 - Now you will booting into the kernel image we compiled, or else make sure you press ESC and choose the right kernel to boot into. This is the baseline of how I have compiled the kernel in the LAB pc and copied the bzImage into Virtual Machine after the changes I have made to the kernel with adding the *Dummy Syscall* definition.

## Dummy Syscall Implementation

- Writing the code base
```bash
  cd path/to/the/linux/kernel
  mkdir dummy
  touch dummysc.c
  touch Makefile
```
- *dummysc.c* will have the definition of the system call, the Makefile will facilitate in compiling the modified kernel
- We have to add a simple line in the Makefile at this folder tree
```bash 
obj-y := dummysc.o
```
- Updating the kernel Makefile
```bash
cd ..
nano Makefile
#add /dummy at the line "core-y"
```
- Add the Dummy Syscall to syscall table
```bash
cd arch/x86/entry/syscalls/
nano syscall_64.tbl
# Add the following line: 548   64   dummy    sys_dummy
```
- Add the header info in syscall.h 
```bash
cd include/linux/
nano syscalls.h
# Add the following line at the end of the file : asmlinkage long sys_dummy(int arg)
```
- Compile the modified kernel
```bash
make O=../build -j$(nproc)
```
- Note that we have to follow the same steps described in the initial part of the readme and copy the modified kernel bzImage into the VM and reboot after updating necessary components.
- We can test the working of the modified kernel by running a file that has syscalls to dummy sys call, the code that has been passed through Part 1 LLVM dummysyscall pass and check the dmesg to see the log and verify the implementation.



## Kernel Monitor

- Note that the work on kernel monitor is still under progress. The submitted file has a bootstraping code that prints the Libc Call ID the dummy syscall is called with as an argument.
- Writing the code base
```bash
touch lib_monitor.py
# Install necessary bpftools
# The contents of the file have been submitted in the zip file.
chmod +x lib_monitor.py
sudo python3 lib_monitor.py &
# Now run the .out program to see the ebpf monitor the code
./program.out
```
