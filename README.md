# OS - Linux Commands

## Lab 4

### Prep

1. What command do you use to open the manual page for `kill`?

   man kill

2. What's `pwd` an abbreviation of?

   pwd stands for Printing Working Directory.

3. When using ls -l the first column is filled with a bunch of characters and dashes.
   What do they mean? What command can be used to modify them?

   `ls -l` prints working directory.

   The characters and dashes e.g. \
   `-rwxrw-r--` \
   `?UUUGGGOOOS`

   means the following:

   `?` - File `flag` type (d for Directory, - for file)

   `U`, `G`, `O` - Read, Write and exectutable modes for [U]ser, [G]roup, and [O]thers (everyone else)

   `S` - Alternative Access (blank means none defined)

   To modify them, use `chmod`.

4. There's a keyboard key which auto completes inputs in the terminal. Which key?

   Tab-key autocompletes.

5. What are the actions done by the commands `ls`, `mv`, `cp` and `rm`?

   `ls`: View the list of files and folders in the given directory.

   `mv`: Move a file to another location.

   `cp`: Copy a file to another location.

   `rm`: Remove files.

6. What command can be used as an interactive file reader?

   `nano`

7. Pressing `'q'` is a common way to exit an interactive program such as `less` or `man`.
   But what's the common shortcut to kill a program?

   Alternative Shortcut for quit `q` is `ctrl + c`

8. Suspend a job shortcut is `ctrl + z`

9. What was the original analogy used in the
   design of pipes?

   A waterpipe where the water represents sequence of bytes sent from one process to the other.

### Exercise 1

1. What line you run to look at only the `$EDITOR` variable from `env`?

   `env | grep $EDITOR`

2. How can you use `export` to append something to your `$PATH`?

   `PATH = $PATH:/new/path`

3. How did you make the file `welcome` runnable?

   `chmod u+x /path/to/welcome`

4. After having made welcome into a runnable file,
   what's the octal representation of its file access permissions?

   `stat -c '%A %a' welcome`

   The commands above returns: `--rwxr--r--` 744 \
   (744 is octal value)

### Exercise 3

1. What was the exact command used when creating the link?
   What's the order of the parameters when using the linking command?

   `ln -s ~/.dotfiles/.bashrc ~/.bashrc`

   `-s`: symbolic link

   `~/.dotfiles/.bashrc`: src

   `~/.bashrc`: dest

### Exercise 4

1. There are timestamps in the log from `dmesg`.
   They are not using EPOCH as their basis - when does time start for `dmesg`?

   Time since booting Linux.

2. What command can you run to filter out only the messages relating to "usb" from `dmesg`,
   with a context of five lines?

   `dmesg | grep usb | head -n 5`

3. What command did you use to make your printing program log to the two files?

   `file.bin 1> OUT.log 2> ERR.log`

4. What command did you use to look at the printout of `OUT.log` every second?

   `watch -n 1 "file.bin >> OUT.log 1>&1 | cat OUT.log"`
