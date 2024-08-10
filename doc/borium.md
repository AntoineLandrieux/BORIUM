
# BORIUM DOC

---

## COMPILER

### TOOLS

> Here we will use a Linux environment.

```sh
apt-get update && apt-get upgrade
sudo apt-get install nasm xorriso qemu build-essential grub2-common
```

### COMPILE AND RUN

After installing the OS compilation tools,
You can compile and run the OS with these commands:

```sh
make && make run
```

And then, you can delete the binary files by doing:

```sh
make clean
```
