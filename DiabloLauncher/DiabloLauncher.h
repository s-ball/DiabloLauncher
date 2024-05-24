
// DiabloLauncher.h : main header file for the DiabloLauncher application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "CDiablo.h"
#include "VersionReader.h"


// CDiabloLauncherApp:
// See DiabloLauncher.cpp for the implementation of this class
//
class CDiabloLauncherApp : public CWinApp
{
	CDiablo diablo;
	VersionReader vr;

#ifdef IN_TEST
	friend class TestRelay;
#endif // IN_TEST

public:
	CDiabloLauncherApp() noexcept;

	const CDiablo& getDiablo() {
		return diablo;
	}
	VersionReader& getVersionReader() {
		return vr;
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
