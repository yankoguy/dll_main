#pragma once

#ifdef DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif
extern "C"
{
	DECLDIR void ScanMemory(); //makes share to be seen in other programs
	void SearchValue();
	void dll_loop(LPSTR communication_buffer);

}