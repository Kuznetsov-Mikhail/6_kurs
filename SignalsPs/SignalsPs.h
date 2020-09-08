
// SignalsPs.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CSignalsPsApp:
// Сведения о реализации этого класса: SignalsPs.cpp
//

class CSignalsPsApp : public CWinApp
{
public:
	CSignalsPsApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CSignalsPsApp theApp;
