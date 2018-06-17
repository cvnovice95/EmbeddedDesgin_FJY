/*  Windows 串口通信 步骤
*   1. 打开串口
*   2. 配置串口
*   3. 读写串口
*   4. 关闭串口
*/
#include <windows.h>
#include <stdio.h>
#define Async
//1.打开串口
/*
	CreateFile 函数说明
	HANDLE CreateFile(
		LPCTSTR  lpFileName, 指针类型变量 要打开的串口名 例如“COM1”;
		DWORD dwDesireAccess, 双字类型  指定串口的访问类型 写入 ,读取, 两者并入;  (GENERIC_READ|GENERIC_WRITE)
		DWORD dwShareMode, 双字类型 指定共享属性 串口不能共享 参数必须为0;
		LPSECURITY_ATTRIBUTES lpSecurityAttributes, 引用安全属性结构  参数默认值NULL。
		DWORD dwCreationDescription， 双字类型  创建标志 串口来说必须是 OPEN_EXISTING
		DWORD dwFlagsAndAttributes ，指定使用异步还是同步 异步参数（FILE_ATTRIBUTE_NORMAL |FILE_FLAG_OVERLAPPEND) 同步方式为 0；
		HANDLE hTemplateFile  串口通信参数必须为空；
		）
*/
HANDLE hCom;//全局变量 ,串口句柄;
bool  OpenCom(){
	hCom = CreateFileA(
		"COM3",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
#ifdef  Async
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,    //0 是 同步   (FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED)  异步
#else
		0,
#endif
		NULL
		);
	if (hCom == INVALID_HANDLE_VALUE){
		printf("%s\n", "OPEN COM FAILED!");
		return false;
	}
	return true;
}
//2.配置串口
/*
	DCB 结构体  
	typedef struct  _DCB{
		...
		DWORD  BaudRate ,   设置波特率   CBR_值   例如 CBR_115200
		DWORD fParty ,			设置 奇偶校验使能  1 为使能。默认为0
		BYTE ByteSize，         设置通信字节数   4-8
		BYTE Parity，				指定 奇偶校验方法  EVENPARITY 偶校验 ODDPARITY 奇校验 NOPARITY无校验 MARKPARITY无校验
		BYTE StopBits ，			设置停止位  ONESTOPBIT TWOSTOPBITS ONE5STOPBITS
		...
	}DCB;

	GetCommState 函数 获取COM的DCB
	{
		HANDLE hFile ,		指定串口句柄
		LPDCB，lpDCB		指向一个DCB（设备控制块）的指针
	}
	SetCommState 函数 设置COM的DCB
	{
		HANDLE hFile，
		LPDCB ，lpDCB
	}

	SetupComm 函数  设置缓冲区
	{
	   HANDLE   hFile，				指定串口句柄
	   DWORD dwInQueue ，		输入缓冲区大小字节数
	   DWORD dwOutQueue ，	输出缓冲区大小字节数
	}

	GetCommTimeouts 获取当前超时设置  
	{
		HANDLE hFiel，				指定串口句柄
		LPCOMTIMEOUTS lpComTimeout  ，	指向  COMMTIMEOUTS 的指针
	}
	GetCommTimeouts 设置当前超时设置
	{
	HANDLE hFiel，					指定串口句柄
	LPCOMTIMEOUTS lpComTimeout  ，		指向  COMMTIMEOUTS 的指针
	}

	typedef   struct _COMMTIMEOUTS{
		DOWRD ReadIntervalTimeout				读间隔超时   （读两个字符的时间差）
		DWORD ReadTotalTimeoutMultiplier		读时间系数
		DWORD ReadTotalTimeoutConstant		读时间常量
		DOWRD WriteTotalTimeoutMultiplier		写时间系数
		DOWRD WriteTotalTimeoutConstant		写时间常量
	}COMMTIMEOUTS

	在读写中  如有超时终止读写

	总超时计算 ： 总超时 = 读/写时间系数* 要读/要写的字符数+读/写时间常量

	PurgeComm 清空缓冲区
	{
		HANDLE hFile, 指定串口句柄
		DWORD flag ，指定操作
	}
	PURGE_TXABORT     中断写操作并返回 即使写操作没有完成
	PURGE_RXABORT     中断读操作并返回
	PURGE_TXCLEAR	  清空输出缓冲区
	PURGE_TXCLEAR	  清空输入缓冲区
*/

void ConfigCom(){
	//设置缓冲区大小
	SetupComm(hCom, 2048, 2048);
	//设置超时
	COMMTIMEOUTS CommTimeOut;
	CommTimeOut.ReadIntervalTimeout = 1000;
	CommTimeOut.ReadTotalTimeoutMultiplier = 500;
	CommTimeOut.ReadTotalTimeoutConstant = 1000;
	CommTimeOut.WriteTotalTimeoutMultiplier = 5000;
	CommTimeOut.WriteTotalTimeoutConstant = 1000;
	SetCommTimeouts(hCom, &CommTimeOut);

	//设置DCB
	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = CBR_115200;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	SetCommState(hCom, &dcb);

	//清空缓冲区
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
}
//3.读写串口
/*
	bool ReadFile(
			HANDLE hFile ,串口的句柄
			LPVIOD lpBuffer，空指针，指向 读入数据的存储地址
			DWORD dwNumberOfBytestoRead,
			LPDWORD lpNumberOfBytesRead，指向DWORD 类型的值， 该值为 实际读入的字节数
			LPOVERLAPPED lpOverlapped   异步时，该参数指向一个OVERLAPPED结构体，同步操作时，该参数为NULL
			）
	bool WriteFile(
			HANDLE hFile ，串口句柄
			LPCVOID lpBuffer, 空指针 ，指向 将要写入的数据的存储地址。
			DWORD nNumberOfBytesToWrite,    要写入的数据的字节数
		    LPDWORD lpNumberOfBytesWritten, 指向一个DWORD数值，该数值返回实际写入的字节数
		    LPOVERLAPPED lpOverlapped   异步时，该参数 指向一个OVERLAPPED结构体，同步操作时，该参数为NULL
			）
	typedef struct _OVERLAPPED{
				DWORD  Internal;   
				DWORD  InternalHigh;   
				DWORD  Offset;   
				DWORD  OffsetHigh;   
				HANDLE hEvent;     在异步操作时   操作完成时会有信号。调用ReadFile WriteFile时 设为无信号。
			}OVERLAPPED
		GetOverlappedResult 函数  判断异步操作是否完成
		{
				HANDLE hFile ,串口句柄
				LPOVERLAPPEND lpOverlapped, 异步操作 时的OVERLAPPEND 结构体指针
				LPDWORD lpNumberOfBytesTransferred，指向 一个DWORD值 该数值返回实际 读写 的字节数。
				BOOL bWait   该参数用于指定函数是否一直等到异步操作结束 TRUE 等到异步操作结束才返回  FALSE 函数立即返回 如果异步操作未完成 GetLastError()返回 ERROR_IO_INCOMPLETE。
		}

		BOOL ClearCommError(
				HANDLE hFile,				// 串口句柄
				LPDWORD lpErrors,			// 指向接收错误码的变量
				LPCOMSTAT lpStat			// 指向COMSTAT的结构体 该结构体返回当前串口的状态。
		  );
		typedef struct _COMSTAT{
					DWORD fCtsHold : 1;   // Tx waiting for CTS signal   
					DWORD fDsrHold : 1;   // Tx waiting for DSR signal   
					DWORD fRlsdHold : 1;  // Tx waiting for RLSD signal   
					DWORD fXoffHold : 1;  // Tx waiting, XOFF char rec''d   
					DWORD fXoffSent : 1;  // Tx waiting, XOFF char sent   
					DWORD fEof : 1;       // EOF character sent   
					DWORD fTxim : 1;      // character waiting for Tx   
					DWORD fReserved : 25; // reserved   
					DWORD cbInQue;        输入缓冲区的字节数
					DWORD cbOutQue;      输出缓冲区的字节数
		} COMSTAT;   


		ReadFile 之前 清除错误
*/
BOOL ReadSync(char InBuffer[],int InSize){
	//准备参数
	//char InBuffer[100];
	DWORD InBufferSize = InSize;
	DWORD ActualReadBytes;


	BOOL  ReadState;
	ReadState = ReadFile(hCom, InBuffer, InBufferSize, &ActualReadBytes, NULL);
	if (!ReadState){
		printf("%s\n", "READ FAILED!");
		return TRUE;
	}
	return FALSE;
}

DWORD ReadAsyncONE(char InBuffer[], DWORD InBufferSize){

	//char InBuffer[1024];
	DWORD ActualReadBytes = InBufferSize;
	BOOL  ReadState;

	//准备 ClearCommError 函数的参数
	DWORD ErrorFlags;                                      //接收错误码的变量
	COMSTAT comStat;

	OVERLAPPED overlApped;
	memset(&overlApped, 0, sizeof(OVERLAPPED));
	overlApped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//清除错误

	ClearCommError(hCom, &ErrorFlags, &comStat);

	//获取临界字节数 
	ActualReadBytes = min(ActualReadBytes, comStat.cbInQue);
	if (!ActualReadBytes){
		return 0;
	}
	//异步读取
	ReadState = ReadFile(hCom, InBuffer, ActualReadBytes, &ActualReadBytes, &overlApped);

	if (!ReadState){
		if (GetLastError() == ERROR_IO_PENDING){
			WaitForSingleObject(overlApped.hEvent, 2000);  //等待异步读操作完成  或 2秒后 函数才返回;

			PurgeComm(hCom, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);

			return ActualReadBytes;
		}
		return 0;
	}

	PurgeComm(hCom, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
	return ActualReadBytes;
}

DWORD ReadAsyncTwo(char InBuffer[],DWORD InBufferSize){
		//准备参数
	//char InBuffer[1024];
	DWORD ActualReadBytes = InBufferSize;
	BOOL ReadState;
	
	//准备ClearCommError 函数参数
	COMSTAT comStat;
	DWORD ErrorFlags;

	OVERLAPPED overlApped;
	memset(&overlApped, 0, sizeof(OVERLAPPED));

	//清除错误
	ClearCommError(hCom, &ErrorFlags, &comStat);
	//获取临界字节数 
	ActualReadBytes = min(ActualReadBytes, comStat.cbInQue);
	if (!ActualReadBytes){
		return 0;
	}
	ReadState = ReadFile(hCom, InBuffer, ActualReadBytes, &ActualReadBytes, &overlApped);

	if (!ReadState){
		if (GetLastError() == ERROR_IO_PENDING){
			GetOverlappedResult(hCom, &overlApped, &ActualReadBytes, TRUE); //GetOverlappedResult函数的最后一个参数设为TRUE,   函数会一直等待，直到读操作完成或由于错误而返回。      
			PurgeComm(hCom, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
			return ActualReadBytes;
		}
		return 0;
	}
	PurgeComm(hCom, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
	return ActualReadBytes;
}


BOOL WriteSync(char OutBuffer[],int OutSize){
	//准备参数
	//char OutBuffer[100];
	DWORD OutBufferSize = OutSize;
	DWORD ActualWriteBytes;
	DWORD  ErrorFlags;
	COMSTAT comStat;
	BOOL  WriteState;
	//清除错误
	ClearCommError(hCom, &ErrorFlags, &comStat);

	WriteState = WriteFile(hCom, OutBuffer, OutBufferSize, &ActualWriteBytes, NULL);
	if (!WriteState){
		printf("%s\n", "Write Failed!");
		return TRUE;
	}
	return FALSE;
}

DWORD WriteAsync(char OutBuffer[],int OutSize){
	//char OutBuffer[1024];
	DWORD ActualWriteBytes = OutSize;
	bool WriteState;
	COMSTAT comStat;
	DWORD ErrorFlags;
	OVERLAPPED overlApped;

	memset(&overlApped, 0, sizeof(overlApped));
	overlApped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	ClearCommError(hCom, &ErrorFlags, &comStat);

	
	WriteState = WriteFile(hCom, OutBuffer, ActualWriteBytes, &ActualWriteBytes, &overlApped);

	if (!WriteState){
		if (GetLastError() == ERROR_IO_PENDING){
			WaitForSingleObject(overlApped.hEvent, 1000);

			return ActualWriteBytes;
		}
		return 0;
	}
		return ActualWriteBytes;
}
//4.关闭串口
/*
   BOOL CloseHandle  
   {
		HANDLE hObject    指定要关闭的串口
   }
*/
void CloseCom(){
	CloseHandle(hCom);
}
int main(){
	//TEST  ReadAsyncONE  and  ReadAsyncTwo  and ReadSync
	OpenCom();
	ConfigCom();

	char INBUFFER[1024];
	char OUTBUFFER[10] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'k' };

	/*while (1){
		//ReadAsyncONE(INBUFFER,1024);
		//ReadAsyncTwo(INBUFFER,1024);
		//ReadSync(INBUFFER,1024);
		for (int i = 0; i < 1024; i++){
			printf("%c", INBUFFER[i]);
			if (i % 10 == 0){
				printf("\n");
			}
		}
	}*/

	//TEST WriteAsync AND WriteSync

	while (1){
#ifdef  Async
		WriteAsync(OUTBUFFER,10);
		ReadAsyncTwo(INBUFFER,1024);
#else

		WriteSync(OUTBUFFER,10);
		ReadSync(INBUFFER,10);
#endif
		for (int i = 0; i < 10; i++){
			printf("%c", INBUFFER[i]);
			if (i % 10 == 0){
				printf("\n");
			}
		}
	}
	CloseCom();
	return 0;
}





