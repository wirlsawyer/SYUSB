#include "stdafx.h"
#include "SYUSBManger.h"

SYUSBManger* SYUSBManger::m_instance = NULL;

SYUSBManger* SYUSBManger::Instance()
{
	if (m_instance == NULL){
		m_instance = new SYUSBManger(); 
	}
	return m_instance;
}

SYUSBManger::SYUSBManger()
{
	m_hModule					= NULL;
	m_DLLFuncTestAdd			= NULL;
	m_DLLFuncRegisterWithHwnd	= NULL;
	m_DLLFuncUnRegister			= NULL;

	//-------------------------------------------------------------------------------
	m_hModule = LoadLibrary(L".\\SYUSB.dll");
	//-------------------------------------------------------------------------------
	if ( m_hModule != NULL )
	{
		//-------------------------------------------------------------------------------
		m_DLLFuncTestAdd = (SYUSB_TestAdd)GetProcAddress( m_hModule, "TestAdd" );
		if (m_DLLFuncTestAdd == NULL)
		{
			OutputDebugString(L"GetProc fault:TestAdd\n");
			::MessageBox(NULL, L"GetProc fault:TestAdd", L"", MB_OK);
			FreeLibrary(m_hModule);
			return;
		}

		//-------------------------------------------------------------------------------
		m_DLLFuncRegisterWithHwnd = (SYUSB_RegisterWithHwnd)GetProcAddress( m_hModule, "RegisterWithHwnd" );
		if (m_DLLFuncRegisterWithHwnd == NULL)
		{
			OutputDebugString(L"GetProc fault:RegisterWithHwnd\n");
			::MessageBox(NULL, L"GetProc fault:RegisterWithHwnd", L"", MB_OK);
			FreeLibrary(m_hModule);
			return;
		}

		//-------------------------------------------------------------------------------
		m_DLLFuncUnRegister = (SYUSB_UnRegister)GetProcAddress( m_hModule, "UnRegister" );
		if (m_DLLFuncUnRegister == NULL)
		{
			OutputDebugString(L"GetProc fault:UnRegister\n");
			::MessageBox(NULL, L"GetProc fault:UnRegister", L"", MB_OK);
			FreeLibrary(m_hModule);
			return;
		}

		


	}else{
		OutputDebugString(L"SYUSBManger fault:Can't find SYUSB.dll\n");
	}
}

SYUSBManger::~SYUSBManger()
{
	FreeLibrary(m_hModule);
}


int SYUSBManger::TestAdd(int a, int b)
{
	return m_DLLFuncTestAdd(a, b);
}

bool SYUSBManger::RegisterWithHwnd(HWND hwnd)
{
	/*
	case WM_DEVICECHANGE:		


	switch(wParam)
	{
	// 當光碟片放進來時,就會啟動
	case DBT_DEVICEARRIVAL: 
	MessageBox(hWnd, L"A device has been inserted and is now available", L"Device", MB_OK);
	break;
	// 當光碟片被拿走
	case DBT_DEVICEREMOVECOMPLETE:
	MessageBox(hWnd, L"Device has been removed", L"Device", MB_OK);
	break;
	case DBT_DEVICEQUERYREMOVE:
	MessageBox(hWnd, L"Permission to remove a device is requested", L"Device", MB_OK);
	break;
	case DBT_DEVICEQUERYREMOVEFAILED:
	MessageBox(hWnd, L"Request to remove a device has been canceled", L"Device", MB_OK);
	break;
	case DBT_DEVICEREMOVEPENDING:
	MessageBox(hWnd, L"Device is about to be removed", L"Device", MB_OK);
	break;

	case DBT_DEVICETYPESPECIFIC :
	MessageBox(hWnd, L"Device-specific event", L"Device", MB_OK);
	break;
	default:
	//MessageBox(hWnd, L"其他訊息", L"", MB_OK);
	break;
	}
	*/
	return m_DLLFuncRegisterWithHwnd(hwnd);
}

bool SYUSBManger::UnRegister()
{
	return m_DLLFuncUnRegister();
}
