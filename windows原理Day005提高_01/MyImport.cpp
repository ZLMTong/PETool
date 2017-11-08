// MyImport.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windowsԭ��Day005���_01.h"
#include "MyImport.h"
#include "afxdialogex.h"


// CMyImport �Ի���

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

void CMyImport::ImportListA()
{
	//�����ƫ��
	m_pImpArray = (IMAGE_IMPORT_DESCRIPTOR*)(RVAToOffset(m_pDosHdr, m_dwImpRva) + (DWORD)m_pDosHdr);
	
	
	m_ImportListA.GetClientRect(&rc);//ָ����������С
	int nWidth = rc.Width();
	TCHAR* pcol[] = { L"DLL����",L"OriginalFirstThunk",L"����ʱ���־",L"ForwarderChain",L"����", L"FirstThunk" };
	for (int i = 0; i < 6; i++)
	{
		m_ImportListA.InsertColumn(i, pcol[i], 0, 120);
	}
	//���������
	int j = 0;
	CString str;
	for (int i = 0; m_pImpArray->Name != 0; i++)
	{
		//DLL����
		DWORD dwNameOfs = RVAToOffset(m_pDosHdr, m_pImpArray->Name);
		char* pDllNmae = (char*)(dwNameOfs + (DWORD)m_pDosHdr);
		str.Format(L"%S", pDllNmae);
		m_ImportListA.InsertItem(i, str);
		//�������Ʊ��RVA
		str.Format(L"%08X", m_pImpArray->OriginalFirstThunk);
		m_ImportListA.SetItemText(i, ++j, str);
		//ʱ��
		str.Format(L"%08X", m_pImpArray->TimeDateStamp);
		m_ImportListA.SetItemText(i, ++j, str);
		//ת����
		str.Format(L"%08X", m_pImpArray->ForwarderChain);
		m_ImportListA.SetItemText(i, ++j, str);
		//NAME��RVA
		str.Format(L"%08X", m_pImpArray->Name);
		m_ImportListA.SetItemText(i, ++j, str);
		//�����ַ���RVA
		str.Format(L"%08X", m_pImpArray->FirstThunk);
		m_ImportListA.SetItemText(i, ++j, str);

		j = 0;
		m_pImpArray++;
	}
}

void CMyImport::ImportListB()
{

	//�����ƫ��
	m_pImpArray = (IMAGE_IMPORT_DESCRIPTOR*)(RVAToOffset(m_pDosHdr, m_dwImpRva) + (DWORD)m_pDosHdr);
	m_ImportListB.GetClientRect(&rc);//ָ����������С
	int nWidth = rc.Width();
	TCHAR* pcol[] = { L"ThunkRVA",L"Thunkƫ��",L"Thunkֵ",L"��ʾ",L"API����" };
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
	
		//���������
		int j = 0;
		int add = 0;
		CString str;
		for (int i = 0; pInt->u1.Function != NULL; i++)
		{	
			//���溯�����ƵĽṹ��
			DWORD dwImpNameOfs = RVAToOffset(m_pDosHdr, pInt->u1.Function);//���뺯���ĵ�ַ
			IMAGE_IMPORT_BY_NAME* pImpName = (IMAGE_IMPORT_BY_NAME*)(dwImpNameOfs + (DWORD)m_pDosHdr);

			//ThunkRVA
			str.Format(L"%08X", m_pImpArray->OriginalFirstThunk + add * 4);
			m_ImportListB.InsertItem(i, str);
			//Thunkƫ��
			str.Format(L"%08X", RVAToOffset(m_pDosHdr, m_pImpArray->FirstThunk) + add * 4);
			m_ImportListB.SetItemText(i, ++j, str);
			//Thunkֵ
			str.Format(L"%08X", pInt->u1.Function);
			m_ImportListB.SetItemText(i, ++j, str);
			//���������
			str.Format(L"%04X", pImpName->Hint);
			m_ImportListB.SetItemText(i, ++j, str);
			//��������
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


// CMyImport ��Ϣ�������


BOOL CMyImport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ImportListA();
	ImportListB()
		;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
