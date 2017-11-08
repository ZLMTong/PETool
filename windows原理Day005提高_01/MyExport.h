#pragma once
#include "afxwin.h"
#include "MyExportList.h"


// CMyExport �Ի���

class CMyExport : public CDialogEx
{
	DECLARE_DYNAMIC(CMyExport)

public:
	CMyExport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyExport();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	DWORD m_dwExpRva;//������
	IMAGE_DOS_HEADER* m_pDosHdr;
	IMAGE_EXPORT_DIRECTORY* pExpTab = NULL;
	

	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos,DWORD dwRva);//rva��Ӧ���ļ�ƫ��,���ת��ʧ��,����-1
	virtual BOOL OnInitDialog();
	void ExportMessage();
	void ExportList();
	CMyExportList m_ExportList;
};
