// MyExport.cpp : 实现文件
//

#include "stdafx.h"
#include "windows原理Day005提高_01.h"
#include "MyExport.h"
#include "afxdialogex.h"


// CMyExport 对话框

IMPLEMENT_DYNAMIC(CMyExport, CDialogEx)

CMyExport::CMyExport(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
	, m_ExportOffset(_T(""))
	, m_Characteristics(_T(""))
	
	, m_Base(_T(""))
	, m_Name(_T(""))
	, m_NameOffset(_T(""))
	, m_NumberOfFunctions(_T(""))
	, m_NumberOfNames(_T(""))
	, m_AddressOfFunctions(_T(""))
	, m_AddressOfNames(_T(""))
	, m_AddressOfNameOrdinals(_T(""))
{

}

CMyExport::~CMyExport()
{
}

void CMyExport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ExportOffset);
	DDX_Text(pDX, IDC_EDIT4, m_Characteristics);
	DDX_Text(pDX, IDC_EDIT5, m_Base);
	DDX_Text(pDX, IDC_EDIT6, m_Name);
	DDX_Text(pDX, IDC_EDIT7, m_NameOffset);
	DDX_Text(pDX, IDC_EDIT8, m_NumberOfFunctions);
	DDX_Text(pDX, IDC_EDIT9, m_NumberOfNames);
	DDX_Text(pDX, IDC_EDIT10, m_AddressOfFunctions);
	DDX_Text(pDX, IDC_EDIT11, m_AddressOfNames);
	DDX_Text(pDX, IDC_EDIT12, m_AddressOfNameOrdinals);
	DDX_Control(pDX, IDC_LIST2, m_ExportList);
}


BEGIN_MESSAGE_MAP(CMyExport, CDialogEx)
END_MESSAGE_MAP()


// CMyExport 消息处理程序

//rva对应的文件偏移,如果转换失败,返回-1
DWORD CMyExport::RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva)
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

BOOL CMyExport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ExportMessage();
	ExportList();



	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CMyExport::ExportMessage()
{
	//得到RVA文件偏移
	DWORD dwExpOfs = RVAToOffset(m_pDosHdr, m_dwExpRva);
	pExpTab = (IMAGE_EXPORT_DIRECTORY*)(dwExpOfs + (DWORD)m_pDosHdr);

	m_Characteristics.Format(L"%08X", pExpTab->Characteristics);//特征值

																//DLL名
	DWORD dwNameOfs = RVAToOffset(m_pDosHdr, pExpTab->Name);
	m_ExportOffset.Format(L"%08X", dwExpOfs);//输出表偏移
	m_NameOffset.Format(L"%S", (char*)(dwNameOfs + (DWORD)m_pDosHdr));//名称字符串
																	  //m_NameOffset = CString((char*)(dwNameOfs + (DWORD)m_pDosHdr));
	m_Base.Format(L"%08X", pExpTab->Base);//基址
	m_Name.Format(L"%08X", pExpTab->Name);//名称
	m_NumberOfFunctions.Format(L"%08X", pExpTab->NumberOfFunctions);//函数数量
	m_NumberOfNames.Format(L"%08X", pExpTab->NumberOfNames);//函数名数量
	m_AddressOfFunctions.Format(L"%08X", pExpTab->AddressOfFunctions);//函数地址的RVA
	m_AddressOfNames.Format(L"%08X", pExpTab->AddressOfNames);//函数名称地址的RVA
	m_AddressOfNameOrdinals.Format(L"%08X", pExpTab->AddressOfNameOrdinals);//序号表的RVA
	UpdateData(FALSE);
}

void CMyExport::ExportList()
{
	// 解析三张表
	DWORD dwExpAddrTabOfs = RVAToOffset(m_pDosHdr, pExpTab->AddressOfFunctions);
	DWORD dwExpNameTabOfs = RVAToOffset(m_pDosHdr, pExpTab->AddressOfNames);
	DWORD dwExpOrdTabOfs = RVAToOffset(m_pDosHdr, pExpTab->AddressOfNameOrdinals);

	// 三张中的地址表,名称都是一个DWORD类型数组
	DWORD* pExpAddr =
		(DWORD*)(dwExpAddrTabOfs + (DWORD)m_pDosHdr);

	DWORD* pExpName =
		(DWORD*)(dwExpNameTabOfs + (DWORD)m_pDosHdr);

	// 序号表是WORD类型的数组
	WORD* pExpOrd =
		(WORD*)(dwExpOrdTabOfs + (WORD)m_pDosHdr);



	CRect rc;
	m_ExportList.GetClientRect(&rc);//指定工作区大小
	int nWidth = rc.Width();
	TCHAR* pcol[] = { L"序号",L"RVA",L"偏移",L"函数名" };
	for (int i = 0; i < 4; i++)
	{
		m_ExportList.InsertColumn(i, pcol[i], 0, 100);
	}
	//添加行内容
	int j = 0;
	CString str;
	for (int i = 0; i < pExpTab->NumberOfFunctions; i++)
	{
		//序号
		str.Format(L"%03X", i + pExpTab->Base);
		m_ExportList.InsertItem(i, str);
		//函数地址表RVA
		str.Format(L"%08X", pExpAddr[i]);
		m_ExportList.SetItemText(i, ++j, str);
		//函数地址表偏移
		str.Format(L"%08X", RVAToOffset(m_pDosHdr,pExpAddr[i]));
		m_ExportList.SetItemText(i, ++j, str);
		//函数名
		DWORD dwNameRva = pExpName[i];
		DWORD dwNameOfs = RVAToOffset(m_pDosHdr, dwNameRva);
		char* pFunctionName = (char*)(dwNameOfs + (DWORD)m_pDosHdr);
		str.Format(L"%S", pFunctionName);
		m_ExportList.SetItemText(i, ++j, str);

		j = 0;
	}
}
