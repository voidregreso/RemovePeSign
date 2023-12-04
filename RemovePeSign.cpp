#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "MainDlg.h"

CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
    ::CoInitialize(NULL);
    ::DefWindowProc(NULL, 0, 0, 0L);
    AtlInitCommonControls(ICC_BAR_CLASSES);

    _Module.Init(NULL, hInstance);

    int nRet = 0;
    {
        CMainDlg dlgMain;
        nRet = dlgMain.DoModal();
    }

    _Module.Term();
    ::CoUninitialize();

    return nRet;
}
