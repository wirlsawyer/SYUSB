#ifndef _SYUSBMANGER_H_
#define _SYUSBMANGER_H_


#include "SYUSBDefine.h"

//#include <vector>

typedef int (WINAPI* SYUSB_TestAdd)(int a, int b);
typedef bool (WINAPI* SYUSB_RegisterWithHwnd)(HWND hwnd);
typedef bool (WINAPI* SYUSB_UnRegister)(void);
//typedef std::vector<SYWlanInfo*> *(WINAPI* SYWlan_Scan)(void);
//typedef bool (WINAPI* SYWlan_ConnetWithSSID)(WCHAR *wszSSID);

class SYUSBManger
{

public:	
	//creat
	static SYUSBManger* Instance(void);
	~SYUSBManger(void);
	//method
	int TestAdd(int a, int b);
	bool RegisterWithHwnd(HWND hwnd);
	bool UnRegister(void);
	//std::vector<SYWlanInfo*> *Scan(void);
	//bool ConnetWithSSID(WCHAR *wszSSID);

	//SYWlanInfo *GetNowConnectedSSID(void);
private:
	//method
	SYUSBManger(void);	


private:
	//var
	//creat
	static SYUSBManger*	m_instance;		
	//dll
	HMODULE  				m_hModule;

	SYUSB_TestAdd			m_DLLFuncTestAdd;
	SYUSB_RegisterWithHwnd	m_DLLFuncRegisterWithHwnd;
	SYUSB_UnRegister		m_DLLFuncUnRegister;
	//SYWlan_Scan				m_DLLFuncScan;
	//SYWlan_ConnetWithSSID	m_DLLFuncConnetWithSSID;
};

#endif