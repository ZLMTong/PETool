// MySection.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windowsԭ��Day005���_01.h"
#include "MySection.h"
#include "afxdialogex.h"


// CMySection �Ի���

IMPLEMENT_DYNAMIC(CMySection, CDialogEx)

CMySection::CMySection(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CMySection::~CMySection()
{
}

void CMySection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_SectionList);
}


BEGIN_MESSAGE_MAP(CMySection, CDialogEx)
END_MESSAGE_MAP()


// CMySection ��Ϣ�������


BOOL CMySection::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//����б���
	
	m_SectionList.GetClientRect(&rc);//ָ����������С
	int nWidth = rc.Width();
	TCHAR* pcol[] = { L"����",L"VOffset",L"VSize",L"ROffset",L"RSize",L"��־" };
	for (int i = 0; i < 6; i++)
	{
		m_SectionList.InsertColumn(i, pcol[i], 0, 80);
	}
	//���������
	int j = 0;
	CString str;
	for (int i = 0; i < m_pFileHdr->NumberOfSections; i++)
	{
		
		str.Format(L"%S", m_pScnHdr[i].Name);
		m_SectionList.InsertItem(i, str);

		str.Format(L"%08X", m_pScnHdr[i].VirtualAddress);
		m_SectionList.SetItemText(i, ++j, str);

		str.Format(L"%08X", m_pScnHdr[i].SizeOfRawData);
		m_SectionList.SetItemText(i, ++j, str);

		str.Format(L"%08X", m_pScnHdr[i].PointerToRawData);
		m_SectionList.SetItemText(i, ++j, str);

		str.Format(L"%08X", m_pScnHdr[i].Misc.VirtualSize);
		m_SectionList.SetItemText(i, ++j, str);

		str.Format(L"%08X", m_pScnHdr[i].Characteristics);
		m_SectionList.SetItemText(i, ++j, str);

		j = 0;
	}
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
