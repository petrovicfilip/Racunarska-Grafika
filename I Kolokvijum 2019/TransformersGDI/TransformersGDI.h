
// TransformersGDI.h : main header file for the TransformersGDI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTransformersGDIApp:
// See TransformersGDI.cpp for the implementation of this class
//

class CTransformersGDIApp : public CWinApp
{
public:
	CTransformersGDIApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTransformersGDIApp theApp;
