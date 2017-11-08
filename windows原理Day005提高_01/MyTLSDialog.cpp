// MyTLSDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "windows原理Day005提高_01.h"
#include "MyTLSDialog.h"
#include "afxdialogex.h"


// CMyTLSDialog 对话框

IMPLEMENT_DYNAMIC(CMyTLSDialog, CDialogEx)

CMyTLSDialog::CMyTLSDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG8, pParent)
	, m_StartAddressOfRawData(_T(""))
	, m_EndAddressOfRawData(_T(""))
	, m_AddressOfIndex(_T(""))
	, m_AddressOfCallBacks(_T(""))
	, m_SizeOfZeroFill(_T(""))
	, m_Characteristics(_T(""))
{

}

CMyTLSDialog::~CMyTLSDialog()
{
}

void CMyTLSDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_StartAddressOfRawData);
	DDX_Text(pDX, IDC_EDIT9, m_EndAddressOfRawData);
	DDX_Text(pDX, IDC_EDIT10, m_AddressOfIndex);
	DDX_Text(pDX, IDC_EDIT11, m_AddressOfCallBacks);
	DDX_Text(pDX, IDC_EDIT12, m_SizeOfZeroFill);
	DDX_Text(pDX, IDC_EDIT13, m_Characteristics);
}


BEGIN_MESSAGE_MAP(CMyTLSDialog, CDialogEx)
END_MESSAGE_MAP()


// CMyTLSDialog 消息处理程序

//rva对应的文件偏移,如果转换失败,返回-1
DWORD CMyTLSDialog::RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva)
{
	IMAGE_SECTION_HEADER* pScnHdr;

	IMAGE_NT_HEADERS* pNtHdr =
		(IMAGE_NT_HEADERS*)(pDos->e_lfanew + (DWORD)pDos);

	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);//区段表头
	DWORD dwNumberOfScn = pNtHdr->FileHeader.NumberOfSections;//区段的数量

															  // 1. 遍历所有区段找到所在区段
	for (int i = 0; i < dwNumberOfScn; ++i)
	{
		DWORD dwEndOfSection = pScnHdr[i].VirtualAddress + pScnHdr[i].SizeOfRawData;
		// 判断这个RVA是否在一个区段的范围内
		if (dwRva >= pScnHdr[i].VirtualAddress
			&& dwRva < dwEndOfSection)
		{
			// 2. 计算该RVA在区段内的偏移:rva 减去首地址
			DWORD dwOffset = dwRva - pScnHdr[i].VirtualAddress;
			// 3. 将区段内偏移加上区段的文件开始偏移
			return dwOffset + pScnHdr[i].PointerToRawData;
		}
	}
	return -1;
}


BOOL CMyTLSDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	DWORD dwExpOfs = RVAToOffset(m_pDosHdr, dwTls);
	m_TLS = (IMAGE_TLS_DIRECTORY*)(dwExpOfs + (DWORD)m_pDosHdr);

	m_StartAddressOfRawData.Format(L"%08X", m_TLS->StartAddressOfRawData);
	m_EndAddressOfRawData.Format(L"%08X", m_TLS->EndAddressOfRawData);
	m_AddressOfIndex.Format(L"%08X", m_TLS->AddressOfIndex);
	m_AddressOfCallBacks.Format(L"%08X", m_TLS->AddressOfCallBacks);
	m_SizeOfZeroFill.Format(L"%08X", m_TLS->SizeOfZeroFill);
	m_Characteristics.Format(L"%08X", m_TLS->Characteristics);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
