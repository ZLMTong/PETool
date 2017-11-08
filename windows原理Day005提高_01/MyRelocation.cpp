// MyRelocation.cpp : 实现文件
//

#include "stdafx.h"
#include "windows原理Day005提高_01.h"
#include "MyRelocation.h"
#include "afxdialogex.h"


// CMyRelocation 对话框

IMPLEMENT_DYNAMIC(CMyRelocation, CDialogEx)

CMyRelocation::CMyRelocation(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG9, pParent)
{

}

CMyRelocation::~CMyRelocation()
{
}

void CMyRelocation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ReListA);
	DDX_Control(pDX, IDC_LIST2, m_ReListB);
}


BEGIN_MESSAGE_MAP(CMyRelocation, CDialogEx)
END_MESSAGE_MAP()


// CMyRelocation 消息处理程序

DWORD CMyRelocation::RVAToOffset(LPVOID pDos, DWORD dwRva) {
	// 找到需要转换的RVA所在的区段,然后计算偏移.
	IMAGE_DOS_HEADER* pDosHdr = (IMAGE_DOS_HEADER*)pDos;

	IMAGE_NT_HEADERS* pNtHdr =
		(IMAGE_NT_HEADERS*)((DWORD)pDos + pDosHdr->e_lfanew);

	IMAGE_SECTION_HEADER* pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);

	DWORD dwScnCount = pNtHdr->FileHeader.NumberOfSections;

	for (DWORD i = 0; i < dwScnCount; i++) {
		// 判断RVA是否在一个区段内
		if (dwRva >= pScnHdr[i].VirtualAddress
			&& dwRva < pScnHdr[i].VirtualAddress + pScnHdr[i].SizeOfRawData) {

			return dwRva - pScnHdr[i].VirtualAddress + pScnHdr[i].PointerToRawData;
		}
	}
	return -1;
}

BOOL CMyRelocation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//添加列标题
	CRect rc;
	m_ReListA.GetClientRect(&rc);//指定工作区大小
	int nWidth = rc.Width();
	TCHAR* pcol[] = { L"索引", L"RVA", L"项目" };
	for (int i = 0; i < 3; i++)
	{
		m_ReListA.InsertColumn(i, pcol[i], 0, 100);
	}

	m_ReListB.GetClientRect(&rc);//指定工作区大小
	TCHAR* pcolB[] = { L"索引", L"RVA", L"偏移" ,L"类型", L"FAR地址"};
	for (int i = 0; i < 5; i++)
	{
		m_ReListB.InsertColumn(i, pcolB[i], 0, 100);
	}
	GetRelovation();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CMyRelocation::GetRelovation()
{
	// 获取重定位表
	/*
	重定位块结构体
	重定位表由一个或一个以上的重定位块结构体组成
	重定位表是一个由重定位块结构体的集合(也就是一个数组),数组是以一个全0的元素最为
	结束的标志.
	typedef struct _IMAGE_BASE_RELOCATION {
	DWORD   VirtualAddress; // 块的起始RVA
	DWORD   SizeOfBlock;    // 整个块的大小(包括VirtualAddress和SizeOfBlock两个字段的大小)
	//WORD    TypeOffset[1];
	} IMAGE_BASE_RELOCATION;
	*/
	IMAGE_BASE_RELOCATION* pBaseRel;
	pBaseRel = (IMAGE_BASE_RELOCATION*)(RVAToOffset(m_pDosHdr, dwRetRva) + (DWORD)m_pDosHdr);
	int nNum = 0;
	int line = 0;

	
	CString str;
	CString strB;

	while (pBaseRel->VirtualAddress != 0)
	{
		// 得到重定位项的个数
		DWORD dwCount = (pBaseRel->SizeOfBlock - (sizeof(IMAGE_BASE_RELOCATION))) / sizeof(WORD);

		//序号
		str.Format(L"%d", nNum+1);
		m_ReListA.InsertItem(nNum, str);

		//需要重定位的位置RVA
		str.Format(L"%08X", pBaseRel->VirtualAddress);
		m_ReListA.SetItemText(nNum, ++line, str);

		//项目
		str.Format(L"%0Xh/%dd", dwCount,dwCount);
		m_ReListA.SetItemText(nNum, ++line, str);

		// 得到重定位项的开始地址
		TypeOffset* pTypeOffset = (TypeOffset*)(pBaseRel + 1);
		int lcow = 0;
		int lineB = 0;
		for (DWORD i = 0; i < dwCount; ++i)
		{

			if (pTypeOffset[i].Type == IMAGE_REL_BASED_HIGHLOW) {

				// 得到需要修复的RVA
				DWORD dwRva = pBaseRel->VirtualAddress + pTypeOffset[i].Offset;

				DWORD dwOffset = RVAToOffset(m_pDosHdr, dwRva);

				// 得到需要修复的opcode地址操作数的所在的地址
				DWORD* pAddress = (DWORD*)(dwOffset + (DWORD)m_pDosHdr);
				
				//序号
				strB.Format(L"%d", lcow +1);
				m_ReListB.InsertItem(lcow, strB);
				
				//需要重定位的位置RVA
				strB.Format(L"%08X", pBaseRel->VirtualAddress + pTypeOffset[i].Offset);
				m_ReListB.SetItemText(lcow, ++lineB, strB);
				
				//偏移
				strB.Format(L"%08X", RVAToOffset(m_pDosHdr, pBaseRel->VirtualAddress + pTypeOffset[i].Offset));
				m_ReListB.SetItemText(lcow, ++lineB, strB);
				
				//类型
				strB.Format(L"%s", L"HIGHLOW (3)");
				m_ReListB.SetItemText(lcow, ++lineB, strB);

				//VA
				strB.Format(L"%08X",*pAddress);
				m_ReListB.SetItemText(lcow, ++lineB, strB);
				lcow++;
				lineB = 0;
			}
		}

		// 找到下一个重定位块
		pBaseRel = (IMAGE_BASE_RELOCATION*)((DWORD)pBaseRel + pBaseRel->SizeOfBlock);

		nNum++;
		line = 0;
	}
}
