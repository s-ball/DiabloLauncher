
// DiabloLauncher.h : main header file for the DiabloLauncher application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "CDiablo.h"


// CDiabloLauncherApp:
// See DiabloLauncher.cpp for the implementation of this class
//

class CDiabloLauncherApp : public CWinApp
{
	CDiablo diablo;

#ifdef IN_TEST
	friend class TestRelay;
#endif // IN_TEST

public:
	CDiabloLauncherApp() noexcept;

	const CDiablo& getDiablo() {
		return diablo;
	}
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDiabloLauncherApp theApp;
