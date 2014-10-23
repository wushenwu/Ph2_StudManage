// Request.cpp: implementation of the CRequest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "Request.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRequest::CRequest()
{

}

CRequest::CRequest(int nOpCode, CString &strTbl, CString &strReq)
{
    m_Operation = nOpCode;
    m_strTbl    = strTbl;
    m_strReq    = strReq;
}

CRequest::~CRequest()
{

}

//////////////////////////////////////////////////////////////////////////
//用于Serialization
IMPLEMENT_SERIAL(CRequest, CObject, VERSIONABLE_SCHEMA)

/************************************************************************/
/* In the beginning, we want to use this to achieve perminent Cache.
   But at the last moment, we decide not to deal with Cache.          */
/************************************************************************/
void CRequest::Serialize(CArchive& ar)
{
    //先调用基类的
    CObject::Serialize(ar);
    
    if (ar.IsStoring())
    {
        //insertion
        
    }
    else
    {
        //extraction
    }
}
