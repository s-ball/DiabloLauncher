// Copyright 2024 s-ball - MIT License

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
	static const UINT ID_LANG = 0x400;

	CDiablo diablo;
	WORD langId;
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
	UINT FirstLangId() {
		return ID_LANG;
	}
	UINT LastLangId() {
		return ID_LANG + vr.getInfo().nblangs - 1;
	}
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	int AddLangMenu();
	afx_msg void OnLangChange(UINT nid);
};

extern CDiabloLauncherApp theApp;

CString RsrcString(UINT id);