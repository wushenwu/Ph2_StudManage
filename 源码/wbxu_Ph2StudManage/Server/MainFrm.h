// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D799F87F_D1F3_432A_B66F_B35560944235__INCLUDED_)
#define AFX_MAINFRM_H__D799F87F_D1F3_432A_B66F_B35560944235__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetWork.h"

class CMainFrame : public CFrameWnd
{	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
    CDataBase m_DataBase;

private:
    UINT m_nMenuID;         //indicate the running state
    CNetWork m_Nets;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

public:
	void LogInfo(CString strLog);
    //关于视图分割
    CSplitterWnd m_SplitterWnd1;  
    CSplitterWnd m_SplitterWnd2;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnStartService();
	afx_msg void OnMenuRefresh(CCmdUI* pCmdUI);
	afx_msg void OnStopService();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D799F87F_D1F3_432A_B66F_B35560944235__INCLUDED_)
