
# BORIUM documentation

---

## COMPILER

### TOOLS

> Here we will use a Linux environment.

```sh
sudo apt update && sudo apt upgrade -y
sudo apt install nasm binutils build-essential mtools grub-common grub-pc-bin xorriso make -y

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

## PREDEFINED FUNCTIONS/KEYWORD

```html
clear              <keyword>  Clear screen
editor             <keyword>  Text editor
help               <keyword>  Show commands
license            <keyword>  Show license
pause              <keyword>  Interrupts the execution
setup              <keyword>  Change BORIUM settings
chr(ascii_code)    <function> Character from ASCII code
color(vga_color)   <function> Text color
cursor(x; y)       <function> Set cursor location
eval(code)         <function> Execute SOARE code
getc()             <function> Get char
input(...)         <function> Write text and ask for user input
keydown(scancode)  <function> Check if a key is pressed
ord(character)     <function> ASCII code from character
play_note(freq; t) <function> Play frequency (freq) for a while (t)
sleep(time)        <function> Pause for a while
system(cmd)        <function> Execute shell code
werr(...)          <function> Write text (error)
write(...)         <function> Write text
```
