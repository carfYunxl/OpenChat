
// xads_PCClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// PCClientApp:
// �йش����ʵ�֣������ xads_PCClient.cpp
//

class PCClientApp : public CWinApp
{
public:
	PCClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern PCClientApp theApp;