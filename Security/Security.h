﻿
// Security.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CSecurityApp:
// Сведения о реализации этого класса: Security.cpp
//

class CSecurityApp : public CWinApp
{
public:
	CSecurityApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CSecurityApp theApp;
