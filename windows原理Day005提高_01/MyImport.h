#pragma once
#include "MyImportListA.h"
#include "MyImportListB.h"


// CMyImport �Ի���

class CMyImport : public CDialogEx
{
	DECLARE_DYNAMIC(CMyImport)

public:
	CMyImport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyImport();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CMyImportListA m_ImportListA;
	CMyImportListB m_ImportListB;
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva);//rva��Ӧ���ļ�ƫ��,���ת��ʧ��,����-1

	DWORD m_dwImpRva;//�����RVA
	IMAGE_DOS_HEADER* m_pDosHdr;//DOSͷ
	IMAGE_IMPORT_DESCRIPTOR* m_pImpArray;//��������
	CRect rc;
	void ImportListA();
	void ImportListB();
	virtual BOOL OnInitDialog();
};
