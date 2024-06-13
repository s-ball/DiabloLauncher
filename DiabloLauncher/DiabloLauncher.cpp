// Copyright 2024 s-ball - MIT License


// DiabloLauncher.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DiabloLauncher.h"
#include "MainFrm.h"

#include "DiabloLauncherDoc.h"
#include "DiabloLauncherView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDiabloLauncherApp

BEGIN_MESSAGE_MAP(CDiabloLauncherApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CDiabloLauncherApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// LANGUAGES
	ON_COMMAND_RANGE(ID_LANG, ID_LANG + 256, &OnLangChange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_LANG, ID_LANG + 256, &OnUILangChange)
END_MESSAGE_MAP()


// CDiabloLauncherApp construction

CDiabloLauncherApp::CDiabloLauncherApp() noexcept
{

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("SBA.DiabloLauncher"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CDiabloLauncherApp object

CDiabloLauncherApp theApp;


// CDiabloLauncherApp initialization

BOOL CDiabloLauncherApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("SBA"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	vr.init(NULL,
		GetProfileInt(_T("Settings"), _T("Language"), GetThreadUILanguage()));
	SetThreadUILanguage(vr.getLangId());

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDiabloLauncherDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDiabloLauncherView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Load Diablo II paths from registry
	if (diablo.init() != ERROR_SUCCESS) {
		AfxMessageBox(IDS_NOT_INSTALLED, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Post process menu to add language switching
	AddLangMenu();

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->SetWindowText(m_pszAppName);
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

int CDiabloLauncherApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CDiabloLauncherApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
	VersionReader& vr;
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	void setVersion();
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX), vr(theApp.getVersionReader())
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CDiabloLauncherApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDiabloLauncherApp message handlers


void CAboutDlg::setVersion()
{
	// TODO: Add your implementation code here.
	Info info = vr.getInfo(m_hWnd);
	GetDlgItem(IDC_VERSION)->SetWindowText(info.version);
	GetDlgItem(IDC_COPYRIGHT)->SetWindowText(info.copyright);
}


BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	setVersion();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CString RsrcString(UINT id) {
	CString str;
	str.LoadString(id);
	return str;
}


int CDiabloLauncherApp::AddLangMenu()
{
	// TODO: Ajoutez ici votre code d'implémentation..
	CMenu* menu = m_pMainWnd->GetMenu()->GetSubMenu(0);
	if (menu == NULL) return FALSE;
	CMenu lmenu;
	if (! lmenu.CreatePopupMenu()) return FALSE;
	const Info& info = vr.getInfo();
	for (unsigned int i = 0; i < info.nblangs; i++) {
		int sz = GetLocaleInfo(info.langs[2*i], LOCALE_SLOCALIZEDDISPLAYNAME, NULL, 0);
		if (sz == 0) continue;
		LPTSTR name = new TCHAR[sz];
		GetLocaleInfo(info.langs[2*i], LOCALE_SLOCALIZEDDISPLAYNAME, name, sz);
		lmenu.AppendMenu(MF_STRING, ID_LANG+i, name);
		delete[] name;
	}
	if (lmenu.GetMenuItemCount() != info.nblangs) return FALSE;
	if (menu->InsertMenu(0, MF_POPUP | MF_BYPOSITION,
		(UINT_PTR)lmenu.m_hMenu, _T("&Languages"))) {
		lmenu.Detach();
	}

	menu->InsertMenu(1, MF_SEPARATOR | MF_BYPOSITION);

	return TRUE;
}

void CDiabloLauncherApp::OnLangChange(UINT nid)
{
	const Info& info = vr.getInfo();
	UINT index = nid - ID_LANG;
	if (info.langs[2 * index] != info.lid) {
		
		if (IDYES == MessageBoxExW(m_pMainWnd->m_hWnd, RsrcString(IDS_WANTRESTART),
			m_pszAppName, MB_YESNO | MB_ICONQUESTION, info.lid)) {
			vr.SetLang(nid - ID_LANG);
			PROCESS_INFORMATION pi;
			STARTUPINFO si = { sizeof(si) };

			WriteProfileInt(_T("Settings"), _T("Language"), vr.getLangId());

			if (CreateProcess(vr.getExeName(), NULL, NULL, NULL, 0, 0, NULL, NULL, &si, &pi)) {
				m_pMainWnd->PostMessageW(WM_CLOSE);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}
			else {
				MessageBoxExW(m_pMainWnd->m_hWnd, RsrcString(IDS_ERRSTART),
					m_pszAppName, MB_OK | MB_ICONERROR, info.lid);
			}
		}
	}
}


void CDiabloLauncherApp::OnUILangChange(CCmdUI* pCmdUI)
{
	// TODO: Ajoutez ici votre code d'implémentation..
	UINT index = pCmdUI->m_nIndex;
	const Info& info = vr.getInfo();
	pCmdUI->SetCheck(info.langs[2 * index] == info.lid);
}