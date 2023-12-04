// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(VFC_MAINDLG_H__B09775FD_29C8_4d5f_9E52_5B18CCE7C3A1__INCLUDED_)
#define VFC_MAINDLG_H__B09775FD_29C8_4d5f_9E52_5B18CCE7C3A1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define TEXT_READY		L"Ready."
#define	TEXT_INVALID	L"Bad PE format."
#define	TEXT_SUCCESS	L"Removed digital signature successfully."
#define TEXT_FAIL_COPY	L"Failed to copy original file."
#define	TEXT_FAIL_STRIPSIGN		L"Failed to remove digital signature."

class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP(CMainDlg)
		COMMAND_HANDLER(IDC_BTN_REMOVE_SIGN, BN_CLICKED, OnBnClickedBtnRemoveSign)
		COMMAND_HANDLER(IDC_BTN_SAVE, BN_CLICKED, OnBnClickedBtnSave)
		COMMAND_HANDLER(IDC_BTN_SELECT, BN_CLICKED, OnBnClickedBtnSelect)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_CODE_HANDLER(EN_CHANGE, OnEnChange)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	END_MSG_MAP()


	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnCtlColorEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnBnClickedBtnSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnBnClickedBtnSelect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnEnChange(WORD, WORD wID, HANDLE hEdit, BOOL& bHandled);

	LRESULT OnBnClickedBtnRemoveSign(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// VisualFC AppWizard will insert additional declarations immediately before the previous line.

#endif // !defined(VFC_MAINDLG_H__B09775FD_29C8_4d5f_9E52_5B18CCE7C3A1__INCLUDED_)
