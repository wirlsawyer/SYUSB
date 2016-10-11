// SYUSB.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "SYUSBDefine.h"
bool flag_Debug = true;

//[USB]
#include <dbt.h>
#include <setupapi.h> // for HDEVINFO
#pragma comment(lib, "setupapi.lib")
HDEVNOTIFY g_hDevNotify;

//[EnumerateDevice]
#include <devguid.h>  // for GUID_DEVCLASS_USB
#include <setupapi.h> // for HDEVINFO
#include <Cfgmgr32.h> // for CM_Get_Device_IDA
#pragma comment(lib, "setupapi.lib")

typedef struct _DEVICE_INFO_
{
	wchar_t HardwareID[256];
	wchar_t CompatiableID[256];
	wchar_t PNPDeviceID[256];
}DEVICE_INFO,*PDEVICE_INFO;

//------------------------------------

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:	
		g_hDevNotify = NULL;

		//g_pVecWlan = new std::vector<SYWlanInfo*>;	

		break;

	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		
		/*
		if (g_pVecWlan)
		{
			//clear
			while(!g_pVecWlan->empty())
			{
				delete g_pVecWlan->back();
				g_pVecWlan->pop_back();
			}
			delete g_pVecWlan;
		}
		*/
		break;
	}


    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

//======================================================================================================================
int WINAPI TestAdd(int a, int b)
{
	return a+b;	
}

//======================================================================================================================
bool WINAPI RegisterWithHwnd(HWND hwnd)
{
	if (g_hDevNotify) return true;
	// ¨Ï¥Î device interface ªº½d¨Ò
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));

	NotificationFilter.dbcc_size = sizeof(NotificationFilter);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_reserved = 0;

	//NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;

	
	g_hDevNotify = RegisterDeviceNotification(hwnd, &NotificationFilter, DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);

	if (g_hDevNotify == NULL)
	{
		if (flag_Debug)	OutputDebugString(L"Fault RegisterDeviceNotification!\n");
		return false;
	}
	if (flag_Debug)	OutputDebugString(L"Success RegisterDeviceNotification!\n");
	return true;
}
//======================================================================================================================
bool WINAPI UnRegister(void)
{
	if (g_hDevNotify)
	{
		BOOL bOk=UnregisterDeviceNotification(g_hDevNotify);
		if(bOk==FALSE){
			if (flag_Debug)	OutputDebugString(L"release_USB Fault!\n");
			return false;
		}else{
			if (flag_Debug)	OutputDebugString(L"release_USB Success!\n");
			g_hDevNotify = NULL;
			return true;
		}
	}	
	return true;
}

//======================================================================================================================
int WINAPI EnumerateDevice()
{	
	DEVICE_INFO m_DeviceInfo[256];

	HDEVINFO hInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_USB, NULL, NULL, DIGCF_PRESENT);

	SP_DEVINFO_DATA data;
	data.cbSize = sizeof(SP_DEVINFO_DATA);

	int iDeviceCount = 0;
	int count = 0;
	BOOL ret = false;
	bool bEndFlag = false;



	//	char DevDesc[128],DevHWID[512],DevDriver[512],DevMFG[128];
	char DevCompID[512];
	while(!bEndFlag)
	{
		ret = SetupDiEnumDeviceInfo(hInfo, count, &data);
		if(!ret)
		{
			bEndFlag = true;
			break;
		}

		char PartentID[256], PartentID1[256];
		memset(PartentID,0,256);
		memset(PartentID1,0,256);

		CM_Get_Device_IDA(data.DevInst,PartentID1,256,0);
		//OutputDebugStringA(PartentID1);

		DEVINST hPartInst;
		CM_Get_Child(&hPartInst, data.DevInst,0);
		CM_Get_Device_IDA(hPartInst, PartentID,256,0);
		//OutputDebugStringA(PartentID);

		memset(DevCompID,0,256);
		//SetupDiGetDeviceRegistryPropertyA(hInfo,&data,SPDRP_HARDWAREID,NULL,(PBYTE)DevHWID,512,NULL);
		SetupDiGetDeviceRegistryPropertyA(hInfo, &data, SPDRP_COMPATIBLEIDS, NULL, (PBYTE)DevCompID, 512, NULL);

		//OutputDebugStringA(DevHWID);

		bool bFound = false;
		char *szIsFound = strstr(DevCompID,"Class_08");

		if(szIsFound)
		{

			MultiByteToWideChar( CP_ACP, 0, PartentID1,
				strlen(PartentID1)+1, m_DeviceInfo[iDeviceCount].HardwareID,
				sizeof(PartentID1)/sizeof(PartentID1[0]));
			MultiByteToWideChar( CP_ACP, 0, DevCompID,
				strlen(DevCompID)+1, m_DeviceInfo[iDeviceCount].CompatiableID,
				sizeof(DevCompID)/sizeof(DevCompID[0]));

			MultiByteToWideChar( CP_ACP, 0, PartentID,
				strlen(PartentID)+1, m_DeviceInfo[iDeviceCount].PNPDeviceID,
				sizeof(PartentID)/sizeof(PartentID[0]));

			wchar_t wstmp[256];
			wsprintf(wstmp, L"[%d] HWID = %s\n     Comp ID = %s\n     PNP ID = %s\n\n"
				,iDeviceCount
				,m_DeviceInfo[iDeviceCount].HardwareID
				,m_DeviceInfo[iDeviceCount].CompatiableID
				,m_DeviceInfo[iDeviceCount].PNPDeviceID);
			OutputDebugString(wstmp);

			iDeviceCount++;

		}
		count++;
	}

	return iDeviceCount;
}