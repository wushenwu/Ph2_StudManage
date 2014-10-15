// HelpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "HelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpDlg dialog


CHelpDlg::CHelpDlg(CTblInfoView *pTblInfoView,
                   CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHelpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    m_pTblInfoView = pTblInfoView;
    ASSERT(m_pTblInfoView != NULL);
}


void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialog)
	//{{AFX_MSG_MAP(CHelpDlg)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_MODIFY, OnBtnModify)
	ON_BN_CLICKED(IDC_BTN_SUBMIT, OnBtnSubmit)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpDlg message handlers

void CHelpDlg::OnBtnSearch() 
{
    m_pTblInfoView->OnSearch();	
}

void CHelpDlg::OnBtnAdd() 
{
    m_pTblInfoView->OnAdd();	
}

void CHelpDlg::OnBtnDel() 
{
    m_pTblInfoView->OnDel();	
}

void CHelpDlg::OnBtnModify() 
{
    m_pTblInfoView->OnModify();
}

void CHelpDlg::OnBtnSubmit() 
{
    m_pTblInfoView->OnSubmit();
}

void CHelpDlg::OnBtnCancel() 
{
    m_pTblInfoView->OnCancel();	
}
