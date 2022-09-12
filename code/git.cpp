// git.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <fileapi.h>
#include <errhandlingapi.h>
#include <stdlib.h>
#include <synchapi.h>
#include <stringapiset.h>

#define INTERVAL_TIME 5000
//char appCmdLine[] = "cscript.exe //NOLOGO C:\\git_bat\\qaq.vbs";
DWORD er;// erro number
void whenerro(void);
DWORD WINAPI save_git(LPVOID lpParam); //function of the thread
char git_command_address[] = R"(C:\git_bat\auto_save.bat)";
int main()
{

    char monitored_dir_address[] = "C:\\Users\\Home\\Desktop\\DAD";
    int change_num = 0;
    HANDLE thread_save_git,process_save_git;
    DWORD wait_result;
    HANDLE change_sig;
    

    while (1)
    {
        //每次重新建立handle时因为拷贝文件夹时会出现非常多个信号
        //主要思路是每当收到一个改变信号就close一次。
        //we need to re_establish the hadle because one change may trigger multiple times of handle
        //so i close the change_sig every loop
        change_sig = FindFirstChangeNotificationA(monitored_dir_address,
            TRUE, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE);
        if (change_sig == INVALID_HANDLE_VALUE)
        {
            whenerro();
            Sleep(50000);
            continue;
        }
        wait_result = WaitForSingleObject(change_sig, INFINITE);
        if (wait_result == WAIT_OBJECT_0)
        {
            change_num++;
            std::cout << "change detected: " << change_num<<"\n";
            //FindNextChangeNotification(change_sig);
            FindCloseChangeNotification(change_sig);
            
            //尝试使用多线程来确保检测时间
            //但是考虑到git的操作本身会影响文件夹，从而又触发信号
            //还是需要等待结束。
            //如果实在等不到就直接消灭线程防止内存泄露
            //多线程有大问题，无法正确的释放资源
            //现在尝试多进程。
            //use multi_proc instead of multi_thread to avoid memory leaks
            STARTUPINFOA si;
            PROCESS_INFORMATION pi;

            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));
            if (CreateProcessA(NULL,
                git_command_address,
                NULL,
                NULL,
                FALSE,
                CREATE_UNICODE_ENVIRONMENT,
                NULL,
                NULL,
                &si,
                &pi) == 0)
            {
                std::cout << "fail\n";
                whenerro();
            }
            else
            {
                std::cout << "pro\n";
            }
            DWORD proc_status = WaitForSingleObject(pi.hProcess, 60000);
            if (proc_status == WAIT_TIMEOUT)
            {
                std::cout << "process terminated:" << pi.dwProcessId;
                TerminateProcess(pi.hProcess, 3);
            }
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            Sleep(INTERVAL_TIME);


            //以下是之前尝试多线程的残余
            //down blow is the monument of failure with multi_thread

            /*thread_save_git = CreateThread(NULL, 0, save_git, NULL, THREAD_ALL_ACCESS, NULL);// new thread
            if (thread_save_git == NULL)
            {
                continue;
            }
            std::cout << "thread craeted\n ";*/
            
            /*DWORD thread_status = WaitForSingleObject(thread_save_git, 20000);
            if (thread_status == WAIT_TIMEOUT)
            {
                TerminateThread(thread_save_git,5);//fails here by unable to relocate resources
            }
            CloseHandle(thread_save_git);*/
            //system("C:\\git_bat\\time");
            
            
        }
        else
        {
            FindCloseChangeNotification(change_sig);

        }
    }


    //my first thougt is down here, by monitering the info of the file every few secends
    //it turns out to be a failure because it can't monitor the changes of the files.
    //it's only useful when adding or deleting files. 
    //之前的想法是每隔几秒检测一次，但是目录文件的修改不会改变目录本身的属性，所以寄了。
    //只有在添加或者删除文件时有用。


    //FILE* p = fopen("C:\\Users\\Home\\Desktop\\DAD", "r");Users/Home/Desktop/test/t.txt
    /*
    FILETIME  ftWrite_1, ftWrite_2;
    HANDLE test = CreateFile(TEXT("C:\\Users\\Home\\Desktop\\test"),
        GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL| FILE_FLAG_BACKUP_SEMANTICS,NULL);//FILE_FLAG_BACKUP_SEMANTICS is needed
    if (test == INVALID_HANDLE_VALUE)
    {
        whenerro();
    }
    else
    {
        std::cout << "success\n";
        //system("pause");
    }
    if (GetFileTime(test, NULL, NULL, &ftWrite_1) == 0)
    {
        whenerro();
    }
    while (1)
    {
        Sleep(2000);
        if (GetFileTime(test, NULL, NULL, &ftWrite_2) == 0)
        {
            whenerro();
            break;
        }
        LONG timeResult = CompareFileTime(&ftWrite_1, &ftWrite_2);
        switch (timeResult)
        {
        case -1://First file time is earlier than second file time.
            ftWrite_1 = ftWrite_2;
            std::cout << "good\n";
            system("C:\\git_bat\\time");
            break;
        case 0://First file time is equal to second file time.'
            std::cout << "no\n";
            break;
        case 1://should not happen
            ftWrite_1 = ftWrite_2;
            system("C:\\git_bat\\time");
            break;
        }


    }*/

}
void whenerro(void)
{
    er = GetLastError();
    std::cout << "发生错误\n";
    std::cout << "errno:\n" << er;
    //system("pause");

}
DWORD WINAPI save_git(LPVOID lpParam)
{
    system(git_command_address);
    return 0;
}

/*HANDLE CreateFileA(
  [in]           LPCSTR                lpFileName,
  [in]           DWORD                 dwDesiredAccess,
  [in]           DWORD                 dwShareMode,
  [in, optional] LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  [in]           DWORD                 dwCreationDisposition,
  [in]           DWORD                 dwFlagsAndAttributes,
  [in, optional] HANDLE                hTemplateFile
);*/


