/* Copyright (C) 
* 2010 - Aurelien Rainone
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>
#include <Windowsx.h>
#include <commctrl.h>
#include <Shellapi.h>
#include <Shlwapi.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "win/resource.h"

#include "doclibd.hpp"
#include "doclibd_includes.hpp"
#include "../core/doclib_core.hpp"
#include "../libmgr/doc_database.hpp"
#include "../version.hpp"

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "server.hpp"


#define TRAYICONID	1//				ID number for the Notify Icon
#define SWM_TRAYMSG	WM_APP//		the message ID sent to our window

#define SWM_SHOW	WM_APP + 1//	show the window
#define SWM_HIDE	WM_APP + 2//	hide the window
#define SWM_EXIT	WM_APP + 3//	close the window

// Global Variables:
HINSTANCE		hInst;	// current instance
NOTIFYICONDATA	niData;	// notify icon data

doclib::core::doclib_conf cfg;

// Forward declarations of functions included in this code module:
BOOL				InitInstance(HINSTANCE, int);
BOOL				OnInitDialog(HWND hWnd);
void				ShowContextMenu(HWND hWnd);
ULONGLONG			GetDllVersion(LPCTSTR lpszDllName);

INT_PTR CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);



/************************************************************************/
/* HANDLING CONSOLE IO in a Windows Subsystem							*/
/*		http://dslweb.nwnexus.com/~ast/dload/guicon.htm					*/
/************************************************************************/


/************************************************************************/
/* HANDLING CONSOLE IO in a Windows Subsystem							*/
/*		http://dslweb.nwnexus.com/~ast/dload/guicon.htm					*/
/************************************************************************/


/************************************************************************/
/* HANDLING CONSOLE IO in a Windows Subsystem							*/
/*		http://dslweb.nwnexus.com/~ast/dload/guicon.htm					*/
/************************************************************************/


/************************************************************************/
/* HANDLING CONSOLE IO in a Windows Subsystem							*/
/*		http://dslweb.nwnexus.com/~ast/dload/guicon.htm					*/
/************************************************************************/


/************************************************************************/
/* HANDLING CONSOLE IO in a Windows Subsystem							*/
/*		http://dslweb.nwnexus.com/~ast/dload/guicon.htm					*/
/************************************************************************/


/************************************************************************/
/* HANDLING CONSOLE IO in a Windows Subsystem							*/
/*		http://dslweb.nwnexus.com/~ast/dload/guicon.htm					*/
/************************************************************************/


/************************************************************************/
/* HANDLING CONSOLE IO in a Windows Subsystem							*/
/*		http://dslweb.nwnexus.com/~ast/dload/guicon.htm					*/
/************************************************************************/




int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	int ret = EXIT_FAILURE;

	try
	{
		doclib::daemon::commandline_options options;
		std::cout << "DocumentLibrary version " << doclib::daemon::get_version_string() << std::endl;
		doclib::daemon::treat_command_line(__argc, __argv, options);

		if (cfg.load(options.config_file))
		{
			// init log instance, providing it 'ini file loaded' values
			doclib::core::core_services::init_logging(cfg);
			LDBG_ << "successfully loaded configuration file";

			doclib::libmgr::doc_database::create_instance(cfg);

			if (doclib::libmgr::doc_database::get_instance().load_database())
			{
				LDBG_ << "successfully loaded document database";
				ret = EXIT_SUCCESS;


				// load daemon server parameters
				std::string address = "0.0.0.0";
				std::string port = boost::lexical_cast<std::string>(cfg.get_port());
				std::size_t num_threads = cfg.get_num_threads();

				// Run server in background thread.
				doclib::daemon::server s(address, port, num_threads);
				boost::thread t(boost::bind(&doclib::daemon::server::run, &s));


				// Perform application initialization:
				if (!InitInstance (hInstance, nCmdShow)) return FALSE;
				hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_DOCLIBD_DLG);

				// Main message loop:
				while (GetMessage(&msg, NULL, 0, 0))
				{
					if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)||
						!IsDialogMessage(msg.hwnd,&msg) ) 
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}

				// Stop the server.
				s.stop();
				L_ << "server stopped" << std::endl;
				t.join();
				LDBG_ << "background thread stopped" << std::endl;


				ret = (int) msg.wParam;

			}
			else
				LERR_ << "couldn't load document database";

		}
		else
			std::cerr << "incorrect configuration (" << options.config_file << ")" << std::endl;
	}
	catch(std::exception &e)
	{
		LERR_ << "exception thrown : " << e.what() << std::endl;
		std::cerr << "exception thrown : " << e.what() << std::endl;
	}

	return ret;
}

//	Initialize the window and tray icon
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	// prepare for XP style controls
	InitCommonControls();

	// store instance handle and create dialog
	hInst = hInstance;
	HWND hWnd = CreateDialog( hInstance, MAKEINTRESOURCE(IDD_DLG_DIALOG),
		NULL, (DLGPROC)DlgProc );
	if (!hWnd) return FALSE;

	// Fill the NOTIFYICONDATA structure and call Shell_NotifyIcon

	ZeroMemory(&niData,sizeof(NOTIFYICONDATA));

	// get Shell32 version number and set the size of the structure
	//		note:	the MSDN documentation about this is a little
	//				dubious and I'm not at all sure if the method
	//				bellow is correct
	ULONGLONG ullVersion = GetDllVersion(_T("Shell32.dll"));
	if(ullVersion >= MAKEDLLVERULL(5, 0,0,0))
		niData.cbSize = sizeof(NOTIFYICONDATA);
	else niData.cbSize = NOTIFYICONDATA_V2_SIZE;

	// the ID number can be anything you choose
	niData.uID = TRAYICONID;

	// state which structure members are valid
	niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

	// load the icon
	niData.hIcon = (HICON)LoadImage(hInstance,MAKEINTRESOURCE(IDI_DOCLIBD_DLG),
		IMAGE_ICON, GetSystemMetrics(SM_CXSMICON),GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);

	// the window to send messages to and the message to send
	//		note:	the message value should be in the
	//				range of WM_APP through 0xBFFF
	niData.hWnd = hWnd;
	niData.uCallbackMessage = SWM_TRAYMSG;

	// tooltip message
	lstrcpyn(niData.szTip, _T("Time flies like an arrow but\n   fruit flies like a banana!"), sizeof(niData.szTip)/sizeof(TCHAR));

	Shell_NotifyIcon(NIM_ADD,&niData);

	// free icon handle
	if(niData.hIcon && DestroyIcon(niData.hIcon))
		niData.hIcon = NULL;

	// call ShowWindow here to make the dialog initially visible

	return TRUE;
}

BOOL OnInitDialog(HWND hWnd)
{
	HMENU hMenu = GetSystemMenu(hWnd,FALSE);
	if (hMenu)
	{
		AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MF_STRING, IDM_ABOUT, _T("About"));
	}
	HICON hIcon = (HICON)LoadImage(hInst,
		MAKEINTRESOURCE(IDI_DOCLIBD_DLG),
		IMAGE_ICON, 0,0, LR_SHARED|LR_DEFAULTSIZE);
	SendMessage(hWnd,WM_SETICON,ICON_BIG,(LPARAM)hIcon);
	SendMessage(hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
	return TRUE;
}

// Name says it all
void ShowContextMenu(HWND hWnd)
{
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu = CreatePopupMenu();
	if(hMenu)
	{
		if( IsWindowVisible(hWnd) )
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_HIDE, _T("Hide"));
		else
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_SHOW, _T("Show"));
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, _T("Exit"));

		// note:	must set window to the foreground or the
		//			menu won't disappear when it should
		SetForegroundWindow(hWnd);

		TrackPopupMenu(hMenu, TPM_BOTTOMALIGN,
			pt.x, pt.y, 0, hWnd, NULL );
		DestroyMenu(hMenu);
	}
}

// Get dll version number
ULONGLONG GetDllVersion(LPCTSTR lpszDllName)
{
	ULONGLONG ullVersion = 0;
	HINSTANCE hinstDll;
	hinstDll = LoadLibrary(lpszDllName);
	if(hinstDll)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");
		if(pDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			HRESULT hr;
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			hr = (*pDllGetVersion)(&dvi);
			if(SUCCEEDED(hr))
				ullVersion = MAKEDLLVERULL(dvi.dwMajorVersion, dvi.dwMinorVersion,0,0);
		}
		FreeLibrary(hinstDll);
	}
	return ullVersion;
}

// Message handler for the app
INT_PTR CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message) 
	{
	case SWM_TRAYMSG:
		switch(lParam)
		{
		case WM_LBUTTONDBLCLK:
			ShowWindow(hWnd, SW_RESTORE);
			break;
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
			ShowContextMenu(hWnd);
		}
		break;
	case WM_SYSCOMMAND:
		if((wParam & 0xFFF0) == SC_MINIMIZE)
		{
			ShowWindow(hWnd, SW_HIDE);
			return 1;
		}
		else if(wParam == IDM_ABOUT)
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam); 

		switch (wmId)
		{
		case SWM_SHOW:
			ShowWindow(hWnd, SW_RESTORE);
			break;
		case SWM_HIDE:
		case IDOK:
			ShowWindow(hWnd, SW_HIDE);
			break;
		case SWM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		}
		return 1;
	case WM_INITDIALOG:
		return OnInitDialog(hWnd);
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		niData.uFlags = 0;
		Shell_NotifyIcon(NIM_DELETE,&niData);
		PostQuitMessage(0);
		break;
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
