
// TransformersDrugiPut.h : main header file for the TransformersDrugiPut application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTransformersDrugiPutApp:
// See TransformersDrugiPut.cpp for the implementation of this class
//

class CTransformersDrugiPutApp : public CWinApp
{
public:
	CTransformersDrugiPutApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTransformersDrugiPutApp theApp;
