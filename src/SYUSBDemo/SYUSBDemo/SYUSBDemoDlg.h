// SYUSBDemoDlg.h : header file
//

#pragma once


// CSYUSBDemoDlg dialog
class CSYUSBDemoDlg : public CDialog
{
// Construction
public:
	CSYUSBDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SYUSBDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg BOOL OnDeviceChange(UINT nEventType,DWORD_PTR dwData);
};
