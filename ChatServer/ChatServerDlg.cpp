#include "stdafx.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"
#include "afxdialogex.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL CChatServerDlg::Log(HWND hWnd, WPARAM wParam, CChatClientContext *cccc)
{
	return ::PostMessageW(hWnd, WM_UPDATE_STATIC, wParam, (LPARAM)cccc);
}

BOOL CChatServerDlg::Log(HWND hWnd, WPARAM wParam, LPCWSTR str)
{
	int len = wcslen(str) + 1;
	LPWSTR logs = new wchar_t[len];
	wcscpy_s(logs, len, str);
	OutputDebugStringW(logs);
	OutputDebugStringW(L"\n");
	return ::PostMessageW(hWnd, WM_UPDATE_STATIC, wParam, (LPARAM)logs);
}

BOOL CChatServerDlg::Log(HWND hWnd, WPARAM wParam, LPCWSTR str, PCSTR wtf)
{
	wchar_t buffer[300];
	swprintf(buffer, 300, L"%s%hs", str, wtf);
	return Log(hWnd, wParam, buffer);
}

BOOL CChatServerDlg::Log(HWND hWnd, WPARAM wParam, LPCWSTR str, LPCWSTR wtf)
{
	wchar_t buffer[300];
	swprintf(buffer, 300, L"%s%s", str, wtf);
	return Log(hWnd, wParam, buffer);
}

BOOL CChatServerDlg::Log(HWND hWnd, WPARAM wParam, LPCWSTR str, sockaddr_in wtf)
{
	wchar_t buffer[300];
	swprintf(buffer, 300, L"%s%hs:%hu", str, inet_ntoa(wtf.sin_addr), wtf.sin_port);
	return Log(hWnd, wParam, buffer);
}

BOOL CChatServerDlg::Log(HWND hWnd, WPARAM wParam, LPCWSTR str, DWORD data)
{
	LPVOID lpMsgBuf;
	wchar_t buffer[300];
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, data, 0x0409, (LPWSTR)&lpMsgBuf, 0, NULL);
	swprintf(buffer, 300, L"%s :: 0x%08x - %s", str, data, (LPWSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
	return Log(hWnd, wParam, buffer);
}

CChatServerDlg::CChatServerDlg(CWnd* pParent)
	: CDialog(IDD_CHATSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CChatServerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CChatServerDlg::OnBnClickedOk)
	ON_MESSAGE(WM_UPDATE_STATIC, &CChatServerDlg::OnUpdateStatic)
	ON_BN_CLICKED(IDC_BUTTON2, &CChatServerDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL CChatServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	Context.ServerThread = NULL;
	Context.Window = m_hWnd;
	PasswordBox = (CEdit*)GetDlgItem(IDC_EDIT1);
	LogBox = (CListBox*)GetDlgItem(IDC_LIST1);
	BtnOk = (CButton*)GetDlgItem(IDOK);
	BtnStop = (CButton*)GetDlgItem(IDC_BUTTON2);
	UserListBox = (CListCtrl*)GetDlgItem(IDC_LIST2);
	UserListBox->InsertColumn(0, L"编号", 0, 100);
	UserListBox->InsertColumn(1, L"昵称", 0, 200);
	UserListBox->InsertColumn(2, L"IP地址", 0, 200);
	UserListBox->InsertColumn(3, L"状态", 0, 100);

	return TRUE;
}

void CChatServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CChatServerDlg::OnUpdateStatic(WPARAM wParam, LPARAM lParam)
{
	CChatClientContext *p; LPWSTR str;

	switch (wParam)
	{
	case 4:
		p = (CChatClientContext*)lParam;
		UserListBox->SetItemText(p->ID, 3, L"离线");
		break;

	case 3:
		p = (CChatClientContext*)lParam;
		str = new wchar_t[5];
		swprintf(str, 5, L"%d", p->ID);
		UserListBox->InsertItem(p->ID, str);
		UserListBox->SetItemText(p->ID, 1, p->Nickname);
		str = new wchar_t[25];
		swprintf(str, 25, L"%hs:%hu", inet_ntoa(p->ClientAddr.sin_addr), p->ClientAddr.sin_port);
		UserListBox->SetItemText(p->ID, 2, str);
		UserListBox->SetItemText(p->ID, 3, L"在线");
		break;

	case 2:
		PasswordBox->EnableWindow(TRUE);
		BtnOk->EnableWindow(TRUE);
		BtnStop->EnableWindow(FALSE);
		LogBox->AddString((LPWSTR)lParam);
		break;
	case 0:
		PasswordBox->EnableWindow(FALSE);
		BtnOk->EnableWindow(FALSE);
		BtnStop->EnableWindow(TRUE);
		LogBox->AddString((LPWSTR)lParam);
		break;

	case 1:
		LogBox->AddString((LPWSTR)lParam);
		break;

	default:
		break;
	}

	return 0;
}

void CChatServerDlg::OnBnClickedCancel()
{
	OnBnClickedButton2();
	CDialog::OnCancel();
}
