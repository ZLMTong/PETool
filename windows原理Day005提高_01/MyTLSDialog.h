#pragma once


// CMyTLSDialog 对话框

class CMyTLSDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMyTLSDialog)

public:
	CMyTLSDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyTLSDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva);
public:
	CString m_StartAddressOfRawData;
	CString m_EndAddressOfRawData;
	CString m_AddressOfIndex;
	CString m_AddressOfCallBacks;
	CString m_SizeOfZeroFill;
	CString m_Characteristics;

	IMAGE_TLS_DIRECTORY* m_TLS;
	DWORD dwTls;
	IMAGE_DOS_HEADER* m_pDosHdr;
	virtual BOOL OnInitDialog();
};
