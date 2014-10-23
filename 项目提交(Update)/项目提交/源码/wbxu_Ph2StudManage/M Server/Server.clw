; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTblInfoView
LastTemplate=CListView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Server.h"
LastPage=0

ClassCount=12
Class1=CServerApp
Class2=CServerDoc
Class3=CServerView
Class4=CMainFrame
Class9=CAboutDlg

ResourceCount=4
Resource1=IDR_MAINFRAME
Resource2=IDD_ABOUTBOX
Class10=CTblTreeView
Class11=CLogView
Class12=CTblInfoView
Resource3=IDR_MAINFRAME1
Resource4=IDD_DLG_CONNECT

[CLS:CServerApp]
Type=0
HeaderFile=Server.h
ImplementationFile=Server.cpp
Filter=N

[CLS:CServerDoc]
Type=0
HeaderFile=ServerDoc.h
ImplementationFile=ServerDoc.cpp
Filter=N

[CLS:CServerView]
Type=0
HeaderFile=ServerView.h
ImplementationFile=ServerView.cpp
Filter=C


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=Server.cpp
ImplementationFile=Server.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=IDM_START
Command2=IDM_STOP
Command3=ID_APP_EXIT
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_VIEW_TOOLBAR
Command9=ID_VIEW_STATUS_BAR
Command10=ID_APP_ABOUT
CommandCount=10

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_DLG_CONNECT]
Type=1
Class=?
ControlCount=0

[CLS:CTblTreeView]
Type=0
HeaderFile=TblTreeView.h
ImplementationFile=TblTreeView.cpp
BaseClass=CTreeView
Filter=C
VirtualFilter=VWC

[CLS:CLogView]
Type=0
HeaderFile=LogView.h
ImplementationFile=LogView.cpp
BaseClass=CEditView
Filter=C

[CLS:CTblInfoView]
Type=0
HeaderFile=TblInfoView.h
ImplementationFile=TblInfoView.cpp
BaseClass=CListView
Filter=C
VirtualFilter=VWC

[MNU:IDR_MAINFRAME1]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=IDM_START
Command17=IDM_STOP
Command18=ID_APP_ABOUT
CommandCount=18

