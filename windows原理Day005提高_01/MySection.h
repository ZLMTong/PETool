#pragma once
#include "afxcmn.h"


// CMySection �Ի���

class CMySection : public CDialogEx
{
	DECLARE_DYNAMIC(CMySection)

public:
	CMySection(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMySection();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	IMAGE_SECTION_HEADER* m_pScnHdr;
	IMAGE_FILE_HEADER* m_pFileHdr;
	CRect rc;
	CListCtrl m_SectionList;
};
