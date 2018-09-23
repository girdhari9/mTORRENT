WELCOME TO MTORRENT!

A terminal based miniTorrent for linux based operating systems.

Instructions:

1. Enter ./mtorrent to start the application

2. ./mtorrent give parameter with it. (<Your IP>, <Your PORT>, <Tracker IP>, <Tracker PORT>, <Download Log file name>) 

3. Enter ./tracker to start Tracker

4. ./tracker give parameter with it. (<Tracker IP>, <Tracker PORT>) 

5. '~/' refers to root directory with respect to the directory where the application started

For example: if application started in /home/user1/myfiles
"~/dir3/dir2" refers to "/home/user1/myfiles/dir3/dir2"

5. File/Directory names given without "~/" in the beginning are considered relative and will be assumed it is from current working directory

The application runs broadly in two modes
1. Client Mode (To take data)
2. Server Mode (Background Process To send data)

Client Mode:

1. Press "Y/y" + Enter key to continue.
2. Press "n/N" to exit from application
3. User can perform any of the following commands in the given format

Command: share

Syntax: share <source_file(s)> <filename.mtorrent file name>

Description: It will create .mtorrent file to upload that file. You will be added in tracker seederlist.

Examples:

share file1.txt file.mtorrent


Command: get

Syntax: get <mtorrent file name>

Description: It will download corresponding file and will store in current direcotry.

Examples:

get file.mtorrent


Command: remove

Syntax: remove <mtorrent file name>

Description: You will be remove from Tracker's seederlist.

Examples:

remove file1.mtorrent


Command: show downloads/download

Syntax: show downlaod

Description: It will show all downlaoded and uploaded files.


