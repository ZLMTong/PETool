#pragma once
#include "afxcmn.h"


// CMyResource 对话框

class CMyResource : public CDialogEx
{
	DECLARE_DYNAMIC(CMyResource)

public:
	CMyResource(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyResource();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva);
	
public:
	CTreeCtrl m_ResourceTree;
	DWORD m_dwResRootRva;//资源根目录RVA
	IMAGE_RESOURCE_DIRECTORY* m_pResRoot;//资源目录

	virtual BOOL OnInitDialog();
	void parseResourcesTable(DWORD dwResRootDirAddr,/*根目录的首地址*/
		IMAGE_RESOURCE_DIRECTORY* pResDir,/*需要解析的资源目录*/
		int nDeep /*记录这是第几层目录*/
		);
	IMAGE_DOS_HEADER* m_pDosHdr;
	HTREEITEM hTree1 = NULL;
	HTREEITEM hTree2 = NULL;
};
