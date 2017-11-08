// MyRVAToOffset.cpp : 实现文件
//

#include "stdafx.h"
#include "windows原理Day005提高_01.h"
#include "MyRVAToOffset.h"
#include "afxdialogex.h"


// CMyRVAToOffset 对话框

IMPLEMENT_DYNAMIC(CMyRVAToOffset, CDialogEx)

CMyRVAToOffset::CMyRVAToOffset(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG7, pParent)
	, m_VA(_T(""))
	, m_RVA(_T(""))
	, m_Offset(_T(""))
{

}

CMyRVAToOffset::~CMyRVAToOffset()
{
}

void CMyRVAToOffset::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_VA);
	DDX_Text(pDX, IDC_EDIT4, m_RVA);
	DDX_Text(pDX, IDC_EDIT5, m_Offset);
}


DWORD CMyRVAToOffset::RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva)
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
		if (dwRva >= pScnHdr[i].VirtualAddress && dwRva < dwEndOfSection)
		{
			// 2. 计算该RVA在区段内的偏移:rva 减去首地址
			DWORD dwOffset = dwRva - pScnHdr[i].VirtualAddress;
			// 3. 将区段内偏移加上区段的文件开始偏移
			return dwOffset + pScnHdr[i].PointerToRawData;
		}
	}
	return -1;
}

BEGIN_MESSAGE_MAP(CMyRVAToOffset, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyRVAToOffset::OnExecute)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMyRVAToOffset 消息处理程序


BOOL CMyRVAToOffset::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMyRVAToOffset::OnExecute()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	wchar_t *p = L"";
	int dword  = wcstol(m_RVA.GetBuffer(), &p, 16);
	m_Offset.Format(L"%08X", RVAToOffset(m_pDosHdr, dword));
	m_VA.Format(L"%08X", m_pOptHdr->ImageBase + dword);
	UpdateData(FALSE);
}


void CMyRVAToOffset::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnExecute();

	CDialogEx::OnLButtonDown(nFlags, point);
}
