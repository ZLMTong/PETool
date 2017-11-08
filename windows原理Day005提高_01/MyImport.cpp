// MyImport.cpp : 实现文件
//

#include "stdafx.h"
#include "windows原理Day005提高_01.h"
#include "MyImport.h"
#include "afxdialogex.h"


// CMyImport 对话框

IMPLEMENT_DYNAMIC(CMyImport, CDialogEx)

CMyImport::CMyImport(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

CMyImport::~CMyImport()
{
}

void CMyImport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ImportListA);
	DDX_Control(pDX, IDC_LIST2, m_ImportListB);
}


DWORD CMyImport::RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva)
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

void CMyImport::ImportListA()
{
	//输入表偏移
	m_pImpArray = (IMAGE_IMPORT_DESCRIPTOR*)(RVAToOffset(m_pDosHdr, m_dwImpRva) + (DWORD)m_pDosHdr);
	
	
	m_ImportListA.GetClientRect(&rc);//指定工作区大小
	int nWidth = rc.Width();
	TCHAR* pcol[] = { L"DLL名称",L"OriginalFirstThunk",L"日期时间标志",L"ForwarderChain",L"名称", L"FirstThunk" };
	for (int i = 0; i < 6; i++)
	{
		m_ImportListA.InsertColumn(i, pcol[i], 0, 120);
	}
	//添加行内容
	int j = 0;
	CString str;
	for (int i = 0; m_pImpArray->Name != 0; i++)
	{
		//DLL名称
		DWORD dwNameOfs = RVAToOffset(m_pDosHdr, m_pImpArray->Name);
		char* pDllNmae = (char*)(dwNameOfs + (DWORD)m_pDosHdr);
		str.Format(L"%S", pDllNmae);
		m_ImportListA.InsertItem(i, str);
		//导入名称表的RVA
		str.Format(L"%08X", m_pImpArray->OriginalFirstThunk);
		m_ImportListA.SetItemText(i, ++j, str);
		//时间
		str.Format(L"%08X", m_pImpArray->TimeDateStamp);
		m_ImportListA.SetItemText(i, ++j, str);
		//转发用
		str.Format(L"%08X", m_pImpArray->ForwarderChain);
		m_ImportListA.SetItemText(i, ++j, str);
		//NAME的RVA
		str.Format(L"%08X", m_pImpArray->Name);
		m_ImportListA.SetItemText(i, ++j, str);
		//导入地址表的RVA
		str.Format(L"%08X", m_pImpArray->FirstThunk);
		m_ImportListA.SetItemText(i, ++j, str);

		j = 0;
		m_pImpArray++;
	}
}

void CMyImport::ImportListB()
{

	//输入表偏移
	m_pImpArray = (IMAGE_IMPORT_DESCRIPTOR*)(RVAToOffset(m_pDosHdr, m_dwImpRva) + (DWORD)m_pDosHdr);
	m_ImportListB.GetClientRect(&rc);//指定工作区大小
	int nWidth = rc.Width();
	TCHAR* pcol[] = { L"ThunkRVA",L"Thunk偏移",L"Thunk值",L"提示",L"API名称" };
	for (int i = 0; i < 5; i++)
	{
		if (i<=3)
		{
			m_ImportListB.InsertColumn(i, pcol[i], 0, nWidth/8);
		}
		else
		{
			m_ImportListB.InsertColumn(i, pcol[i], 0, nWidth / 2);
		}
	}
	while (m_pImpArray->Name != 0)
	{
		DWORD INTOfs = RVAToOffset(m_pDosHdr, m_pImpArray->FirstThunk);
		DWORD IATOfs = RVAToOffset(m_pDosHdr, m_pImpArray->FirstThunk);
		IMAGE_THUNK_DATA* pInt = (IMAGE_THUNK_DATA*)(INTOfs + (DWORD)m_pDosHdr);
		IMAGE_THUNK_DATA* pIat = (IMAGE_THUNK_DATA*)(IATOfs + (DWORD)m_pDosHdr);
	
		//添加行内容
		int j = 0;
		int add = 0;
		CString str;
		for (int i = 0; pInt->u1.Function != NULL; i++)
		{	
			//保存函数名称的结构体
			DWORD dwImpNameOfs = RVAToOffset(m_pDosHdr, pInt->u1.Function);//导入函数的地址
			IMAGE_IMPORT_BY_NAME* pImpName = (IMAGE_IMPORT_BY_NAME*)(dwImpNameOfs + (DWORD)m_pDosHdr);

			//ThunkRVA
			str.Format(L"%08X", m_pImpArray->OriginalFirstThunk + add * 4);
			m_ImportListB.InsertItem(i, str);
			//Thunk偏移
			str.Format(L"%08X", RVAToOffset(m_pDosHdr, m_pImpArray->FirstThunk) + add * 4);
			m_ImportListB.SetItemText(i, ++j, str);
			//Thunk值
			str.Format(L"%08X", pInt->u1.Function);
			m_ImportListB.SetItemText(i, ++j, str);
			//函数的序号
			str.Format(L"%04X", pImpName->Hint);
			m_ImportListB.SetItemText(i, ++j, str);
			//函数名字
			str.Format(L"%S", pImpName->Name);
			m_ImportListB.SetItemText(i, ++j, str);
			j = 0;
			pInt++;
			add++;
		}
		m_pImpArray++;
	}
}

BEGIN_MESSAGE_MAP(CMyImport, CDialogEx)
END_MESSAGE_MAP()


// CMyImport 消息处理程序


BOOL CMyImport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ImportListA();
	ImportListB()
		;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
