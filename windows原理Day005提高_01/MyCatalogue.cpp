// MyCatalogue.cpp : 实现文件
//

#include "stdafx.h"
#include "windows原理Day005提高_01.h"
#include "MyCatalogue.h"
#include "afxdialogex.h"
#include "MyExport.h"
#include "MyImport.h"
#include "MyResource.h"
#include "MyTLSDialog.h"
#include "MyRelocation.h"


// CMyCatalogue 对话框

IMPLEMENT_DYNAMIC(CMyCatalogue, CDialogEx)

CMyCatalogue::CMyCatalogue(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_outputTableRva(_T(""))
	, m_exportSize(_T(""))
	, m_importRva(_T(""))
	, m_importSize(_T(""))
	, m_resourceRva(_T(""))
	, m_resourceSize(_T(""))
	, m_exceptionRva(_T(""))
	, m_exceptionSize(_T(""))
	, m_securityRva(_T(""))
	, m_securitySize(_T(""))
	, m_baserelocRva(_T(""))
	, m_baserelocSize(_T(""))
	, m_debugRva(_T(""))
	, m_debugSize(_T(""))
	, m_architectureRva(_T(""))
	, m_architectureSize(_T(""))
	, m_globalptrRva(_T(""))
	, m_globalptrSize(_T(""))
	, m_tlsRva(_T(""))
	, m_tlsSize(_T(""))
	, m_loadRva(_T(""))
	, m_loadSize(_T(""))
	, m_importBundRva(_T(""))
	, m_importBundSize(_T(""))
	, m_iatRva(_T(""))
	, m_iatSize(_T(""))
	, m_importDelayRva(_T(""))
	, m_importDelaySize(_T(""))
	, m_comRva(_T(""))
	, m_comSize(_T(""))
	, m_retainRva(_T(""))
	, m_retainSize(_T(""))
{

}

CMyCatalogue::~CMyCatalogue()
{
}

void CMyCatalogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT16, m_outputTableRva);
	DDX_Text(pDX, IDC_EDIT17, m_exportSize);
	DDX_Text(pDX, IDC_EDIT22, m_importRva);
	DDX_Text(pDX, IDC_EDIT23, m_importSize);
	DDX_Text(pDX, IDC_EDIT24, m_resourceRva);
	DDX_Text(pDX, IDC_EDIT25, m_resourceSize);
	DDX_Text(pDX, IDC_EDIT26, m_exceptionRva);
	DDX_Text(pDX, IDC_EDIT27, m_exceptionSize);
	DDX_Text(pDX, IDC_EDIT28, m_securityRva);
	DDX_Text(pDX, IDC_EDIT29, m_securitySize);
	DDX_Text(pDX, IDC_EDIT30, m_baserelocRva);
	DDX_Text(pDX, IDC_EDIT31, m_baserelocSize);
	DDX_Text(pDX, IDC_EDIT32, m_debugRva);
	DDX_Text(pDX, IDC_EDIT33, m_debugSize);
	DDX_Text(pDX, IDC_EDIT34, m_architectureRva);
	DDX_Text(pDX, IDC_EDIT35, m_architectureSize);
	DDX_Text(pDX, IDC_EDIT1, m_globalptrRva);
	DDX_Text(pDX, IDC_EDIT5, m_globalptrSize);
	DDX_Text(pDX, IDC_EDIT6, m_tlsRva);
	DDX_Text(pDX, IDC_EDIT7, m_tlsSize);
	DDX_Text(pDX, IDC_EDIT8, m_loadRva);
	DDX_Text(pDX, IDC_EDIT9, m_loadSize);
	DDX_Text(pDX, IDC_EDIT10, m_importBundRva);
	DDX_Text(pDX, IDC_EDIT11, m_importBundSize);
	DDX_Text(pDX, IDC_EDIT12, m_iatRva);
	DDX_Text(pDX, IDC_EDIT13, m_iatSize);
	DDX_Text(pDX, IDC_EDIT14, m_importDelayRva);
	DDX_Text(pDX, IDC_EDIT15, m_importDelaySize);
	DDX_Text(pDX, IDC_EDIT18, m_comRva);
	DDX_Text(pDX, IDC_EDIT19, m_comSize);
	DDX_Text(pDX, IDC_EDIT20, m_retainRva);
	DDX_Text(pDX, IDC_EDIT21, m_retainSize);
}


BEGIN_MESSAGE_MAP(CMyCatalogue, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyCatalogue::OnExport)
	ON_BN_CLICKED(IDC_BUTTON10, &CMyCatalogue::OnImport)
	ON_BN_CLICKED(IDC_BUTTON13, &CMyCatalogue::OnResource)
	ON_BN_CLICKED(IDC_BUTTON28, &CMyCatalogue::OnTLS)
	ON_BN_CLICKED(IDC_BUTTON19, &CMyCatalogue::OnRelocation)
END_MESSAGE_MAP()


// CMyCatalogue 消息处理程序
void CMyCatalogue::getDirctoryInfo()
{
	m_outputTableRva.Format(L"%08X", pOptHdr->DataDirectory[0].VirtualAddress);
	m_exportSize.Format(L"%08X", pOptHdr->DataDirectory[0].Size);

	m_importRva.Format(L"%08X", pOptHdr->DataDirectory[1].VirtualAddress);
	m_importSize.Format(L"%08X", pOptHdr->DataDirectory[1].Size);

	m_resourceRva.Format(L"%08X", pOptHdr->DataDirectory[2].VirtualAddress);
	m_resourceSize.Format(L"%08X", pOptHdr->DataDirectory[2].Size);

	m_exceptionRva.Format(L"%08X", pOptHdr->DataDirectory[3].VirtualAddress);
	m_exceptionSize.Format(L"%08X", pOptHdr->DataDirectory[3].Size);

	m_securityRva.Format(L"%08X", pOptHdr->DataDirectory[4].VirtualAddress);
	m_securitySize.Format(L"%08X", pOptHdr->DataDirectory[4].Size);

	m_baserelocRva.Format(L"%08X", pOptHdr->DataDirectory[5].VirtualAddress);
	m_baserelocSize.Format(L"%08X", pOptHdr->DataDirectory[5].Size);

	m_debugRva.Format(L"%08X", pOptHdr->DataDirectory[6].VirtualAddress);
	m_debugSize.Format(L"%08X", pOptHdr->DataDirectory[6].Size);

	m_architectureRva.Format(L"%08X", pOptHdr->DataDirectory[7].VirtualAddress);
	m_architectureSize.Format(L"%08X", pOptHdr->DataDirectory[7].Size);

	m_globalptrRva.Format(L"%08X", pOptHdr->DataDirectory[8].VirtualAddress);
	m_globalptrSize.Format(L"%08X", pOptHdr->DataDirectory[8].Size);

	m_tlsRva.Format(L"%08X", pOptHdr->DataDirectory[9].VirtualAddress);
	m_tlsSize.Format(L"%08X", pOptHdr->DataDirectory[9].Size);

	m_loadRva.Format(L"%08X", pOptHdr->DataDirectory[10].VirtualAddress);
	m_loadSize.Format(L"%08X", pOptHdr->DataDirectory[10].Size);

	m_importBundRva.Format(L"%08X", pOptHdr->DataDirectory[11].VirtualAddress);
	m_importBundSize.Format(L"%08X", pOptHdr->DataDirectory[11].Size);

	m_iatRva.Format(L"%08X", pOptHdr->DataDirectory[12].VirtualAddress);
	m_iatSize.Format(L"%08X", pOptHdr->DataDirectory[12].Size);

	m_importDelayRva.Format(L"%08X", pOptHdr->DataDirectory[13].VirtualAddress);
	m_importDelaySize.Format(L"%08X", pOptHdr->DataDirectory[13].Size);

	m_comRva.Format(L"%08X", pOptHdr->DataDirectory[14].VirtualAddress);
	m_comSize.Format(L"%08X", pOptHdr->DataDirectory[14].Size);

	m_retainRva.Format(L"%08X", pOptHdr->DataDirectory[15].VirtualAddress);
	m_retainSize.Format(L"%08X", pOptHdr->DataDirectory[15].Size);
}


BOOL CMyCatalogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	getDirctoryInfo();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMyCatalogue::OnExport()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExport Export;
	Export.m_dwExpRva = pOptHdr->DataDirectory[0].VirtualAddress;
	Export.m_pDosHdr = m_pDosHdr;
	Export.DoModal();
}


void CMyCatalogue::OnImport()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyImport Import;
	Import.m_dwImpRva = pOptHdr->DataDirectory[1].VirtualAddress;
	Import.m_pDosHdr = m_pDosHdr;
	Import.DoModal();
}


void CMyCatalogue::OnResource()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyResource Resource;
	Resource.m_dwResRootRva = pOptHdr->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
	Resource.m_pResRoot= (IMAGE_RESOURCE_DIRECTORY*)(Resource.m_dwResRootRva + (DWORD)m_pDosHdr);
	Resource.m_pDosHdr = m_pDosHdr;
	Resource.DoModal();
}


void CMyCatalogue::OnTLS()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyTLSDialog tls;
	tls.m_pDosHdr = m_pDosHdr;
	tls.dwTls = pOptHdr->DataDirectory[9].VirtualAddress;
	tls.DoModal();
}


void CMyCatalogue::OnRelocation()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyRelocation re;
	re.dwRetRva = pOptHdr->DataDirectory[5].VirtualAddress;
	re.m_pDosHdr = m_pDosHdr;
	re.DoModal();
}
