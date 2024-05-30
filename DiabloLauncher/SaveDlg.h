// Copyright 2024 s-ball - MIT License

#pragma once
#include "afxdialogex.h"
#include "Cd2sManager.h"

// SaveDlg dialog

class SaveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SaveDlg)

	const Character& character;
	BOOL restore;
	CButton recycleButton;
public:
//	SaveDlg(CWnd* pParent = nullptr);   // standard constructor
	SaveDlg(const Character* pc, BOOL restore = FALSE, CWnd* pParent = nullptr);   // standard constructor
	virtual ~SaveDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl states;

	bool recycle;
	int selectedIndex;
	virtual void OnOK();
};
