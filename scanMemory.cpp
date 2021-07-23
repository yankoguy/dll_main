#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>


void SearchValue(MEMORY_BASIC_INFORMATION* pMbi)
{
	LPVOID minAddress, maxAddress; //min and max addr of the memory block
	minAddress = pMbi->BaseAddress;
	maxAddress = (LPBYTE)minAddress + pMbi->RegionSize - sizeof(INT); //the reason for removing 4 bytes is to not exceed from our current mem block
	while (minAddress <= maxAddress)
	{
		if (*(INT*)minAddress == 985631547)
		{
			*(INT*)minAddress = 100;
			printf("foundddddd");
		}
		minAddress = (LPBYTE)minAddress + 1;
	}
}

void scan()
{
	printf("start searching...\n");
	MEMORY_BASIC_INFORMATION mbi;
	SYSTEM_INFO si;
	LPVOID minAddress, maxAddress;


	GetSystemInfo(&si);
	minAddress = si.lpMinimumApplicationAddress; //min mem addr of the process
	maxAddress = si.lpMaximumApplicationAddress; //max mem addr of process

	while (minAddress < maxAddress)
	{
		//runs on the memory
		VirtualQuery(minAddress, &mbi, sizeof(mbi));
		if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_READWRITE) SearchValue(&mbi);
		minAddress = (LPBYTE)mbi.BaseAddress + mbi.RegionSize;
	}
	printf("\nend searching");
}