// MyResource.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windowsԭ��Day005���_01.h"
#include "MyResource.h"
#include "afxdialogex.h"


// CMyResource �Ի���

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


// CMyResource ��Ϣ�������
DWORD CMyResource::RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva)
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

BOOL CMyResource::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	IMAGE_RESOURCE_DIRECTORY* pResOfs = (IMAGE_RESOURCE_DIRECTORY*)(RVAToOffset(m_pDosHdr, m_dwResRootRva) + (DWORD)m_pDosHdr);
	parseResourcesTable((DWORD)pResOfs, pResOfs, 1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CMyResource::parseResourcesTable(DWORD dwResRootDirAddr,/*��Ŀ¼���׵�ַ*/
	IMAGE_RESOURCE_DIRECTORY* pResDir,/*��Ҫ��������ԴĿ¼*/
	int nDeep /*��¼���ǵڼ���Ŀ¼*/
	)
{
	//1. ����Ŀ¼�ǵڼ���,����ȡĿ¼����е�ID��Ϣ
	/*
	+-----------------+
	|     ��ԴĿ¼     |
	+-----------------+
	+-------------+
	|  ��ԴĿ¼��� |
	+-------------+
	|  ��ԴĿ¼��� |
	+-------------+
	*/
	// ��ȡĿ¼��ڵ��ܸ���
	DWORD dwEntryCount = pResDir->NumberOfIdEntries + pResDir->NumberOfNamedEntries;

	// ��ȡĿ¼���������׵�ַ
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pResDirEntry;
	pResDirEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pResDir + 1);

	WCHAR buff[512];


	CString str;
	if (nDeep == 1) { /*��һ��*/

					  // ��һ��Ŀ¼���,ID��Ϣ����Դ���͵�id
					  // ָ����һ���ƫ��,ָ����ǵڶ������ԴĿ¼.
		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			// ������Դ������
			// �ж���Դ���������εĻ����ַ������͵�
			if (pResDirEntry[i].NameIsString == 1) {
				// ��Դ�������ַ���
				IMAGE_RESOURCE_DIR_STRING_U* pTypeName;
				// NameOffset �����ƫ��,�ǰ���Դ��Ŀ¼�ĵ�ַ��Ϊ����ַ��ƫ��
				pTypeName =
					(IMAGE_RESOURCE_DIR_STRING_U*)
					(pResDirEntry[i].NameOffset + dwResRootDirAddr);

				wcsncpy_s(buff, pTypeName->NameString, pTypeName->Length);

				// ���ڵ� 
				hTree1 = m_ResourceTree.InsertItem(buff, 0, 3, NULL, TVI_LAST);
			}
			else {
				// ˵����Դ����ID��һ�����ε�ֵ
				WCHAR *szType[] =
				{
					(L"") ,				// 0
					(L"���") ,				// 1 
					(L"λͼ") ,				// 2 RT_BITMAP
					(L"ͼ��") ,			    // 3 RT_ICON
					(L"�˵�") ,				// 4 RT_MENU
					(L"�Ի���") ,			// 5 RT_DIALOG      
					(L"�ַ�����") ,			// 6 RT_STRING      
					(L"����Ŀ¼") ,			// 7 RT_FONTDIR     
					(L"����") ,				// 8 RT_FONT        
					(L"���ټ�") ,			// 9 RT_ACCELERATOR 
					(L"RC����") ,		// 10 RT_RCDATA      
					(L"��Ϣ��") ,			// 11 RT_MESSAGETABLE
					(L"�����") ,			// 12 
					(L"") ,				   // 13 
					(L"ͼ����") ,			// 14 
					L"" ,					// 15
					(L"�汾��Ϣ") ,			// 16
					(L"�Ի������Ŀ¼") ,	// 17 #define RT_DLGINCLUDE   MAKEINTRESOURCE(17)
					L"" ,								// 18 #define RT_PLUGPLAY     MAKEINTRESOURCE(19)
					L"" ,								// 19 #define RT_VXD          MAKEINTRESOURCE(20)
					L"" ,								// 20 #define RT_ANICURSOR    MAKEINTRESOURCE(21)
					L"" ,								// 21 #define RT_ANIICON      MAKEINTRESOURCE(22)
					L"" ,									// 22 
					(L"HTML") ,						    // 23 #define RT_HTML         MAKEINTRESOURCE(23)
					(L"�嵥�ļ�")			// 24 RT_MANIFEST
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

			// ������һ��Ŀ¼
			IMAGE_RESOURCE_DIRECTORY* pNextDir =
				(IMAGE_RESOURCE_DIRECTORY*)(pResDirEntry[i].OffsetToDirectory + dwResRootDirAddr);//��Ŀ¼ƫ�Ƶ�ַ+��Ŀ¼�׵�ַ

			parseResourcesTable(dwResRootDirAddr, pNextDir, nDeep + 1);
		}
	}
	else if (nDeep == 2) /*�ڶ���*/
	{ 

		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			// ��ԴĿ¼�ĵڶ���, �������ÿ����Դ����Դid
			// ����ID
			// 1. ����ID
			// 2. �ַ���ID
			if (pResDirEntry[i].NameIsString) {
				// ��Դ�������ַ���
				IMAGE_RESOURCE_DIR_STRING_U* pTypeName;
				// NameOffset �����ƫ��,�ǰ���Դ��Ŀ¼�ĵ�ַ��Ϊ����ַ��ƫ��
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

			// ����ƫ��
			// ������һ��Ŀ¼
			IMAGE_RESOURCE_DIRECTORY* pNextDir =
				(IMAGE_RESOURCE_DIRECTORY*)(pResDirEntry[i].OffsetToDirectory + dwResRootDirAddr);

			parseResourcesTable(dwResRootDirAddr, pNextDir, nDeep + 1);
		}
	}
	else if (nDeep == 3) {/*������*/

						  // ��������ԴĿ¼���, ���������Դ���������
						  // ����ƫ��(ƫ����һ��ָ��������ڵ�ƫ��)
		IMAGE_RESOURCE_DATA_ENTRY* pResDataEntry = 0;

		if (pResDirEntry->DataIsDirectory == 0) {

			pResDataEntry = (IMAGE_RESOURCE_DATA_ENTRY*)(pResDirEntry->OffsetToData + dwResRootDirAddr);

			CString str;
			str.Format(L"RVA:%08X", pResDataEntry->OffsetToData);
			m_ResourceTree.InsertItem(str, 1, 3, hTree2, TVI_LAST);

			str.Format(L"ƫ��:%08X", RVAToOffset(m_pDosHdr ,pResDataEntry->OffsetToData));
			m_ResourceTree.InsertItem(str, 1, 3, hTree2, TVI_LAST);

			str.Format(L"��С:%08X", pResDataEntry->Size);
			m_ResourceTree.InsertItem(str, 1, 3, hTree2, TVI_LAST);
		}
	}
}

