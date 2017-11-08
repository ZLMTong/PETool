
// windows原理Day005提高_01Dlg.h : 头文件
//

#pragma once
#include "PELoadFileList.h"


// Cwindows原理Day005提高_01Dlg 对话框
class Cwindows原理Day005提高_01Dlg : public CDialogEx
{
// 构造
public:
	Cwindows原理Day005提高_01Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDOWSDAY005_01_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CPELoadFileList m_peHeatherList;
};
