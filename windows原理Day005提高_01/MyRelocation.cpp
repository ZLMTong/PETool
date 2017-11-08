// MyRelocation.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windowsԭ��Day005���_01.h"
#include "MyRelocation.h"
#include "afxdialogex.h"


// CMyRelocation �Ի���

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


// CMyRelocation ��Ϣ�������

DWORD CMyRelocation::RVAToOffset(LPVOID pDos, DWORD dwRva) {
	// �ҵ���Ҫת����RVA���ڵ�����,Ȼ�����ƫ��.
	IMAGE_DOS_HEADER* pDosHdr = (IMAGE_DOS_HEADER*)pDos;

	IMAGE_NT_HEADERS* pNtHdr =
		(IMAGE_NT_HEADERS*)((DWORD)pDos + pDosHdr->e_lfanew);

	IMAGE_SECTION_HEADER* pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);

	DWORD dwScnCount = pNtHdr->FileHeader.NumberOfSections;

	for (DWORD i = 0; i < dwScnCount; i++) {
		// �ж�RVA�Ƿ���һ��������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//����б���
	CRect rc;
	m_ReListA.GetClientRect(&rc);//ָ����������С
	int nWidth = rc.Width();
	TCHAR* pcol[] = { L"����", L"RVA", L"��Ŀ" };
	for (int i = 0; i < 3; i++)
	{
		m_ReListA.InsertColumn(i, pcol[i], 0, 100);
	}

	m_ReListB.GetClientRect(&rc);//ָ����������С
	TCHAR* pcolB[] = { L"����", L"RVA", L"ƫ��" ,L"����", L"FAR��ַ"};
	for (int i = 0; i < 5; i++)
	{
		m_ReListB.InsertColumn(i, pcolB[i], 0, 100);
	}
	GetRelovation();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CMyRelocation::GetRelovation()
{
	// ��ȡ�ض�λ��
	/*
	�ض�λ��ṹ��
	�ض�λ����һ����һ�����ϵ��ض�λ��ṹ�����
	�ض�λ����һ�����ض�λ��ṹ��ļ���(Ҳ����һ������),��������һ��ȫ0��Ԫ����Ϊ
	�����ı�־.
	typedef struct _IMAGE_BASE_RELOCATION {
	DWORD   VirtualAddress; // �����ʼRVA
	DWORD   SizeOfBlock;    // ������Ĵ�С(����VirtualAddress��SizeOfBlock�����ֶεĴ�С)
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
		// �õ��ض�λ��ĸ���
		DWORD dwCount = (pBaseRel->SizeOfBlock - (sizeof(IMAGE_BASE_RELOCATION))) / sizeof(WORD);

		//���
		str.Format(L"%d", nNum+1);
		m_ReListA.InsertItem(nNum, str);

		//��Ҫ�ض�λ��λ��RVA
		str.Format(L"%08X", pBaseRel->VirtualAddress);
		m_ReListA.SetItemText(nNum, ++line, str);

		//��Ŀ
		str.Format(L"%0Xh/%dd", dwCount,dwCount);
		m_ReListA.SetItemText(nNum, ++line, str);

		// �õ��ض�λ��Ŀ�ʼ��ַ
		TypeOffset* pTypeOffset = (TypeOffset*)(pBaseRel + 1);
		int lcow = 0;
		int lineB = 0;
		for (DWORD i = 0; i < dwCount; ++i)
		{

			if (pTypeOffset[i].Type == IMAGE_REL_BASED_HIGHLOW) {

				// �õ���Ҫ�޸���RVA
				DWORD dwRva = pBaseRel->VirtualAddress + pTypeOffset[i].Offset;

				DWORD dwOffset = RVAToOffset(m_pDosHdr, dwRva);

				// �õ���Ҫ�޸���opcode��ַ�����������ڵĵ�ַ
				DWORD* pAddress = (DWORD*)(dwOffset + (DWORD)m_pDosHdr);
				
				//���
				strB.Format(L"%d", lcow +1);
				m_ReListB.InsertItem(lcow, strB);
				
				//��Ҫ�ض�λ��λ��RVA
				strB.Format(L"%08X", pBaseRel->VirtualAddress + pTypeOffset[i].Offset);
				m_ReListB.SetItemText(lcow, ++lineB, strB);
				
				//ƫ��
				strB.Format(L"%08X", RVAToOffset(m_pDosHdr, pBaseRel->VirtualAddress + pTypeOffset[i].Offset));
				m_ReListB.SetItemText(lcow, ++lineB, strB);
				
				//����
				strB.Format(L"%s", L"HIGHLOW (3)");
				m_ReListB.SetItemText(lcow, ++lineB, strB);

				//VA
				strB.Format(L"%08X",*pAddress);
				m_ReListB.SetItemText(lcow, ++lineB, strB);
				lcow++;
				lineB = 0;
			}
		}

		// �ҵ���һ���ض�λ��
		pBaseRel = (IMAGE_BASE_RELOCATION*)((DWORD)pBaseRel + pBaseRel->SizeOfBlock);

		nNum++;
		line = 0;
	}
}
