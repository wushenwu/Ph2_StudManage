// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Server.h"

#include "MainFrm.h"
#include "TblTreeView.h"
#include "TblInfoView.h"
#include "LogView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
static CEdit *gs_pEditLog = NULL;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(IDM_START, OnStartService)
	ON_UPDATE_COMMAND_UI_RANGE(IDM_START, IDM_STOP, OnMenuRefresh)
	ON_COMMAND(IDM_STOP, OnStopService)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{    
    m_nMenuID = 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

//#define USE_TOOLBAR
#ifdef USE_TOOLBAR
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
#endif

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

#ifdef USE_TOOLBAR
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
#endif

    //SetWindowText("Server");

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    // Method 2 to change title, then SetWindowText in OnCreate
    //cs.style &= ~FWS_ADDTOTITLE;  // Remove FWS_ADDTOTITLE from cs.style

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
#ifndef _DEBUG
    //Should Not return like this if u Create Your Client
	return CFrameWnd::OnCreateClient(lpcs, pContext);
#endif

    //划分成两行一列
    BOOL bRet = m_SplitterWnd1.CreateStatic(this, 2, 1);
    
    //将第1行进行再分割，分割成一个TreeView，和原来的画图视图
    bRet = m_SplitterWnd2.CreateStatic(&m_SplitterWnd1, 
                                        1, 2, 
                                        WS_CHILD | WS_VISIBLE, 
                                        m_SplitterWnd1.IdFromRowCol(0, 0)
                                        );
    
    //紧接着createview
    m_SplitterWnd2.CreateView(0, 0, RUNTIME_CLASS(CTblTreeView), CSize(0, 0), pContext);
    m_SplitterWnd2.CreateView(0, 1, RUNTIME_CLASS(CTblInfoView), CSize(0, 0), pContext);
    
    m_SplitterWnd2.SetColumnInfo(0, 200, 1);
    
    //将第2行设置为CEditView
    m_SplitterWnd1.CreateView(1, 0, RUNTIME_CLASS(CLogView), CSize(0, 0), pContext);
    
    m_SplitterWnd1.SetRowInfo(0, 300, 10);

    //
    CLogView *pLogView = reinterpret_cast<CLogView*>(m_SplitterWnd1.GetPane(1,0)); 
    gs_pEditLog = &pLogView->GetEditCtrl();
    
	return TRUE;
}

void CMainFrame::LogInfo(CString strLog)
{
#ifndef _DEBUG
    return;
#endif
    
    CTime t = CTime::GetCurrentTime();
    CString strTime = t.Format("%Y/%m/%d %H:%M:%S");
    
    int nLen = gs_pEditLog->GetWindowTextLength();
    gs_pEditLog->SetSel(nLen, nLen);
    gs_pEditLog->ReplaceSel(strTime + " " + strLog + "\r\n");
}

void CMainFrame::OnStartService()
{
    m_Nets.OnStartService();

    m_nMenuID = IDM_START;
}

void CMainFrame::OnStopService() 
{
    m_Nets.OnStopService();

    m_nMenuID = IDM_STOP;	
}

void CMainFrame::OnMenuRefresh(CCmdUI* pCmdUI) 
{
    if (m_nMenuID == pCmdUI->m_nID)
    {
        pCmdUI->SetCheck(TRUE);
    }
    else
    {
        pCmdUI->SetCheck(FALSE);
    }
    	
}

