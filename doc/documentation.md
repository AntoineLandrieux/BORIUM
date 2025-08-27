
# BORIUM documentation

---

## COMPILER

### TOOLS

> Here we will use a Linux environment.

```sh
sudo apt update && sudo apt upgrade -y
sudo apt install nasm binutils build-essential grub-common grub-pc-bin xorriso make -y

# Test it using Qemu !

sudo apt install qemu-system 
```

### COMPILE AND RUN

After installing the OS compilation tools,
You can compile and run the OS with these commands (QEMU is required for `make run`) :

```sh
make
make run
```

And then, you can delete the binary files by doing:

```sh
make clean
```
