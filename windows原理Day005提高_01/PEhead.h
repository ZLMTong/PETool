#pragma once
#include "afxwin.h"


// PEhead �Ի���

class PEhead : public CDialogEx
{
	DECLARE_DYNAMIC(PEhead)

public:
	PEhead(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PEhead();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	IMAGE_FILE_HEADER* m_pFileHdr;		//�ļ�ͷ
	IMAGE_OPTIONAL_HEADER* m_pOptHdr;
	IMAGE_SECTION_HEADER* m_pScnHdr;
	IMAGE_DOS_HEADER* m_pDosHdr;
	afx_msg void OnCatalogue();
	afx_msg void OnSection();
	afx_msg void OnTime();
	afx_msg void OnRVAToOffset();
};
