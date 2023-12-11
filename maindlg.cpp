#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"

LRESULT CMainDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
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

	DragAcceptFiles(TRUE);

	return TRUE;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	// TODO: Add validation code
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCtlColorEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	return (LRESULT)hBrush;
}

LRESULT CMainDlg::OnBnClickedBtnSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled)
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

LRESULT CMainDlg::OnBnClickedBtnSelect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled)
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

BOOL IsValidPE(LPCTSTR filePath)
{
	HANDLE hFile;
	DWORD bytesRead;
	IMAGE_DOS_HEADER dosHeader;
	IMAGE_NT_HEADERS ntHeaders;

	// Open file
	hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	// Read & check DOS header
	if (!ReadFile(hFile, &dosHeader, sizeof(IMAGE_DOS_HEADER), &bytesRead, NULL) || bytesRead != sizeof(IMAGE_DOS_HEADER))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	// Locate and read NT header
	SetFilePointer(hFile, dosHeader.e_lfanew, NULL, FILE_BEGIN);
	if (!ReadFile(hFile, &ntHeaders, sizeof(IMAGE_NT_HEADERS), &bytesRead, NULL) || bytesRead != sizeof(IMAGE_NT_HEADERS))
	{
		CloseHandle(hFile); // failed to locate
		return FALSE;
	}
	if (ntHeaders.Signature != IMAGE_NT_SIGNATURE)
	{
		CloseHandle(hFile); // not a valid NT header
		return FALSE;
	}

	CloseHandle(hFile);
	return TRUE;
}

LRESULT CMainDlg::OnEnChange(WORD, WORD wID, HANDLE hEdit, BOOL &bHandled)
{
	if (IDC_PEFILE == wID)
	{
		WCHAR wzFile[MAX_PATH] = {0};
		GetDlgItemText(IDC_PEFILE, wzFile, _countof(wzFile));

		if (IsValidPE(wzFile))
		{
			WCHAR wzDrive[_MAX_DRIVE], wzDir[_MAX_DIR], wzFileName[_MAX_FNAME], wzFileExt[_MAX_EXT];
			_wsplitpath_s(wzFile, wzDrive, _MAX_DRIVE, wzDir, _MAX_DIR, wzFileName, _MAX_FNAME, wzFileExt, _MAX_EXT);

			wcsncat_s(wzFileName, L".nosign", _MAX_FNAME);
			WCHAR wzFileNoSign[MAX_PATH] = {0};
			_wmakepath_s(wzFileNoSign, wzDrive, wzDir, wzFileName, wzFileExt);

			SetDlgItemText(IDC_PEFILE_NOSIGN, wzFileNoSign);
			SetDlgItemText(IDC_STATE, TEXT_READY);
			::EnableWindow(GetDlgItem(IDC_BTN_REMOVE_SIGN), TRUE);
		}
		else
		{
			SetDlgItemText(IDC_STATE, TEXT_INVALID);
			SetDlgItemText(IDC_PEFILE, L"");
			SetDlgItemText(IDC_PEFILE_NOSIGN, L"");
			::EnableWindow(GetDlgItem(IDC_BTN_REMOVE_SIGN), FALSE);
		}

		bHandled = FALSE;
	}

	return 0;
}

LRESULT CMainDlg::OnBnClickedBtnRemoveSign(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled)
{
	WCHAR wzFile[MAX_PATH] = {0};
	WCHAR wzNewFile[MAX_PATH] = {0};

	GetDlgItemText(IDC_PEFILE, wzFile, _countof(wzFile));
	GetDlgItemText(IDC_PEFILE_NOSIGN, wzNewFile, _countof(wzNewFile));

	if (!CopyFile(wzFile, wzNewFile, FALSE))
	{
		SetDlgItemText(IDC_STATE, TEXT_FAIL_COPY);
		return 0;
	}

	HANDLE hFile = CreateFile(wzNewFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	BOOL isSuccess = ImageRemoveCertificate(hFile, 0);
	CloseHandle(hFile);

	if (!isSuccess)
	{
		SetDlgItemText(IDC_STATE, TEXT_FAIL_STRIPSIGN);
		DeleteFile(wzNewFile);
	}
	else
	{
		SetDlgItemText(IDC_STATE, TEXT_SUCCESS);
	}

	return 0;
}

LRESULT CMainDlg::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	HDROP hDrop = (HDROP)wParam;

	// Check if only one file is dropped
	UINT numFiles = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
	if (numFiles == 1) {
		WCHAR filePath[MAX_PATH];
		DragQueryFile(hDrop, 0, filePath, MAX_PATH);
		SetDlgItemText(IDC_PEFILE, filePath);
	}

	DragFinish(hDrop);
	bHandled = TRUE;
	return 0;
}