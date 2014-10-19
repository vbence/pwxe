PHP Write Xor Execute
=====================

A PHP extension to block execution of writable files. PHP Write Xor Execute (*PWXE* in short) will stop with a fatal error on execution (also through *include* and *require*) of any file which is either writable or owned by the current user.

[Write XOR Execute](http://en.wikipedia.org/wiki/W%5EX) is a concept used by operating systems to ensure that arbitrary data is not executed. This feature (although for files instead of memory blocks) is applied here.

Many exploits work through uploaded or overwritten PHP scripts. This extension blocks that attack vector.

## Compiling

Get the binary with the following commands:

    phpize
    ./configure
    make

Install it (requires root privileges):

    sudo make install

You will have to **restart Apache** for the changes to take effect.

## Configuration

The extension can be enabled / disabled with the **pwxe.filter** (admin) flag. (It is enabled by default).
