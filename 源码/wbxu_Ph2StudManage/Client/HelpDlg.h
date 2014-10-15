#if !defined(AFX_HELPDLG_H__016D69D7_F7E1_41C1_929A_18FB0C66FE3C__INCLUDED_)
#define AFX_HELPDLG_H__016D69D7_F7E1_41C1_929A_18FB0C66FE3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HelpDlg.h : header file
//

#include "TblInfoView.h"

/////////////////////////////////////////////////////////////////////////////
// CHelpDlg dialog

/************************************************************************/
/* 辅助操作的非模态Dialog, 可用菜单替代                               */
/************************************************************************/
class CHelpDlg : public CDialog
{
// Construction
public:
	//CHelpDlg(CWnd* pParent = NULL);   // standard constructor
    CHelpDlg(CTblInfoView *pTblInfoView, CWnd* pParent = NULL);

private:
    CTblInfoView *m_pTblInfoView;

// Dialog Data
	//{{AFX_DATA(CHelpDlg)
	enum { IDD = IDD_DLG_HELP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHelpDlg)
	afx_msg void OnBtnSearch();
    afx_msg void OnBtnAdd();
    afx_msg void OnBtnDel();
    afx_msg void OnBtnModify();
    afx_msg void OnBtnSubmit();
	afx_msg void OnBtnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPDLG_H__016D69D7_F7E1_41C1_929A_18FB0C66FE3C__INCLUDED_)
