
// windowsԭ��Day005���_01Dlg.h : ͷ�ļ�
//

#pragma once
#include "PELoadFileList.h"


// Cwindowsԭ��Day005���_01Dlg �Ի���
class Cwindowsԭ��Day005���_01Dlg : public CDialogEx
{
// ����
public:
	Cwindowsԭ��Day005���_01Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDOWSDAY005_01_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CPELoadFileList m_peHeatherList;
};
