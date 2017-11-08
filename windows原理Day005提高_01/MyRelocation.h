#pragma once
#include "afxcmn.h"


// CMyRelocation 对话框

typedef struct TypeOffset {
	WORD Offset : 12;  // (1) 大小为12Bit的重定位偏移
	WORD Type : 4;    // (2) 大小为4Bit的重定位信息类型值
}TypeOffset;	    // 这个结构体是A1Pass总结的

class CMyRelocation : public CDialogEx
{
	DECLARE_DYNAMIC(CMyRelocation)

public:
	CMyRelocation(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyRelocation();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG9 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
