
// klk2018.h : main header file for the klk2018 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Cklk2018App:
// See klk2018.cpp for the implementation of this class
//

class Cklk2018App : public CWinApp
{
public:
	Cklk2018App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cklk2018App theApp;
