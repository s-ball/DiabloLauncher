
// DiabloLauncherView.h : interface of the CDiabloLauncherView class
//

#pragma once


class CDiabloLauncherView : public CFormView
{
protected: // create from serialization only
	CDiabloLauncherView() noexcept;
	DECLARE_DYNCREATE(CDiabloLauncherView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_DIABLOLAUNCHER_FORM };
#endif

// Attributes
public:
	CDiabloLauncherDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CDiabloLauncherView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl characters;
	afx_msg void OnClickedLaunch();
	afx_msg void OnClickedRestore();
	afx_msg void OnClickedSave();
	afx_msg void OnItemchangedListChars(NMHDR* pNMHDR, LRESULT* pResult);
	CButton restoreBtn;
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // debug version in DiabloLauncherView.cpp
inline CDiabloLauncherDoc* CDiabloLauncherView::GetDocument() const
   { return reinterpret_cast<CDiabloLauncherDoc*>(m_pDocument); }
#endif

