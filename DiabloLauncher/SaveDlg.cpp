// SaveDlg.cpp : implementation file
//

#include "pch.h"
#include "DiabloLauncher.h"
#include "afxdialogex.h"
#include "SaveDlg.h"
#include "resource.h"


// SaveDlg dialog

IMPLEMENT_DYNAMIC(SaveDlg, CDialogEx)

//SaveDlg::SaveDlg(CWnd* pParent /*=nullptr*/)
//	: CDialogEx(IDD_SAVE, pParent)
//{
//
//}

SaveDlg::SaveDlg(const Character* pc, BOOL restore, CWnd* pParent)
	: CDialogEx(IDD_SAVE, pParent), character(*pc), restore(restore)
{
}


SaveDlg::~SaveDlg()
{
}

void SaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, states);
	DDX_Control(pDX, IDC_CHECK1, recycle);
}


BEGIN_MESSAGE_MAP(SaveDlg, CDialogEx)
END_MESSAGE_MAP()


// SaveDlg message handlers


BOOL SaveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	states.SetExtendedStyle(states.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	states.InsertColumn(0, _T("File name"), 0, 110);
	states.InsertColumn(1, _T("Save Time"), 0, 120);

	int index = 0;
	for (const State& state : character.saves) {
		CString text;
		text.Format(_T("%s%d"), (LPCTSTR) state.getName(), state.getLev());
		states.InsertItem(index, text);
		SYSTEMTIME utctime, time;
		FileTimeToSystemTime(&state.getTime(), &utctime);
		SystemTimeToTzSpecificLocalTime(NULL, &utctime, &time);
		text.FormatMessage(IDS_TIME_FMT, time.wYear, time.wMonth, time.wDay,
			time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
		states.SetItem(index, 1, LVIF_TEXT,
			const_cast<LPWSTR>((LPCWSTR)text), 0, 0, 0, NULL);
		index++;
	}
	
	if (restore) {
		this->SetWindowText(_T("Restore"));
		states.EnableWindow();
		recycle.ShowWindow(SW_HIDE);
		if (index > 0) {
			states.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
			states.SetSelectionMark(0);
		}
	}
	else if (index == 0) {
		recycle.ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
