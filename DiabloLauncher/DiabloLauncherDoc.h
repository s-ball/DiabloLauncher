#include "Cd2sManager.h"
// DiabloLauncherDoc.h : interface of the CDiabloLauncherDoc class
//


#pragma once


class CDiabloLauncherDoc : public CDocument
{
protected: // create from serialization only
	CDiabloLauncherDoc() noexcept;
	DECLARE_DYNCREATE(CDiabloLauncherDoc)

// Attributes
public:
	Cd2sManager mgr;
	std::vector<const Character*> items;

// Operations
public:
	void reload();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CDiabloLauncherDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
