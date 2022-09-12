# git_drive
automatically keep updated with your GitHub reposities in windows
sadly, the branch is fixed with master, change the code if you wish.
system requirements: windows (win10 can work)

other requirements:
be sure your computer has already installed git, and has inited a folder.
you can check it out by using git pull or git commit in the certain folder and see if it works.

for me, I connect my git with GitHub reposities through SSH.

## In the folder "git_bat" 
you can copy the whole dir to c:\ if you wish to change less addresses.

1. start_update.vbs. 
please copy this file to "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp" if you wish to synchronize your folder with remote reposites automatically after your windows starts up.
it starts the "C:\git_bat\auto_update.bat" with 0, which means "do not show commad window", you can change it to 1 if you wish to see the cmd window.

2.auto_update.bat
**cd to the certain folder. you'd better change it to your own adress.**
it will pull the master brach unconditially, **it may be risky, be sure to check it out.**
after sleeping 3 seconds, it calls "C:\git_bat\start_monitor.vbs"

3.start_monitor.vbs
the only reason I write this is to excute C:\git_bat\git.exe silently(without cmd window)

4.auto_save.bat
this bat file will be called in the git.exe(build from source code git)
it pushes the local file to the remote server.
**still, you shall change the address here.**

## In the folder "code"
there's only one file called git.cpp
**you may have change some address**, it's easy because they are witten with variebles.

The **main idea** of the program is by using a windows api, witch send you a signal (kind of) when changes in the certain dirctory is detected.
some comments are written in chinese, don't worry, you can see a translation around.
