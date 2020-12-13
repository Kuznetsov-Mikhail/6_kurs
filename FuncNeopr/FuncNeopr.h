
// FuncNeopr.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CFuncNeoprApp:
// Сведения о реализации этого класса: FuncNeopr.cpp
//

class CFuncNeoprApp : public CWinApp
{
public:
	CFuncNeoprApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CFuncNeoprApp theApp;
