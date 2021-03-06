#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"
#include "../ChatServer/datagram.h"

#define WM_UPDATE_STATIC (WM_USER + 100)

class CChatClientApp : public CWinApp
{
public:
	CChatClientApp();
	SOCKET ClientSocket;
	CWinThread *Thread;
	static UINT ClientThread(LPVOID);
	msgqueue MessageQueue;
	CEdit *hWnd;
	wchar_t Password[17];
	wchar_t Nickname[21];
	int CurrentId;
	DWORD TargetIP;
	BOOL Log(WPARAM wParam, LPCWSTR a);
	BOOL Log(WPARAM wParam, LPCWSTR a, PCSTR v);
	BOOL Log(WPARAM wParam, LPCWSTR a, LPCWSTR v);
	BOOL Log(WPARAM wParam, LPCWSTR a, DWORD b);
	BOOL Log(WPARAM wParam, LPCWSTR str, sockaddr_in wtf);

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CChatClientApp theApp;
