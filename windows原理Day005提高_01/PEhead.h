#pragma once
#include "afxwin.h"


// PEhead 对话框

class PEhead : public CDialogEx
{
	DECLARE_DYNAMIC(PEhead)

public:
	PEhead(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PEhead();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CString path;

	void getPEHeaderInfo();
	
	CString m_AddressOfEntryPoint;
	CString m_ImageBase;
	CString m_SizeOfImage;
	CString m_BaseOfCode;
	CString m_BaseOfData;
	CString m_SectionAlignment;
	CString m_FileAlignment;
	CString m_Magic;
	CString m_Subsystem;
	CString m_NumberOfSections;
	CString m_TimeDateStamp;
	CString m_SizeOfHeaders;
	CString m_Characteristics;
	CString m_CheckSum;
	CString m_SizeOfOptionalHeader;
	CString m_NumberOfRvaAndSizes;

	IMAGE_FILE_HEADER* m_pFileHdr;		//文件头
	IMAGE_OPTIONAL_HEADER* m_pOptHdr;
	IMAGE_SECTION_HEADER* m_pScnHdr;
	IMAGE_DOS_HEADER* m_pDosHdr;
	afx_msg void OnCatalogue();
	afx_msg void OnSection();
	afx_msg void OnTime();
	afx_msg void OnRVAToOffset();
};
