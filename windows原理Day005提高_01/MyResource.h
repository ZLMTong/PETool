#pragma once
#include "afxcmn.h"


// CMyResource �Ի���

class CMyResource : public CDialogEx
{
	DECLARE_DYNAMIC(CMyResource)

public:
	CMyResource(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyResource();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva);
	
public:
	CTreeCtrl m_ResourceTree;
	DWORD m_dwResRootRva;//��Դ��Ŀ¼RVA
	IMAGE_RESOURCE_DIRECTORY* m_pResRoot;//��ԴĿ¼

	virtual BOOL OnInitDialog();
	void parseResourcesTable(DWORD dwResRootDirAddr,/*��Ŀ¼���׵�ַ*/
		IMAGE_RESOURCE_DIRECTORY* pResDir,/*��Ҫ��������ԴĿ¼*/
		int nDeep /*��¼���ǵڼ���Ŀ¼*/
		);
	IMAGE_DOS_HEADER* m_pDosHdr;
	HTREEITEM hTree1 = NULL;
	HTREEITEM hTree2 = NULL;
};
