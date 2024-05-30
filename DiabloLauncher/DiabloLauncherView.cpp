// Copyright 2024 s-ball - MIT License

// DiabloLauncherView.cpp : implementation of the CDiabloLauncherView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DiabloLauncher.h"
#endif

#include "DiabloLauncherDoc.h"
#include "DiabloLauncherView.h"
#include "StartAndWait.h"
#include "SaveDlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDiabloLauncherView

IMPLEMENT_DYNCREATE(CDiabloLauncherView, CFormView)

BEGIN_MESSAGE_MAP(CDiabloLauncherView, CFormView)
	ON_BN_CLICKED(IDC_LAUNCH, &CDiabloLauncherView::OnClickedLaunch)
	ON_BN_CLICKED(IDC_RESTORE, &CDiabloLauncherView::OnClickedRestore)
	ON_BN_CLICKED(IDC_SAVE, &CDiabloLauncherView::OnClickedSave)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CHARS, &CDiabloLauncherView::OnItemchangedListChars)
END_MESSAGE_MAP()

// CDiabloLauncherView construction/destruction

CDiabloLauncherView::CDiabloLauncherView() noexcept
	: CFormView(IDD_DIABLOLAUNCHER_FORM)
{
	// TODO: add construction code here
}

CDiabloLauncherView::~CDiabloLauncherView()
{
}

void CDiabloLauncherView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHARS, characters);
	DDX_Control(pDX, IDC_RESTORE, restoreBtn);
}

BOOL CDiabloLauncherView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CDiabloLauncherView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	characters.SetExtendedStyle(characters.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	characters.InsertColumn(0, RsrcString(IDS_NAME), 0, 110);
	characters.InsertColumn(1, RsrcString(IDS_SAVETIME), 0, 120);
	characters.InsertColumn(2, RsrcString(IDS_SAVEDVERSIONS), 0, 100);

	OnUpdate(NULL, 1, NULL);
	//GetDlgItem(IDC_LIST_CHARS)->SetFocus();
}


// CDiabloLauncherView diagnostics

#ifdef _DEBUG
void CDiabloLauncherView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDiabloLauncherView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDiabloLauncherDoc* CDiabloLauncherView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDiabloLauncherDoc)));
	return (CDiabloLauncherDoc*)m_pDocument;
}
#endif //_DEBUG


// CDiabloLauncherView message handlers


void CDiabloLauncherView::OnClickedLaunch()
{
	// TODO: Add your control notification handler code here
	theApp.GetMainWnd()->ShowWindow(SW_HIDE);
	WORD cr = (WORD)startAndWait(theApp.getDiablo().getGame());
	theApp.GetMainWnd()->ShowWindow(SW_RESTORE);
	if (cr != 0) {
		AfxMessageBox(IDS_LAUNCHERROR, MB_ICONERROR);
	}
	else {
		GetDocument()->reload();
		OnUpdate(NULL, 1, NULL);
	}
}


void CDiabloLauncherView::OnClickedRestore()
{
	// TODO: Add your control notification handler code here
	int selected = characters.GetSelectionMark();
	const Character* character = GetDocument()->items[selected];
	SaveDlg saveDlg(character, TRUE);
	INT_PTR cr = saveDlg.DoModal();
	switch (cr) {
	case -1:
	case IDABORT:
		AfxMessageBox(RsrcString(IDS_SYSTEMERROR), MB_ICONERROR);
		break;
	case IDCANCEL:
		return;
	}
	if (saveDlg.selectedIndex == -1) {
		MessageBox(RsrcString(IDS_NOITEM), RsrcString(IDS_RESTOREERROR), MB_OK | MB_ICONERROR);
		return;
	}
	Character& ch = GetDocument()->mgr.characters[character->cur.getName()];
	const State& state = ch.saves[saveDlg.selectedIndex];
	ch.restore(state);
	GetDocument()->reload();
	OnUpdate(NULL, 1, NULL);
}


void CDiabloLauncherView::OnClickedSave()
{
	// TODO: Add your control notification handler code here
	int selected = characters.GetSelectionMark();
	const Character* character = GetDocument()->items[selected];
	SaveDlg saveDlg(character);
	INT_PTR cr = saveDlg.DoModal();
	switch (cr) {
	case -1:
	case IDABORT:
		AfxMessageBox(RsrcString(IDS_SYSTEMERROR), MB_ICONERROR);
		break;
	case IDCANCEL:
		return;
	}
	Character& ch = GetDocument()->mgr.characters[character->cur.getName()];
	ch.save(saveDlg.recycle);
	GetDocument()->reload();
	OnUpdate(NULL, 1, NULL);
}


void CDiabloLauncherView::OnItemchangedListChars(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED)) {
		int selected = pNMLV->iItem;
		CDiabloLauncherDoc* pDoc = GetDocument();
		if (selected >= 0 && selected < pDoc->items.size()) {
			const Character* character = pDoc->items[selected];
			restoreBtn.EnableWindow(character->saves.size() > 0);
		} 
	}
	*pResult = 0;
}


void CDiabloLauncherView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: Add your specialized code here and/or call the base class
	if (lHint != 0) {
		characters.DeleteAllItems();
		int index = 0;
		for (const Character*& c : GetDocument()->items) {
			characters.InsertItem(index, const_cast<LPWSTR>((LPCWSTR)c->cur.getName()));
			CString text;
			SYSTEMTIME utctime, time;
			FileTimeToSystemTime(&c->cur.getTime(), &utctime);
			SystemTimeToTzSpecificLocalTime(NULL, &utctime, &time);
			text.FormatMessage(IDS_TIME_FMT, time.wYear, time.wMonth, time.wDay,
				time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
			characters.SetItem(index, 1, LVIF_TEXT,
				const_cast<LPWSTR>((LPCWSTR)text), 0, 0, 0, NULL);
			text.Format(_T("%d"), (int)c->saves.size());
			characters.SetItem(index, 2, LVIF_TEXT,
				const_cast<LPWSTR>((LPCWSTR)text), 0, 0, 0, NULL);
			index++;
		}
		if (index > 0) {
			characters.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
			characters.SetSelectionMark(0);
		}
	}
}