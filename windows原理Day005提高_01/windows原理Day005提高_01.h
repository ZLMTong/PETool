
// windowsԭ��Day005���_01.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cwindowsԭ��Day005���_01App: 
// �йش����ʵ�֣������ windowsԭ��Day005���_01.cpp
//

class Cwindowsԭ��Day005���_01App : public CWinApp
{
public:
	Cwindowsԭ��Day005���_01App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cwindowsԭ��Day005���_01App theApp;