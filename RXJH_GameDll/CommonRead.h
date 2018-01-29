#pragma once

inline DWORD Read_RD(DWORD dwData)
{
	_asm
	{
		mov ebx,dwData
		mov eax,[ebx]
	}
}

inline DWORD Read_RB(DWORD dwData)
{
	_asm
	{
		mov ebx,dwData
		mov eax,0
		mov al,BYTE PTR DS:[ebx]
	}
}


inline char* Read_RS(DWORD dwData)
{
	_asm
	{
		mov ebx,dwData
		mov eax,ebx
	}
}

inline float Read_RF(DWORD dwData)
{
	DWORD dwMem = *((DWORD*)dwData);
	//revert
	DWORD* pMem = &dwMem;
	float* pRet = (float*)pMem;
	float ret = *pRet;
	return ret;

	/*_asm
	{
		mov ebx,dwData
		FLD [ebx]
	}*/
}
