// PELoadFileList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windowsԭ��Day005���_01.h"
#include "PELoadFileList.h"
#include "PEhead.h"


// CPELoadFileList

IMPLEMENT_DYNAMIC(CPELoadFileList, CListBox)

CPELoadFileList::CPELoadFileList()
{

}

CPELoadFileList::~CPELoadFileList()
{
}


BEGIN_MESSAGE_MAP(CPELoadFileList, CListBox)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CPELoadFileList ��Ϣ�������




void CPELoadFileList::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TCHAR szPath[MAX_PATH] = {};
	// ͳ����ק���ļ��е���Ŀ
	int nCount = DragQueryFile(hDropInfo, -1, szPath, MAX_PATH);
	// �ֱ��ȡÿ���ļ��е�·��
	for (int i = 0; i < nCount; i++)
	{
		DragQueryFile(hDropInfo, i, szPath, MAX_PATH);
		if (LB_ERR == FindString(0, szPath))
		{// ���m_ListBox����û������µ��ļ���·��������ӽ�m_ListBox
			AddString(szPath);
		}
		PEhead dialog;
		dialog.path = szPath;
		dialog.DoModal();

	}


	CListBox::OnDropFiles(hDropInfo);
}
