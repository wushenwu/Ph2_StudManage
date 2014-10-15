// TblInfoView.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "TblInfoView.h"
#include "MainFrm.h"

#include "HelpDlg.h"
#include "Request.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PAD   "*"
#define SEP   "|"

/////////////////////////////////////////////////////////////////////////////
// CTblInfoView

IMPLEMENT_DYNCREATE(CTblInfoView, CListView)

CTblInfoView::CTblInfoView()
{
    m_strCurTbl  = "";
    m_Operation  = OP_NONE;
    m_pEdit      = NULL;

    m_nItem      = -1;
    m_nSubItem   = -1;
}

CTblInfoView::~CTblInfoView()
{
}

BEGIN_MESSAGE_MAP(CTblInfoView, CListView)
	//{{AFX_MSG_MAP(CTblInfoView)
	ON_COMMAND(IDM_OPERATE, OnOperate)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
    ON_MESSAGE(MSG_EDIT_END, OnMSGEditEnd)
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
                    | LVS_REPORT | LVS_SHAREIMAGELISTS | LVS_ALIGNLEFT 
                    | LVS_NOSORTHEADER 
                    | LVS_EX_FULLROWSELECT
                  );

    GetListCtrl().SetExtendedStyle(WS_EX_LEFT|WS_EX_RIGHTSCROLLBAR|WS_EX_CLIENTEDGE
                                |LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES
                                );
	
}

/************************************************************************/
/* TreeView中选择表名， ListView中展示相关信息                         */
/************************************************************************/
void CTblInfoView::UpdateListInfo(CString strTblName)
{
    if (m_Operation != OP_NONE)
    {
        AfxMessageBox("请先提交操作");
        return;
    }

    //just need to set the header.
    static CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
    static CListCtrl &ctrl        = GetListCtrl();
    CHeaderCtrl *pHeader          = ctrl.GetHeaderCtrl(); 
    CNetWork *pNet                = &pMainFrame->m_Nets;
    
    int nCount = pHeader->GetItemCount();
    int i;
    for (i = 0; i < nCount; i++)
    {
        //pHeader->DeleteItem(0);   bug, should do like this
        ctrl.DeleteColumn(0);
    }

    ctrl.DeleteAllItems();

    //m_map_tbl_cols[strTblName];
    CString strOri  = pNet->GetOriName(strTblName);
    CString strCols = pNet->GetColsForTbl(strOri);
    CString strSub  = _T("");
    for (i = 0; ; i++)
    {
        if (!AfxExtractSubString(strSub, strCols, i, '|')
            || 0 == strSub.GetLength())
        {
            break;
        }
        
        //we use alias
        ctrl.InsertColumn(i, pNet->GetAlias(strSub), LVCFMT_LEFT, 70);
    }

    m_strCurTbl = strOri;
}

/************************************************************************/
/* 对“操作”菜单的响应，弹出辅助操作的非模态对话框                    */
/************************************************************************/
void CTblInfoView::OnOperate() 
{
    CHelpDlg *pdlg = new CHelpDlg(this);
    if (NULL == pdlg)
    {
        return;
    }
    
    BOOL bRet = pdlg->Create(IDD_DLG_HELP,this);
    if (!bRet)
    {
        delete pdlg;
        return;
    }
    
    pdlg->ShowWindow(SW_SHOW);	   
}

/************************************************************************/
/* 设置相关操作类型，以及ListView的有关提示                                                                     */
/************************************************************************/
void CTblInfoView::OnSearch()
{
    m_Operation = INFO_TYPE_SELECT;

    SetHitInfo();
}

/************************************************************************/
/* 双击时，在ListView中绘制EditView, 以模拟DataGrid                     */
/************************************************************************/
void CTblInfoView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if (OP_NONE == m_Operation)
    {
        return;
    }

    NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
    if (-1 == pNMListView->iItem)
    {
        //no item selected
        return;
    }

    static CListCtrl &ctrl = GetListCtrl();
    ctrl.SetRedraw(FALSE);
  
    int nItem       = pNMListView->iItem;
    int nSubItem    = pNMListView->iSubItem;
    m_nItem         = nItem;
    m_nSubItem      = nSubItem;

    //check which one cannot be edited (like PK, FK), determined by        the current tables.
    //TDB
    //we just add id as the 1st column
    if (0 == nSubItem)
    {
        return;
    }

    //对双击的row, col重绘edit,
    if (m_pEdit != NULL)
    {
        delete m_pEdit;
        m_pEdit = NULL;
    }

    CInfoEdit *pEdit = new CInfoEdit();
    if (NULL == pEdit)
    {
        return;
    }

    //make sure to clean up
    m_pEdit = pEdit;

    pEdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN, 
                CRect(0, 0, 0, 0), 
                this, 
                1);	
    
    //获取相应范围
    CRect rc;
    ctrl.GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rc);

    CString strItem = ctrl.GetItemText(nItem, nSubItem);
    pEdit->MoveWindow(&rc);	
    pEdit->ShowWindow(SW_SHOW);
    pEdit->SetWindowText(strItem);
    pEdit->SetFocus();
    pEdit->SetSel(0, -1);   //select all the text
       
	ctrl.SetRedraw(TRUE);
	
	*pResult = 0;
}

void CTblInfoView::OnAdd()
{
    m_Operation = INFO_TYPE_INSERT;

    SetHitInfo();
}

void CTblInfoView::OnDel()
{
    static CListCtrl &ctrl = GetListCtrl();
    int nItem = ctrl.GetSelectionMark();
    if (-1 == nItem)
    {
        AfxMessageBox("请选中行操作");
        return;
    }

    m_Operation = INFO_TYPE_DEL;

    ctrl.DeleteItem(nItem);

    OnSubmit();      
}

void CTblInfoView::OnModify()
{
    static CListCtrl &ctrl = GetListCtrl();
    int nItem = ctrl.GetSelectionMark();
    if (-1 == nItem)
    {
        AfxMessageBox("请选中行操作，然后提交");
        return;
    }

    m_Operation = INFO_TYPE_UPDATE; 
}

void CTblInfoView::OnCancel()
{
    m_Operation = OP_NONE;
}

void CTblInfoView::OnSubmit()
{
    if (OP_NONE == m_Operation)
    {
        AfxMessageBox("尚未选择有关操作");
        return;
    }

    static CMainFrame *pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
    static CNetWork *pNet         = &pMainFrame->m_Nets;

    //获取相关字段
    static CListCtrl &ctrl = GetListCtrl();
    int nItem = ctrl.GetSelectionMark();
    if (-1 == nItem)
    {
        return;
    }

    //获取头部相关信息
    CHeaderCtrl *pHeader = ctrl.GetHeaderCtrl(); 
    int nCount = pHeader->GetItemCount();

    TCHAR  szBuffer[MAXBYTE];
    HDITEM hdi;
    hdi.mask        = HDI_TEXT;
    hdi.pszText     = szBuffer;
    hdi.cchTextMax  = MAXBYTE;

    CString strSub; 
    CString strCol;
    CString strInfo = "";
    int i = 0;
    int j = 0;
    for (i = 0; i < nCount; i++)
    {
        strSub = ctrl.GetItemText(nItem, i);
        if (0 == strSub.Compare(PAD)
            || 0 == strSub.GetLength())
        {
            continue;
        }

        j++;    //非空字段

        pHeader->GetItem(i, &hdi);
        strCol = hdi.pszText;

        //alias process
        strInfo += pNet->GetOriName(strCol);
        strInfo += SEP;
        strInfo += strSub;
        strInfo += SEP;
    }

    //check
    if ((INFO_TYPE_INSERT == m_Operation
            || INFO_TYPE_UPDATE == m_Operation
            )
        && j < nCount - 1)
    {
        AfxMessageBox("字段不完整，无法操作");
        return;
    }

    //将字段、数据、动作信息发给M Server
    CRequest *pReq = new CRequest(m_Operation, m_strCurTbl, strInfo);
    if (pReq != NULL)
    {
        pNet->SendRequest(pReq);
    }

    m_Operation = OP_SUBMITTING;
}

/************************************************************************/
/* 处理ListView中自绘的EditView                                                                     */
/************************************************************************/
void CTblInfoView::OnMSGEditEnd(WPARAM wParam,LPARAM lParam)
{
    CString strSub;
    m_pEdit->GetWindowText(strSub);
    
    static CListCtrl &ctrl = GetListCtrl();
    ctrl.SetItemText(m_nItem, m_nSubItem, strSub);
}

/************************************************************************/
/* 将查询所得结果展示到ListView中                                                                     */
/************************************************************************/
void CTblInfoView::SetSelResults(const char *pszRet)
{    
    //make sure m_strCurTbl  matches the results.
    //so, OP_SUBMITTING is added
    static CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
    static CListCtrl &ctrl = GetListCtrl();
    CHeaderCtrl *pHeader = ctrl.GetHeaderCtrl(); 
    int nCount = pHeader->GetItemCount();

    ctrl.DeleteAllItems();
    
    CString strCols = pszRet;
    CString strSub = _T("");

    int nItem    = 0;
    int nSubItem = 0;
    int nIndex   = 0;

    LVITEM lv;
    lv.mask  = LVIF_TEXT;

    for (int i = 0; ; i++)
    {
        if (!AfxExtractSubString(strSub, strCols, i, '|')
            || 0 == strSub.GetLength())
        {
            break;
        }

        nItem       = i / nCount;
        nSubItem    = i % nCount;

        lv.iItem    = nIndex;//0; //nItem;
        lv.iSubItem = nSubItem;
        lv.pszText  = strSub.GetBuffer(0);

        if (0 == nSubItem)
        {
            nIndex = ctrl.InsertItem(&lv);
        }
        else
        {
            ctrl.SetItem(&lv);
        }
    }   

    m_Operation = OP_NONE;
}

void CTblInfoView::SetHitInfo()
{
    CListCtrl &ctrl = GetListCtrl();
    ctrl.DeleteAllItems();
    
    CHeaderCtrl *pHeader = ctrl.GetHeaderCtrl(); 
    int nCount = pHeader->GetItemCount();
    
    LVITEM lv;
    lv.mask  = LVIF_TEXT;
    lv.iItem = 0;
    for (int i = 0; i < 1/*nCount*/; i++)
    {
        lv.iSubItem = i;
        lv.pszText = PAD;
        
        if (0 == i)
        {
            ctrl.InsertItem(&lv);
        }
        else
        {
            ctrl.SetItem(&lv);
        }
    }
}
