#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"

LRESULT CMainDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);
	SetDlgItemText(IDC_STATE, L"");
	::EnableWindow(GetDlgItem(IDC_BTN_REMOVE_SIGN), FALSE);

	return TRUE;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCtlColorEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	return (LRESULT)hBrush;
}

LRESULT CMainDlg::OnBnClickedBtnSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	TCHAR szFileName[MAX_PATH] = {};
	OPENFILENAMEW openFileName = {};
	openFileName.lStructSize = sizeof(OPENFILENAMEW);
	openFileName.nMaxFile = MAX_PATH;
	openFileName.hwndOwner = this->m_hWnd;
	openFileName.lpstrFilter = L"PE Files (*.exe;*.dll;*.sys)\0*.exe;*.dll;*.sys\0All Files (*.*)\0*.*\0";
	openFileName.lpstrFile = szFileName;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&openFileName))
	{
		SetDlgItemText(IDC_PEFILE_NOSIGN, openFileName.lpstrFile);
	}

	return 0;
}

LRESULT CMainDlg::OnBnClickedBtnSelect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	TCHAR szFileName[MAX_PATH] = {};
	OPENFILENAMEW openFileName = {};
	openFileName.lStructSize = sizeof(OPENFILENAMEW);
	openFileName.nMaxFile = MAX_PATH;
	openFileName.hwndOwner = this->m_hWnd;
	openFileName.lpstrFilter = L"PE Files (*.exe;*.dll;*.sys)\0*.exe;*.dll;*.sys\0All Files (*.*)\0*.*\0";
	openFileName.lpstrFile = szFileName;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&openFileName))
	{
		SetDlgItemText(IDC_PEFILE, openFileName.lpstrFile);
	}

	return 0;
}

LRESULT CMainDlg::OnEnChange(WORD, WORD wID, HANDLE hEdit, BOOL& bHandled)
{
	if (IDC_PEFILE == wID)
	{
		HWND hEditNoSign = GetDlgItem(IDC_PEFILE_NOSIGN);
		WCHAR wzFile[MAX_PATH] = {0}, tmp[MAX_PATH] = {0};
		GetDlgItemText(IDC_PEFILE, wzFile, _countof(wzFile));
		DWORD dwFileAttribute = GetFileAttributesW(wzFile);

		if (INVALID_FILE_ATTRIBUTES != dwFileAttribute &&
			!(FILE_ATTRIBUTE_DIRECTORY & dwFileAttribute))
		{
			LARGE_INTEGER liFileSize;
			LPVOID p = MapPeFile(wzFile, &liFileSize);

			if (IsInvalidPe(p))
			{
				SetDlgItemText(IDC_STATE, TEXT_INVALID);
				SetDlgItemText(IDC_PEFILE, L"");
				SetDlgItemText(IDC_PEFILE_NOSIGN, L"");
				::EnableWindow(GetDlgItem(IDC_BTN_REMOVE_SIGN), FALSE);
				bHandled = TRUE;
			}
			else
			{
				wcsncpy_s(tmp, wzFile, wcslen(wzFile) - 4);
				wcscpy_s(wzFile, tmp);
				wcscat_s(wzFile, L".nosign.exe");
				SetDlgItemText(IDC_PEFILE_NOSIGN, wzFile);
				SetDlgItemText(IDC_STATE, TEXT_READY);
				::EnableWindow(GetDlgItem(IDC_BTN_REMOVE_SIGN), TRUE);
			}

			UnmapPeFile(p);
		}
		else
		{
			SetDlgItemText(IDC_PEFILE_NOSIGN, L"");
			::EnableWindow(GetDlgItem(IDC_BTN_REMOVE_SIGN), FALSE);
		}

		bHandled = FALSE;
	}

	return 0;
}

LRESULT CMainDlg::OnBnClickedBtnRemoveSign(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	WCHAR wzFile[MAX_PATH] = { 0 };
	WCHAR wzNewFile[MAX_PATH] = { 0 };

	GetDlgItemText(IDC_PEFILE, wzFile, _countof(wzFile));
	GetDlgItemText(IDC_PEFILE_NOSIGN, wzNewFile, _countof(wzNewFile));

	LARGE_INTEGER liFileSize;
	LPVOID p = MapPeFile(wzFile, &liFileSize);
	DWORD dwOffset, dwSize;

	if (!GetPeSignOffsetAndSize(p, &dwOffset, &dwSize))
	{
		SetDlgItemText(IDC_STATE, TEXT_NOSIGN);
		goto _exit;
	}

	if (RemovePeSign(p, &liFileSize, wzNewFile))
		SetDlgItemText(IDC_STATE, TEXT_SUCCESS);
	else
		SetDlgItemText(IDC_STATE, TEXT_FAIL);

_exit:
	UnmapPeFile(p);
	return 0;
}