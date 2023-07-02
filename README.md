# Pseudo 'top' implementation for Linux

### Nicolò Della Porta & Mattia Zingaretti

[Link to our repository](https://github.com/NycoDP/top_daje)

In this repository you will find an implementation of the pseudo "top" Linux command, which allows the user to monitor all processes' info and, possibly, to interact sending signals to them.

# How it is structured

In the main folder there are two directories: **`src`** and **`docs`**.
In **`src`** there are all the files containing the C implementation and their header files.
In **`docs`** there are some info we have needed to get in order to implement things righteously and there is a .txt file `(credits.txt)` that contains the most significant links we used to understand how some things worked: in particular for the `ncurses.h` lib, which really simplifies things to create a TUI using C language.
In addition to this, in this folder there are also some data structures we used to store processes data into (doubly linked list implementation taken from the SO repository :) ).

# How it works

A ncurses window is initialised and then starts the process of crawling information from the **`/proc/`** directory: all pid folders are read, then the **`/proc/[pid]/stat`** file is opened to read and store all process' info into an item that will be inserted into the main structure used to keep an item for each 'living' process.
Once all info of all processes have been stored, it proceeds to print all these informations divided in scrollable pages.
By scrolling, you can choose a process and decide whether to:

- kill it;
- terminate it (which basically means the same of killing it);
- suspend it;
- resume it (if it had been previously suspended).

# How to run

To properly run our project it is needed to run the ``install.sh``script that installs a library and creates a ``.bin`` folder used to collect binaries.
We implemented a simple Makefile that makes thing easier for everyone who desires to make it run.

In particular there are three rules:

- to compile the program:

```bash
    $ make    
```

    In the first two cases you will also need to launch the executable created inside `.bin` directory;

- to print all infos in stdout (*useful for debug*):

```bash
    $ make test   
```

- to remove binaries:

```bash
    $ make clean  
```
