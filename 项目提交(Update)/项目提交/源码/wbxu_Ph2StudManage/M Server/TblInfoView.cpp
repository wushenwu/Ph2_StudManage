// TblInfoView.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "TblInfoView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTblInfoView

IMPLEMENT_DYNCREATE(CTblInfoView, CListView)

CTblInfoView::CTblInfoView()
{
}

CTblInfoView::~CTblInfoView()
{
}


BEGIN_MESSAGE_MAP(CTblInfoView, CListView)
	//{{AFX_MSG_MAP(CTblInfoView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTblInfoView drawing

void CTblInfoView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTblInfoView diagnostics

#ifdef _DEBUG
void CTblInfoView::AssertValid() const
{
	CListView::AssertValid();
}

void CTblInfoView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTblInfoView message handlers

void CTblInfoView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
    //设置TreeView Style
    LONG lOld = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
    SetWindowLong(GetSafeHwnd(),
                    GWL_STYLE, 
                    lOld 
                    | WS_CHILDWINDOW| WS_VISIBLE | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL |WS_TABSTOP 
                    | LVS_REPORT| LVS_EDITLABELS | LVS_SHAREIMAGELISTS | LVS_ALIGNLEFT 
                    | LVS_NOSORTHEADER
                    );
	
}

/************************************************************************/
/* used for testing                                                     */
/************************************************************************/
void CTblInfoView::UpdateListInfo(CString strTblName)
{
#if 0
    static CListCtrl &ctrl = GetListCtrl();
    static CHeaderCtrl *pHeader = ctrl.GetHeaderCtrl(); 
    
//     int nCount = pHeader->GetItemCount();
//     for (int i = 0; i < nCount; i++)
//     {
//     }

    CString sql = _T("select * from " + strTblName);
    BOOL bRet = TRUE;

    static CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
    CADORecordset *pRSet = (reinterpret_cast<CServerApp*>AfxGetApp()).m_DataBase.m_pRSet;
    bRet = pRSet->Open(sql, CADORecordset::openUnknown);
    if (!bRet)
    {
        return;
    }

    int nCount = pRSet->GetFieldCount();
    CADOFieldInfo fieldInfo;
    CString str;
    for (int i = 0; i < nCount; i++)
    {
        pRSet->GetFieldInfo(i, &fieldInfo);
        str += fieldInfo.m_strName;
        str += " | ";
    }

    CString strCol;
    while (!pRSet->IsEof())
    {
        for (i = 0; i < nCount; i++)
        {
            pRSet->GetFieldValue(i, strCol);
            str += strCol;
            str += " | ";
        }
        
        pRSet->MoveNext();
    }



    pMainFrame->LogInfo(sql);
    pMainFrame->LogInfo(str);

    sql = "insert t_class values('cls012', '12级信息安全')";
    pRSet->Open(sql, CADORecordset::openUnknown);
    pMainFrame->LogInfo(sql);
#endif
}
