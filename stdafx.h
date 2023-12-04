// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(VFC_STDAFX_H__1FF55366_7908_419a_8CE6_7D41CEEF1FE8__INCLUDED_)
#define VFC_STDAFX_H__1FF55366_7908_419a_8CE6_7D41CEEF1FE8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Change these values to use different versions
#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>
#include <atldlgs.h>
#include <imagehlp.h>

#pragma comment(lib, "imagehlp.lib")

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// VisualFC AppWizard will insert additional declarations immediately before the previous line.

#endif // !defined(VFC_STDAFX_H__1FF55366_7908_419a_8CE6_7D41CEEF1FE8__INCLUDED_)
