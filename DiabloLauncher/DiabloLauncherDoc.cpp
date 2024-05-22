
// DiabloLauncherDoc.cpp : implementation of the CDiabloLauncherDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DiabloLauncher.h"
#endif

#include "DiabloLauncherDoc.h"

#include <propkey.h>

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDiabloLauncherDoc

IMPLEMENT_DYNCREATE(CDiabloLauncherDoc, CDocument)

BEGIN_MESSAGE_MAP(CDiabloLauncherDoc, CDocument)
END_MESSAGE_MAP()


// CDiabloLauncherDoc construction/destruction

CDiabloLauncherDoc::CDiabloLauncherDoc() noexcept
{
	// TODO: add one-time construction code here
}

CDiabloLauncherDoc::~CDiabloLauncherDoc()
{
}

BOOL CDiabloLauncherDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	reload();
	return TRUE;
}

void CDiabloLauncherDoc::reload() {
	mgr.load();
	items = mgr.getItems();
}


// CDiabloLauncherDoc serialization

void CDiabloLauncherDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CDiabloLauncherDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CDiabloLauncherDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CDiabloLauncherDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDiabloLauncherDoc diagnostics

#ifdef _DEBUG
void CDiabloLauncherDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDiabloLauncherDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDiabloLauncherDoc commands
