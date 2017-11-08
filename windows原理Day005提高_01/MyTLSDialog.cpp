// MyTLSDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windowsԭ��Day005���_01.h"
#include "MyTLSDialog.h"
#include "afxdialogex.h"


// CMyTLSDialog �Ի���

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


// CMyTLSDialog ��Ϣ�������

//rva��Ӧ���ļ�ƫ��,���ת��ʧ��,����-1
DWORD CMyTLSDialog::RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva)
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


BOOL CMyTLSDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

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
				  // �쳣: OCX ����ҳӦ���� FALSE
}
