#include <windows.h>
#include <stdio.h>
#define BUF_SIZE 1024;

int m3ain(int argc)
{


HANDLE hFile;
    DWORD NumberOfBytesRead = 0, dw;
   char *buf = new char[1024];
    OVERLAPPED overlapped;
    overlapped.Offset = overlapped.OffsetHigh = 0;  
    memset(buf, 0, 1024);

overlapped.hEvent = CreateEvent(NULL, true, false, NULL); 
if(NULL == overlapped.hEvent)
    printf("error");
		LPCWSTR fname = L"C:\\Users\\ANTadmin\\Desktop\\GNSSReader\\Tests\\ThreadTest.txt";
hFile = CreateFile(fname,
                  GENERIC_READ,
                  FILE_SHARE_READ,
                  NULL,
                  OPEN_EXISTING,
    FILE_FLAG_OVERLAPPED | FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING , 
                  NULL);



if (hFile == INVALID_HANDLE_VALUE)
        printf("invalid hfile\n");

   int i;   
   i= ReadFile(hFile,
                 buf,
                 1024,
                 &NumberOfBytesRead,
        &overlapped);
   if( GetLastError() == ERROR_IO_PENDING)
   {


       dw = WaitForSingleObject(overlapped.hEvent, INFINITE);
    if(dw ==  WAIT_OBJECT_0)
        if (GetOverlappedResult(hFile,&overlapped,&NumberOfBytesRead, TRUE) != 0)   
        {
            if (NumberOfBytesRead != 0) 
            {
                printf("!!!\n");
            }

        }

   }
  // std::cin.get();
   return 0;
}