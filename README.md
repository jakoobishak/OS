# OS - Linux Commands

## Lab 3 - UNIX

### Prep

1. What command do you use to open the manual page for `kill`?

   `man kill`

2. What's `pwd` an abbreviation of?

   `pwd` stands for Printing Working Directory.

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

### Exercise 1 - Environment Variables, $PATH and File Permissions

1. What line you run to look at only the `$EDITOR` variable from `env`?

   `env | grep $EDITOR`

2. How can you use `export` to append something to your `$PATH`?

   `export PATH = $PATH:/new/path`

3. How did you make the file `welcome` runnable?

   `chmod u+x /path/to/welcome`

4. After having made welcome into a runnable file,
   what's the octal representation of its file access permissions?

   `stat -c '%A %a' welcome`

   The commands above returns: `--rwxr--r--` 744 \
   (744 is octal value)

### Exercise 3 - Links and Hidden Files

1. What was the exact command used when creating the link?
   What's the order of the parameters when using the linking command?

   `ln -s ~/.dotfiles/.bashrc ~/.bashrc`

   `-s`: symbolic link

   `~/.dotfiles/.bashrc`: src

   `~/.bashrc`: dest

### Exercise 4 - Computer Plumbing

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

### Exercise 5 - Plumbing With `xargs`

1. When using cat on a file, to which file handle is the content sent?

   Standard input to standard output.

2. What parameter do you use to make `bash` execute a command (string)?

   `-c`

3. What was the full command for creating and announcing files?

   `cat arguments | xargs -I line bash -c "touch line && echo line was created!"`

4. What command did you use to remove the filenames listed in `arguments`?

   `cat arguments | xargs -I line bash -c "rm line"`

### Exercise 7 - Process Control

1. Instead of stopping a process and putting it in the background manually,
   you can do it by starting it in a certain way. How?

   Add `"&"` character at the end. For example, `long-stress &`

2. What command did you use to kill lots of jobs at once?

   The command is `kill %{1..n}`. For instance, `kill %{1..20}` kills the first 20 jobs.

### Exercise 8 ‐ File Ownership

1. What `chown` command was used to change group ownership?

   `chown jakoobi:tracing trace-beg`

2. What `chmdod` command was used to change access rights?

   `chmod u-rwx trace-beg` was used to remove [U]ser right to read, write and execute. \
   `chmod g+rwx trace-beg` was used to add the [G]roup right to read, write and execute.

3. What does the `x` flag mean for a directory?

   Allows the affected user to enter the directory, and access its sub files and directories.

### Exercise 9 A ‐ File Systems and Devices

`fstab` holds settings for how all disks and partitions are to be mounted on boot.

```
$ cat /etc/fstab
```

1. How many partitions are mounted through fstab on boot, and what are their mount points?

   The command above returns 2 rows, which means there is 2 mount points:

   1. `/boot`
   2. `/`

2. Which is the file (because in UNIX every- thing is a file) in / dev / that is mounted at the root (/)?

   `/dev/mmcblk0p2`

3. What's a command that returns the last (tail) event in the system log (`dmesg`) pertaining to the device mounted at /?

   `dmesg | grep / | tail -1`

4. Which file system (IdType) does the `udisk` system report that your partition mounted at / uses?

   ext4

   The following commands were used to find out `file system (IdType)`:

   1. `findmnt`
   2. `udisks info -b SOURCE` (SOURCE from `findmnt`)

### Exercise 9 B ‐ Memory

System memory info command:

```
$ cat /proc/meminfo
```

1. How much memory (in MB, please) does meminfo report?

   3883 MB

2. What's the name for swap on a Windows system?

   PAGEFILE (or page file)
