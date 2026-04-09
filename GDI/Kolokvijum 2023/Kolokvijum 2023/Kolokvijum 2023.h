
// Kolokvijum 2023.h : main header file for the Kolokvijum 2023 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKolokvijum2023App:
// See Kolokvijum 2023.cpp for the implementation of this class
//

class CKolokvijum2023App : public CWinApp
{
public:
	CKolokvijum2023App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKolokvijum2023App theApp;
