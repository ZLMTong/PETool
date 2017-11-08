// PEhead.cpp : 实现文件
//

#include "stdafx.h"
#include "windows原理Day005提高_01.h"
#include "PEhead.h"
#include "afxdialogex.h"
#include "MyCatalogue.h"
#include "MySection.h"
#include "MyRVAToOffset.h"
//#include <windows.h>


// PEhead 对话框

IMPLEMENT_DYNAMIC(PEhead, CDialogEx)

PEhead::PEhead(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_AddressOfEntryPoint(_T(""))
	, m_ImageBase(_T(""))
	, m_SizeOfImage(_T(""))
	, m_BaseOfCode(_T(""))
	, m_BaseOfData(_T(""))
	, m_SectionAlignment(_T(""))
	, m_FileAlignment(_T(""))
	, m_Magic(_T(""))
	, m_Subsystem(_T(""))
	, m_NumberOfSections(_T(""))
	, m_TimeDateStamp(_T(""))
	, m_SizeOfHeaders(_T(""))
	, m_Characteristics(_T(""))
	, m_CheckSum(_T(""))
	, m_SizeOfOptionalHeader(_T(""))
	, m_NumberOfRvaAndSizes(_T(""))
{

}

PEhead::~PEhead()
{
}

void PEhead::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT8, m_AddressOfEntryPoint);

	DDX_Text(pDX, IDC_EDIT9, m_ImageBase);
	DDX_Text(pDX, IDC_EDIT6, m_SizeOfImage);
	DDX_Text(pDX, IDC_EDIT7, m_BaseOfCode);
	DDX_Text(pDX, IDC_EDIT4, m_BaseOfData);
	DDX_Text(pDX, IDC_EDIT5, m_SectionAlignment);
	DDX_Text(pDX, IDC_EDIT2, m_FileAlignment);
	DDX_Text(pDX, IDC_EDIT3, m_Magic);
	DDX_Text(pDX, IDC_EDIT16, m_Subsystem);
	DDX_Text(pDX, IDC_EDIT17, m_NumberOfSections);
	DDX_Text(pDX, IDC_EDIT14, m_TimeDateStamp);
	DDX_Text(pDX, IDC_EDIT15, m_SizeOfHeaders);
	DDX_Text(pDX, IDC_EDIT12, m_Characteristics);
	DDX_Text(pDX, IDC_EDIT13, m_CheckSum);
	DDX_Text(pDX, IDC_EDIT10, m_SizeOfOptionalHeader);
	DDX_Text(pDX, IDC_EDIT11, m_NumberOfRvaAndSizes);
}


void PEhead::getPEHeaderInfo()
{

	//打开PE文件
	HANDLE hFile = INVALID_HANDLE_VALUE;

	hFile = CreateFile(path,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"文件无法打开!");

		ExitProcess(0);
	}
	//获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	//申请内存空间
	BYTE *pBuf = new BYTE[dwFileSize];

	//将文件内容读取到内存中
	DWORD dwRead = 0;
	ReadFile(hFile, pBuf, dwFileSize, &dwRead, NULL);

	//将缓冲区当成DOS头结构体来解析
	IMAGE_DOS_HEADER* pDosHdr = (IMAGE_DOS_HEADER*)pBuf;
	m_pDosHdr = pDosHdr;
	//NT头
	IMAGE_NT_HEADERS* pNtHdr = (IMAGE_NT_HEADERS*)(pDosHdr->e_lfanew + (DWORD)pBuf);

	//判断是否是一个有效的PE文件
	if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE || pNtHdr->Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(L"不是有效的PE文件");
		ExitProcess(0);
	}
	m_pFileHdr = &pNtHdr->FileHeader;
	m_pOptHdr = &pNtHdr->OptionalHeader;
	m_pOptHdr= &pNtHdr->OptionalHeader;
	//区段头表
	DWORD dwNtHeaderSize = sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + m_pFileHdr->SizeOfOptionalHeader;
	IMAGE_SECTION_HEADER* pScnHdr = NULL;
	pScnHdr = (IMAGE_SECTION_HEADER*)((DWORD)pNtHdr + dwNtHeaderSize);
	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);
	m_pScnHdr= IMAGE_FIRST_SECTION(pNtHdr);

	m_AddressOfEntryPoint.Format(L"%08X", m_pOptHdr->AddressOfEntryPoint);
	m_ImageBase.Format(L"%08X", m_pOptHdr->ImageBase);
	m_SizeOfImage.Format(L"%08X", m_pOptHdr->SizeOfImage);
	m_BaseOfCode.Format(L"%08X", m_pOptHdr->BaseOfCode);
	m_BaseOfData.Format(L"%08X", m_pOptHdr->BaseOfData);
	m_SectionAlignment.Format(L"%08X", m_pOptHdr->SectionAlignment);
	m_FileAlignment.Format(L"%08X", m_pOptHdr->FileAlignment);
	m_Magic.Format(L"%08X", m_pOptHdr->Magic);
	m_Subsystem.Format(L"%08X", m_pOptHdr->Subsystem);
	m_NumberOfSections.Format(L"%08X", m_pFileHdr->NumberOfSections);
	m_TimeDateStamp.Format(L"%08X", m_pFileHdr->TimeDateStamp);
	m_SizeOfHeaders.Format(L"%08X", m_pOptHdr->SizeOfHeaders);
	m_Characteristics.Format(L"%08X", m_pFileHdr->Characteristics);
	m_CheckSum.Format(L"%08X", m_pOptHdr->CheckSum);
	m_SizeOfOptionalHeader.Format(L"%08X", m_pFileHdr->SizeOfOptionalHeader);
	m_NumberOfRvaAndSizes.Format(L"%08X", m_pOptHdr->NumberOfRvaAndSizes);
	UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(PEhead, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &PEhead::OnCatalogue)
	ON_BN_CLICKED(IDC_BUTTON3, &PEhead::OnSection)
	ON_BN_CLICKED(IDC_BUTTON6, &PEhead::OnTime)
	ON_BN_CLICKED(IDC_BUTTON5, &PEhead::OnRVAToOffset)
END_MESSAGE_MAP()


// PEhead 消息处理程序


BOOL PEhead::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(TRUE);
	getPEHeaderInfo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void PEhead::OnCatalogue()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyCatalogue dialog;
	dialog.pOptHdr= m_pOptHdr;
	dialog.m_pDosHdr = m_pDosHdr;
	dialog.DoModal();
}


void PEhead::OnSection()
{
	// TODO: 在此添加控件通知处理程序代码
	CMySection se;
	se.m_pScnHdr = m_pScnHdr;
	se.m_pFileHdr = m_pFileHdr;
	se.DoModal();
}


void PEhead::OnTime()
{
	// TODO: 在此添加控件通知处理程序代码
	tm* FileTime = gmtime((time_t*)&m_pFileHdr->TimeDateStamp);

	CString time;
	time.Format(L"%d", FileTime->tm_year + 1900);
	time.Append(L"-");
	time.AppendFormat(L"%d", (FileTime->tm_mon + 1));
	time.Append(L"-");
	time.AppendFormat(L"%d", FileTime->tm_mday);
	time.Append(L"  ");
	time.AppendFormat(L"%d", FileTime->tm_hour);
	time.Append(L":");
	time.AppendFormat(L"%d", FileTime->tm_min);
	time.Append(L":");
	time.AppendFormat(L"%d", FileTime->tm_sec);
	MessageBox(time);

}


void PEhead::OnRVAToOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyRVAToOffset ro;
	ro.m_pDosHdr = m_pDosHdr;
	ro.m_pOptHdr = m_pOptHdr;
	ro.m_pScnHdr = m_pScnHdr;
	ro.DoModal();
}
