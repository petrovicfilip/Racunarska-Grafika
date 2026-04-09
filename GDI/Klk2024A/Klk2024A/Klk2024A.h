
// Klk2024A.h : main header file for the Klk2024A application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2024AApp:
// See Klk2024A.cpp for the implementation of this class
//

class CKlk2024AApp : public CWinApp
{
public:
	CKlk2024AApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlk2024AApp theApp;
