// MyResource.cpp : 实现文件
//

#include "stdafx.h"
#include "windows原理Day005提高_01.h"
#include "MyResource.h"
#include "afxdialogex.h"


// CMyResource 对话框

IMPLEMENT_DYNAMIC(CMyResource, CDialogEx)

CMyResource::CMyResource(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG6, pParent)
{

}

CMyResource::~CMyResource()
{
}

void CMyResource::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_ResourceTree);
}


BEGIN_MESSAGE_MAP(CMyResource, CDialogEx)
END_MESSAGE_MAP()


// CMyResource 消息处理程序
DWORD CMyResource::RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva)
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

BOOL CMyResource::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	IMAGE_RESOURCE_DIRECTORY* pResOfs = (IMAGE_RESOURCE_DIRECTORY*)(RVAToOffset(m_pDosHdr, m_dwResRootRva) + (DWORD)m_pDosHdr);
	parseResourcesTable((DWORD)pResOfs, pResOfs, 1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CMyResource::parseResourcesTable(DWORD dwResRootDirAddr,/*根目录的首地址*/
	IMAGE_RESOURCE_DIRECTORY* pResDir,/*需要解析的资源目录*/
	int nDeep /*记录这是第几层目录*/
	)
{
	//1. 根据目录是第几层,来获取目录入口中的ID信息
	/*
	+-----------------+
	|     资源目录     |
	+-----------------+
	+-------------+
	|  资源目录入口 |
	+-------------+
	|  资源目录入口 |
	+-------------+
	*/
	// 获取目录入口的总个数
	DWORD dwEntryCount = pResDir->NumberOfIdEntries + pResDir->NumberOfNamedEntries;

	// 获取目录入口数组的首地址
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pResDirEntry;
	pResDirEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pResDir + 1);

	WCHAR buff[512];


	CString str;
	if (nDeep == 1) { /*第一层*/

					  // 第一层目录入口,ID信息是资源类型的id
					  // 指向下一层的偏移,指向的是第二层的资源目录.
		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			// 解析资源的类型
			// 判断资源类型是整形的还是字符串类型的
			if (pResDirEntry[i].NameIsString == 1) {
				// 资源类型是字符串
				IMAGE_RESOURCE_DIR_STRING_U* pTypeName;
				// NameOffset 保存的偏移,是把资源根目录的地址作为基地址的偏移
				pTypeName =
					(IMAGE_RESOURCE_DIR_STRING_U*)
					(pResDirEntry[i].NameOffset + dwResRootDirAddr);

				wcsncpy_s(buff, pTypeName->NameString, pTypeName->Length);

				// 根节点 
				hTree1 = m_ResourceTree.InsertItem(buff, 0, 3, NULL, TVI_LAST);
			}
			else {
				// 说明资源类型ID是一个整形的值
				WCHAR *szType[] =
				{
					(L"") ,				// 0
					(L"光标") ,				// 1 
					(L"位图") ,				// 2 RT_BITMAP
					(L"图标") ,			    // 3 RT_ICON
					(L"菜单") ,				// 4 RT_MENU
					(L"对话框") ,			// 5 RT_DIALOG      
					(L"字符串表") ,			// 6 RT_STRING      
					(L"字体目录") ,			// 7 RT_FONTDIR     
					(L"字体") ,				// 8 RT_FONT        
					(L"加速键") ,			// 9 RT_ACCELERATOR 
					(L"RC数据") ,		// 10 RT_RCDATA      
					(L"消息表") ,			// 11 RT_MESSAGETABLE
					(L"光标组") ,			// 12 
					(L"") ,				   // 13 
					(L"图标组") ,			// 14 
					L"" ,					// 15
					(L"版本信息") ,			// 16
					(L"对话框包含目录") ,	// 17 #define RT_DLGINCLUDE   MAKEINTRESOURCE(17)
					L"" ,								// 18 #define RT_PLUGPLAY     MAKEINTRESOURCE(19)
					L"" ,								// 19 #define RT_VXD          MAKEINTRESOURCE(20)
					L"" ,								// 20 #define RT_ANICURSOR    MAKEINTRESOURCE(21)
					L"" ,								// 21 #define RT_ANIICON      MAKEINTRESOURCE(22)
					L"" ,									// 22 
					(L"HTML") ,						    // 23 #define RT_HTML         MAKEINTRESOURCE(23)
					(L"清单文件")			// 24 RT_MANIFEST
				};

				if (pResDirEntry[i].Id >= 1 && pResDirEntry[i].Id <= 24) {
					str.Format(L"%s", szType[pResDirEntry[i].Id]);
					hTree1 = m_ResourceTree.InsertItem(str, 0, 3, NULL, TVI_LAST);
				}
				else {
					str.Format(L"%d", pResDirEntry[i].Id);
					hTree1 = m_ResourceTree.InsertItem(str, 0, 3, NULL, TVI_LAST);
				}
			}

			// 解析下一层目录
			IMAGE_RESOURCE_DIRECTORY* pNextDir =
				(IMAGE_RESOURCE_DIRECTORY*)(pResDirEntry[i].OffsetToDirectory + dwResRootDirAddr);//子目录偏移地址+根目录首地址

			parseResourcesTable(dwResRootDirAddr, pNextDir, nDeep + 1);
		}
	}
	else if (nDeep == 2) /*第二层*/
	{ 

		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			// 资源目录的第二层, 保存的是每种资源的资源id
			// 解析ID
			// 1. 整形ID
			// 2. 字符串ID
			if (pResDirEntry[i].NameIsString) {
				// 资源类型是字符串
				IMAGE_RESOURCE_DIR_STRING_U* pTypeName;
				// NameOffset 保存的偏移,是把资源根目录的地址作为基地址的偏移
				pTypeName =
					(IMAGE_RESOURCE_DIR_STRING_U*)
					(pResDirEntry[i].NameOffset + dwResRootDirAddr);

				wcsncpy_s(buff, pTypeName->NameString, pTypeName->Length);
				hTree2 = m_ResourceTree.InsertItem(buff, 1, 3, hTree1, TVI_LAST);
			}
			else {
				str.Format(L"%d", pResDirEntry[i].Id);
				hTree2 = m_ResourceTree.InsertItem(str, 1, 3, hTree1, TVI_LAST);

			}

			// 解析偏移
			// 解析下一层目录
			IMAGE_RESOURCE_DIRECTORY* pNextDir =
				(IMAGE_RESOURCE_DIRECTORY*)(pResDirEntry[i].OffsetToDirectory + dwResRootDirAddr);

			parseResourcesTable(dwResRootDirAddr, pNextDir, nDeep + 1);
		}
	}
	else if (nDeep == 3) {/*第三层*/

						  // 第三层资源目录入口, 保存的是资源的数据入口
						  // 解析偏移(偏移是一个指向数据入口的偏移)
		IMAGE_RESOURCE_DATA_ENTRY* pResDataEntry = 0;

		if (pResDirEntry->DataIsDirectory == 0) {

			pResDataEntry = (IMAGE_RESOURCE_DATA_ENTRY*)(pResDirEntry->OffsetToData + dwResRootDirAddr);

			CString str;
			str.Format(L"RVA:%08X", pResDataEntry->OffsetToData);
			m_ResourceTree.InsertItem(str, 1, 3, hTree2, TVI_LAST);

			str.Format(L"偏移:%08X", RVAToOffset(m_pDosHdr ,pResDataEntry->OffsetToData));
			m_ResourceTree.InsertItem(str, 1, 3, hTree2, TVI_LAST);

			str.Format(L"大小:%08X", pResDataEntry->Size);
			m_ResourceTree.InsertItem(str, 1, 3, hTree2, TVI_LAST);
		}
	}
}

