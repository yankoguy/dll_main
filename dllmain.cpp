// dllmain.cpp : Defines the entry point for the DLL application.

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DLL_EXPORT //has to be before dllmain.h
#include "dllmain.h"
#include "scanMemory.h"

#define BUF_SIZE 256
#define SHARED_MEMORY_FILE_NAME "myFile"
#define END_STREAM_TOKEN '!' //Char that indicates the end of memory transferring
#define MAX_COMMANDS_WAIT 3


extern "C"
{
	DECLDIR void ScanMemory() //exported function
	{
		MessageBoxA(0, "dwada", 0, 0);
		/*
		MEMORY_BASIC_INFORMATION mbi;
		SYSTEM_INFO si;
		LPVOID minAddress, maxAddress;

		GetSystemInfo(&si);
		minAddress = si.lpMinimumApplicationAddress; //minimal memory address fot current proccess
		maxAddress = si.lpMaximumApplicationAddress;  //maximal memory address fot current proccess
		
		while (minAddress < maxAddress)
		{
			VirtualQuery(minAddress, &mbi, sizeof(mbi));
			if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_READWRITE) SearchValue(&mbi);
			minAddress = (LPBYTE)mbi.BaseAddress + mbi.RegionSize;
		}
		*/
	}
	void SearchValue()
	{
		printf("I am not exported, can be called only within theDLL\n");
	}
}



BOOL APIENTRY DllMain(
	HANDLE hModule, // Handle to DLL module
	DWORD ul_reason_for_call,
	LPVOID lpReserved) // Reserved
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		FreeConsole();
		if (!AllocConsole())
		{
			MessageBoxA(0, "console was not created", 0, 0);

		}
		FILE *fDummy;

		if (freopen_s(&fDummy, "CONOUT$", "w", stdout)) {
			MessageBoxA(0, "Error in freopen_s", 0, 0);
		}

		// A process is loading the DLL.
		HANDLE hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, SHARED_MEMORY_FILE_NAME);
		if (hMapFile == NULL)
		{
			printf("Error in hMapFile %d", GetLastError());
			return TRUE;
		}
		LPSTR pBuf;

		pBuf = (LPSTR)MapViewOfFile(
			hMapFile, // handle to map object
			FILE_MAP_ALL_ACCESS, // read/write permission
			0, // start point (upper word)
			0, // start point (lower word)
			BUF_SIZE);

		if (pBuf == NULL)
		{
			printf("Error in MapViewOfFile %d",GetLastError());
			return TRUE;
		}
		dll_loop(pBuf);
	}
	return TRUE;
}




void proceess_commads(char* command) {
	//printf(command);
	scan();
}


void dll_loop(LPSTR communication_buffer)
{
	char line[BUF_SIZE];
	int line_index = -1;
	while (1)
	{
		Sleep(100);

		if (strlen(communication_buffer) != 0) 
		{
			line_index = -1;
			for (int i = 0; communication_buffer[i] != '\0'; i++)
			{
				line_index++;

				if (communication_buffer[i] == END_STREAM_TOKEN) {
					line[i] = '\0';
					proceess_commads(line);
				}
				else {
					line[line_index] = communication_buffer[i];
				}

			}
			memset(communication_buffer, '\0', BUF_SIZE); //clear pBuf
		}
	}
}

