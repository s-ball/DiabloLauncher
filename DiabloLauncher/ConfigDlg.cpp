// ConfigDlg.cpp : implementation file
//

#include "pch.h"
#include "DiabloLauncher.h"
#include "afxdialogex.h"
#include "ConfigDlg.h"


// ConfigDlg dialog

IMPLEMENT_DYNAMIC(ConfigDlg, CDialogEx)

ConfigDlg::ConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONFIG, pParent)
{

}

ConfigDlg::~ConfigDlg()
{
}

void ConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAX, max);
}


BEGIN_MESSAGE_MAP(ConfigDlg, CDialogEx)
END_MESSAGE_MAP()


// ConfigDlg message handlers


BOOL ConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
