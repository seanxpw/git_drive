**中文版本的有点写不动了，详细信息请先参考英文版。**

系统：windows

这个程序可以同步指定的文件夹与远程的git仓库。
首先需要确保这个文件夹能够正常使用git，如git pull, git push
## git_bat 文件夹介绍

git_bat 文件夹放有一些程序需要的脚本，里面的地址是写死了的，还有默认是同步分支是master还请注意。

可以将整个git_bat文件夹拷贝到c盘根目录下。

如果要查看文件的执行逻辑请从start_update.vbs看起，然后一步步跟踪。
如果想让程序开机自动运行请拷start_update.vbs文件到
C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp

需要更改cd路径的bat文件有
1. auto_update.bat
2. auto_save.bat

vbs用于调用bat，可能也要修改路径。

## 源代码介绍

主要思路是调用win api检测文件夹下的文件是否更新，如果更新了就运行auto_save.bat，来进行git push等一套操作来同步。
如果真的要尝试使用本程序，请更改git.cpp代码中的路径。

