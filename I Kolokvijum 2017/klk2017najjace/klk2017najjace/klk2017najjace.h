
// klk2017najjace.h : main header file for the klk2017najjace application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Cklk2017najjaceApp:
// See klk2017najjace.cpp for the implementation of this class
//

class Cklk2017najjaceApp : public CWinApp
{
public:
	Cklk2017najjaceApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cklk2017najjaceApp theApp;
