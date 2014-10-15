#if !defined(AFX_TBLINFOVIEW_H__926255B7_60F7_4C52_AD62_BE096846616E__INCLUDED_)
#define AFX_TBLINFOVIEW_H__926255B7_60F7_4C52_AD62_BE096846616E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TblInfoView.h : header file
//
#include <afxcview.h>

/////////////////////////////////////////////////////////////////////////////
// CTblInfoView view

class CTblInfoView : public CListView
{
protected:
	CTblInfoView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTblInfoView)

// Attributes
public:

// Operations
public:
	void UpdateListInfo(CString strTblName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTblInfoView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTblInfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTblInfoView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TBLINFOVIEW_H__926255B7_60F7_4C52_AD62_BE096846616E__INCLUDED_)
