# Microsoft Developer Studio Generated NMAKE File, Based on Server.dsp
!IF "$(CFG)" == ""
CFG=Server - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Server - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Server - Win32 Release" && "$(CFG)" != "Server - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Server.mak" CFG="Server - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Server - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Server - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Server - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Server.exe"


CLEAN :
	-@erase "$(INTDIR)\DataBase.obj"
	-@erase "$(INTDIR)\LogView.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MyPacket.obj"
	-@erase "$(INTDIR)\MySocket.obj"
	-@erase "$(INTDIR)\MyThreadPool.obj"
	-@erase "$(INTDIR)\NetWork.obj"
	-@erase "$(INTDIR)\RecvFromWorkItem.obj"
	-@erase "$(INTDIR)\Server.obj"
	-@erase "$(INTDIR)\Server.pch"
	-@erase "$(INTDIR)\Server.res"
	-@erase "$(INTDIR)\ServerDoc.obj"
	-@erase "$(INTDIR)\ServerView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TblInfoView.obj"
	-@erase "$(INTDIR)\TblTreeView.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Server.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Server.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\Server.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Server.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Server.pdb" /machine:I386 /out:"$(OUTDIR)\Server.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DataBase.obj" \
	"$(INTDIR)\LogView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MyPacket.obj" \
	"$(INTDIR)\MySocket.obj" \
	"$(INTDIR)\MyThreadPool.obj" \
	"$(INTDIR)\NetWork.obj" \
	"$(INTDIR)\RecvFromWorkItem.obj" \
	"$(INTDIR)\Server.obj" \
	"$(INTDIR)\ServerDoc.obj" \
	"$(INTDIR)\ServerView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TblInfoView.obj" \
	"$(INTDIR)\TblTreeView.obj" \
	"$(INTDIR)\Server.res"

"$(OUTDIR)\Server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\bin\Server.exe" "$(OUTDIR)\Server.pch" "$(OUTDIR)\Server.bsc"


CLEAN :
	-@erase "$(INTDIR)\DataBase.obj"
	-@erase "$(INTDIR)\DataBase.sbr"
	-@erase "$(INTDIR)\LogView.obj"
	-@erase "$(INTDIR)\LogView.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MyPacket.obj"
	-@erase "$(INTDIR)\MyPacket.sbr"
	-@erase "$(INTDIR)\MySocket.obj"
	-@erase "$(INTDIR)\MySocket.sbr"
	-@erase "$(INTDIR)\MyThreadPool.obj"
	-@erase "$(INTDIR)\MyThreadPool.sbr"
	-@erase "$(INTDIR)\NetWork.obj"
	-@erase "$(INTDIR)\NetWork.sbr"
	-@erase "$(INTDIR)\RecvFromWorkItem.obj"
	-@erase "$(INTDIR)\RecvFromWorkItem.sbr"
	-@erase "$(INTDIR)\Server.obj"
	-@erase "$(INTDIR)\Server.pch"
	-@erase "$(INTDIR)\Server.res"
	-@erase "$(INTDIR)\Server.sbr"
	-@erase "$(INTDIR)\ServerDoc.obj"
	-@erase "$(INTDIR)\ServerDoc.sbr"
	-@erase "$(INTDIR)\ServerView.obj"
	-@erase "$(INTDIR)\ServerView.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TblInfoView.obj"
	-@erase "$(INTDIR)\TblInfoView.sbr"
	-@erase "$(INTDIR)\TblTreeView.obj"
	-@erase "$(INTDIR)\TblTreeView.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Server.bsc"
	-@erase "$(OUTDIR)\Server.pdb"
	-@erase "..\bin\Server.exe"
	-@erase "..\bin\Server.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\Server.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Server.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DataBase.sbr" \
	"$(INTDIR)\LogView.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\MyPacket.sbr" \
	"$(INTDIR)\MySocket.sbr" \
	"$(INTDIR)\MyThreadPool.sbr" \
	"$(INTDIR)\NetWork.sbr" \
	"$(INTDIR)\RecvFromWorkItem.sbr" \
	"$(INTDIR)\Server.sbr" \
	"$(INTDIR)\ServerDoc.sbr" \
	"$(INTDIR)\ServerView.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TblInfoView.sbr" \
	"$(INTDIR)\TblTreeView.sbr"

"$(OUTDIR)\Server.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Server.pdb" /debug /machine:I386 /out:"../bin/Server.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\DataBase.obj" \
	"$(INTDIR)\LogView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MyPacket.obj" \
	"$(INTDIR)\MySocket.obj" \
	"$(INTDIR)\MyThreadPool.obj" \
	"$(INTDIR)\NetWork.obj" \
	"$(INTDIR)\RecvFromWorkItem.obj" \
	"$(INTDIR)\Server.obj" \
	"$(INTDIR)\ServerDoc.obj" \
	"$(INTDIR)\ServerView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TblInfoView.obj" \
	"$(INTDIR)\TblTreeView.obj" \
	"$(INTDIR)\Server.res"

"..\bin\Server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Server.dep")
!INCLUDE "Server.dep"
!ELSE 
!MESSAGE Warning: cannot find "Server.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Server - Win32 Release" || "$(CFG)" == "Server - Win32 Debug"
SOURCE=.\DataBase.cpp

!IF  "$(CFG)" == "Server - Win32 Release"


"$(INTDIR)\DataBase.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Server.pch"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\DataBase.obj"	"$(INTDIR)\DataBase.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\LogView.cpp

!IF  "$(CFG)" == "Server - Win32 Release"


"$(INTDIR)\LogView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Server.pch"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\LogView.obj"	"$(INTDIR)\LogView.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "Server - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Server.pch"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\MySocket\MyPacket.cpp

!IF  "$(CFG)" == "Server - Win32 Release"


"$(INTDIR)\MyPacket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Server.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\MyPacket.obj"	"$(INTDIR)\MyPacket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\MySocket\MySocket.cpp

!IF  "$(CFG)" == "Server - Win32 Release"


"$(INTDIR)\MySocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Server.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\MySocket.obj"	"$(INTDIR)\MySocket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ThreadPool\MyThreadPool.cpp

!IF  "$(CFG)" == "Server - Win32 Release"


"$(INTDIR)\MyThreadPool.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Server.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\MyThreadPool.obj"	"$(INTDIR)\MyThreadPool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\NetWork.cpp

!IF  "$(CFG)" == "Server - Win32 Release"


"$(INTDIR)\NetWork.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Server.pch"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\NetWork.obj"	"$(INTDIR)\NetWork.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\RecvFromWorkItem.cpp

!IF  "$(CFG)" == "Server - Win32 Release"


"$(INTDIR)\RecvFromWorkItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Server.pch"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\RecvFromWorkItem.obj"	"$(INTDIR)\RecvFromWorkItem.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Server.cpp

!IF  "$(CFG)" == "Server - Win32 Release"


"$(INTDIR)\Server.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Server.pch"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\Server.obj"	"$(INTDIR)\Server.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Server.rc

"$(INTDIR)\Server.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ServerDoc.cpp

!IF  "$(CFG)" == "Server - Win32 Release"


"$(INTDIR)\ServerDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Server.pch"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\ServerDoc.obj"	"$(INTDIR)\ServerDoc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ServerView.cpp

!IF  "$(CFG)" == "Server - Win32 Release"


"$(INTDIR)\ServerView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Server.pch"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\ServerView.obj"	"$(INTDIR)\ServerView.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Server.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Server.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Server.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TblInfoView.cpp

!IF  "$(CFG)" == "Server - Win32 Release"


"$(INTDIR)\TblInfoView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Server.pch"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\TblInfoView.obj"	"$(INTDIR)\TblInfoView.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TblTreeView.cpp

!IF  "$(CFG)" == "Server - Win32 Release"


"$(INTDIR)\TblTreeView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Server.pch"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\TblTreeView.obj"	"$(INTDIR)\TblTreeView.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

