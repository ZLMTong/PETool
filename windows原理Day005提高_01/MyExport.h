#pragma once
#include "afxwin.h"
#include "MyExportList.h"


// CMyExport 对话框

class CMyExport : public CDialogEx
{
	DECLARE_DYNAMIC(CMyExport)

public:
	CMyExport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyExport();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_ExportOffset;
	CString m_Characteristics;
	CString m_Base;
	CString m_Name;
	CString m_NameOffset;
	CString m_NumberOfFunctions;
	CString m_NumberOfNames;
	CString m_AddressOfFunctions;
	CString m_AddressOfNames;
	CString m_AddressOfNameOrdinals;
	DWORD m_dwExpRva;//导出表
	IMAGE_DOS_HEADER* m_pDosHdr;
	IMAGE_EXPORT_DIRECTORY* pExpTab = NULL;
	

	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos,DWORD dwRva);//rva对应的文件偏移,如果转换失败,返回-1
	virtual BOOL OnInitDialog();
	void ExportMessage();
	void ExportList();
	CMyExportList m_ExportList;
};
