#pragma once
#include "afxcmn.h"


// CMyRelocation �Ի���

typedef struct TypeOffset {
	WORD Offset : 12;  // (1) ��СΪ12Bit���ض�λƫ��
	WORD Type : 4;    // (2) ��СΪ4Bit���ض�λ��Ϣ����ֵ
}TypeOffset;	    // ����ṹ����A1Pass�ܽ��

class CMyRelocation : public CDialogEx
{
	DECLARE_DYNAMIC(CMyRelocation)

public:
	CMyRelocation(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyRelocation();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG9 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DWORD RVAToOffset(LPVOID pDos, DWORD dwRva);
public:
	CListCtrl m_ReListA;
	virtual BOOL OnInitDialog();
	DWORD dwRetRva;
	IMAGE_DOS_HEADER* m_pDosHdr;
	void GetRelovation();
	CListCtrl m_ReListB;
};
