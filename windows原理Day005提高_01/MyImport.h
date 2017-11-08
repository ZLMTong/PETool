#pragma once
#include "MyImportListA.h"
#include "MyImportListB.h"


// CMyImport 对话框

class CMyImport : public CDialogEx
{
	DECLARE_DYNAMIC(CMyImport)

public:
	CMyImport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyImport();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyImportListA m_ImportListA;
	CMyImportListB m_ImportListB;
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva);//rva对应的文件偏移,如果转换失败,返回-1

	DWORD m_dwImpRva;//导入表RVA
	IMAGE_DOS_HEADER* m_pDosHdr;//DOS头
	IMAGE_IMPORT_DESCRIPTOR* m_pImpArray;//导入表对象
	CRect rc;
	void ImportListA();
	void ImportListB();
	virtual BOOL OnInitDialog();
};
