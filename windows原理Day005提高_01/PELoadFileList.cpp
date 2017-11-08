// PELoadFileList.cpp : 实现文件
//

#include "stdafx.h"
#include "windows原理Day005提高_01.h"
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



// CPELoadFileList 消息处理程序




void CPELoadFileList::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TCHAR szPath[MAX_PATH] = {};
	// 统计拖拽的文件夹的数目
	int nCount = DragQueryFile(hDropInfo, -1, szPath, MAX_PATH);
	// 分别获取每个文件夹的路径
	for (int i = 0; i < nCount; i++)
	{
		DragQueryFile(hDropInfo, i, szPath, MAX_PATH);
		if (LB_ERR == FindString(0, szPath))
		{// 如果m_ListBox里面没有这个新的文件夹路径，再添加进m_ListBox
			AddString(szPath);
		}
		PEhead dialog;
		dialog.path = szPath;
		dialog.DoModal();

	}


	CListBox::OnDropFiles(hDropInfo);
}
