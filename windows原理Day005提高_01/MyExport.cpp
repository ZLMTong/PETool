// MyExport.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windowsԭ��Day005���_01.h"
#include "MyExport.h"
#include "afxdialogex.h"


// CMyExport �Ի���

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


// CMyExport ��Ϣ�������

//rva��Ӧ���ļ�ƫ��,���ת��ʧ��,����-1
DWORD CMyExport::RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva)
{
	IMAGE_SECTION_HEADER* pScnHdr;

	IMAGE_NT_HEADERS* pNtHdr =
		(IMAGE_NT_HEADERS*)(pDos->e_lfanew + (DWORD)pDos);

	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);//���α�ͷ
	DWORD dwNumberOfScn = pNtHdr->FileHeader.NumberOfSections;//���ε�����

	// 1. �������������ҵ���������
	for (int i = 0; i < dwNumberOfScn; ++i)
	{
		DWORD dwEndOfSection = pScnHdr[i].VirtualAddress + pScnHdr[i].SizeOfRawData;
		// �ж����RVA�Ƿ���һ�����εķ�Χ��
		if (dwRva >= pScnHdr[i].VirtualAddress
			&& dwRva < dwEndOfSection)
		{
			// 2. �����RVA�������ڵ�ƫ��:rva ��ȥ�׵�ַ
			DWORD dwOffset = dwRva - pScnHdr[i].VirtualAddress;
			// 3. ��������ƫ�Ƽ������ε��ļ���ʼƫ��
			return dwOffset + pScnHdr[i].PointerToRawData;
		}
	}
	return -1;
}

BOOL CMyExport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ExportMessage();
	ExportList();



	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CMyExport::ExportMessage()
{
	//�õ�RVA�ļ�ƫ��
	DWORD dwExpOfs = RVAToOffset(m_pDosHdr, m_dwExpRva);
	pExpTab = (IMAGE_EXPORT_DIRECTORY*)(dwExpOfs + (DWORD)m_pDosHdr);

	m_Characteristics.Format(L"%08X", pExpTab->Characteristics);//����ֵ

																//DLL��
	DWORD dwNameOfs = RVAToOffset(m_pDosHdr, pExpTab->Name);
	m_ExportOffset.Format(L"%08X", dwExpOfs);//�����ƫ��
	m_NameOffset.Format(L"%S", (char*)(dwNameOfs + (DWORD)m_pDosHdr));//�����ַ���
																	  //m_NameOffset = CString((char*)(dwNameOfs + (DWORD)m_pDosHdr));
	m_Base.Format(L"%08X", pExpTab->Base);//��ַ
	m_Name.Format(L"%08X", pExpTab->Name);//����
	m_NumberOfFunctions.Format(L"%08X", pExpTab->NumberOfFunctions);//��������
	m_NumberOfNames.Format(L"%08X", pExpTab->NumberOfNames);//����������
	m_AddressOfFunctions.Format(L"%08X", pExpTab->AddressOfFunctions);//������ַ��RVA
	m_AddressOfNames.Format(L"%08X", pExpTab->AddressOfNames);//�������Ƶ�ַ��RVA
	m_AddressOfNameOrdinals.Format(L"%08X", pExpTab->AddressOfNameOrdinals);//��ű��RVA
	UpdateData(FALSE);
}

void CMyExport::ExportList()
{
	// �������ű�
	DWORD dwExpAddrTabOfs = RVAToOffset(m_pDosHdr, pExpTab->AddressOfFunctions);
	DWORD dwExpNameTabOfs = RVAToOffset(m_pDosHdr, pExpTab->AddressOfNames);
	DWORD dwExpOrdTabOfs = RVAToOffset(m_pDosHdr, pExpTab->AddressOfNameOrdinals);

	// �����еĵ�ַ��,���ƶ���һ��DWORD��������
	DWORD* pExpAddr =
		(DWORD*)(dwExpAddrTabOfs + (DWORD)m_pDosHdr);

	DWORD* pExpName =
		(DWORD*)(dwExpNameTabOfs + (DWORD)m_pDosHdr);

	// ��ű���WORD���͵�����
	WORD* pExpOrd =
		(WORD*)(dwExpOrdTabOfs + (WORD)m_pDosHdr);



	CRect rc;
	m_ExportList.GetClientRect(&rc);//ָ����������С
	int nWidth = rc.Width();
	TCHAR* pcol[] = { L"���",L"RVA",L"ƫ��",L"������" };
	for (int i = 0; i < 4; i++)
	{
		m_ExportList.InsertColumn(i, pcol[i], 0, 100);
	}
	//���������
	int j = 0;
	CString str;
	for (int i = 0; i < pExpTab->NumberOfFunctions; i++)
	{
		//���
		str.Format(L"%03X", i + pExpTab->Base);
		m_ExportList.InsertItem(i, str);
		//������ַ��RVA
		str.Format(L"%08X", pExpAddr[i]);
		m_ExportList.SetItemText(i, ++j, str);
		//������ַ��ƫ��
		str.Format(L"%08X", RVAToOffset(m_pDosHdr,pExpAddr[i]));
		m_ExportList.SetItemText(i, ++j, str);
		//������
		DWORD dwNameRva = pExpName[i];
		DWORD dwNameOfs = RVAToOffset(m_pDosHdr, dwNameRva);
		char* pFunctionName = (char*)(dwNameOfs + (DWORD)m_pDosHdr);
		str.Format(L"%S", pFunctionName);
		m_ExportList.SetItemText(i, ++j, str);

		j = 0;
	}
}
