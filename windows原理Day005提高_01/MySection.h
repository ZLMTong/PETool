#pragma once
#include "afxcmn.h"


// CMySection 对话框

class CMySection : public CDialogEx
{
	DECLARE_DYNAMIC(CMySection)

public:
	CMySection(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMySection();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	IMAGE_SECTION_HEADER* m_pScnHdr;
	IMAGE_FILE_HEADER* m_pFileHdr;
	CRect rc;
	CListCtrl m_SectionList;
};
