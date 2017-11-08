#pragma once


// CMyRVAToOffset 对话框

class CMyRVAToOffset : public CDialogEx
{
	DECLARE_DYNAMIC(CMyRVAToOffset)

public:
	CMyRVAToOffset(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyRVAToOffset();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_VA;
	CString m_RVA;
	CString m_Offset;

	IMAGE_DOS_HEADER* m_pDosHdr;
	IMAGE_OPTIONAL_HEADER* m_pOptHdr;
	IMAGE_SECTION_HEADER* m_pScnHdr;
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva);
	virtual BOOL OnInitDialog();
	afx_msg void OnExecute();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
