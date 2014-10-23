// Request.h: interface for the CRequest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REQUEST_H__B4F49605_837E_4761_9705_DE323C876B6A__INCLUDED_)
#define AFX_REQUEST_H__B4F49605_837E_4761_9705_DE323C876B6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRequest : public CObject     //1.用于Serialization.
{
public:
	CRequest();                     //2.空的构造，对于Serialization是必须的
    CRequest(int nOpCode, CString &strTbl, CString &strReq);
	virtual ~CRequest();

    //////////////////////////////////////////////////////////////////////////
    //用于Serialization
    DECLARE_SERIAL(CRequest)
        
    virtual void Serialize(CArchive& ar);

public:
    long     m_Operation;    //what kind of operation, select, insert, del, update，
    CString m_strTbl;       //which table
    CString m_strReq;       //specific field and values

};

#endif // !defined(AFX_REQUEST_H__B4F49605_837E_4761_9705_DE323C876B6A__INCLUDED_)
