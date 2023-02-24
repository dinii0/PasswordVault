// PasswordVault.cpp : Defines the entry point for the application.
//

#include "ConvertionsDataTypes.h"
#include "WordCryption.h"
#include "PasswordVault.h"
#include "SpecialBoxes.h"
#include "VaultFile.h"
#include "AddItemBox.h"

#include "framework.h"
#include <windows.h>
#include <commctrl.h>
#include <string>

#define MAX_LOADSTRING 100

#define ID_WINDOWS_KID 1070

int secondal_menu_index = -1;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND newWindow;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    read_encrypted_file();
    instance_handler = hInstance;

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PASSWORDVAULT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PASSWORDVAULT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PASSWORDVAULT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(220, 220, 220));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PASSWORDVAULT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

HMENU hMenu;
HWND hList;
int item_index = -1;
#define ID_REMOVE_ITEM 1069
#define ID_PUSH_BUTTON 1070

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rcClient;

    switch (message)
    {
    case WM_CREATE:
        {
            SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);

            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            hList = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | LVS_REPORT, 1, 50, screenWidth - 2, 350, hWnd, NULL, NULL, NULL);
            SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
            withdraw_handler(hList);

            LVCOLUMN lvColumn;
            lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
            lvColumn.fmt = LVCFMT_LEFT;
            lvColumn.cx = screenWidth / 2;
            lvColumn.pszText = PTSTR(L"Name");
            SendMessage(hList, LVM_INSERTCOLUMN, 5, (LPARAM)&lvColumn);

            lvColumn.cx = screenWidth / 2;
            lvColumn.pszText = PTSTR(L"Password");
            SendMessage(hList, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);

            LVITEM lvItem;
            lvItem.mask = LVIF_TEXT;
            for (int i = 0; i < host_words.size(); i++) {
                lvItem.iItem = i;
                lvItem.iSubItem = 0;
                lvItem.pszText = convert_string_to_ptstr(host_words[i][0]);
                SendMessage(hList, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

                lvItem.iSubItem = 1;
                lvItem.pszText = convert_string_to_ptstr(host_words[i][1]);
                SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&lvItem);
            }

            int screenLength = GetSystemMetrics(SM_CYSCREEN);
            HWND hwndButton = CreateWindow(L"BUTTON", L"Add items", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, static_cast<int>(screenLength + screenLength / 1.5), 15, 80, 20, hWnd, (HMENU)ID_PUSH_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

            break;
        }
    case WM_NOTIFY:
        {
            NMHDR* pnmh = (NMHDR*)lParam;
            if (pnmh->hwndFrom == hList) {
                if (pnmh->code == NM_RCLICK) {
                    int64_t result = (int64_t)pnmh->hwndFrom - (int64_t)hList;
                    if (result == 0) {
                        NMITEMACTIVATE* pia = (NMITEMACTIVATE*)lParam;
                        item_index = pia->iItem;

                        POINT pt;
                        GetCursorPos(&pt);
                        item_index = show_context_menu(hList, pt.x, pt.y, item_index);
                    }
                }
                else if (pnmh->code == LVN_ITEMACTIVATE) {
                    NMITEMACTIVATE* pia = (NMITEMACTIVATE*)lParam;
                    LPNMLISTVIEW pnmv = (LPNMLISTVIEW)lParam;

                    LVITEM lvitem = { 0 };
                    lvitem.mask = LVIF_TEXT;
                    lvitem.iItem = pnmv->iItem;
                    lvitem.iSubItem = pnmv->iSubItem;
                    lvitem.cchTextMax = 64;
                    TCHAR buffer[64];
                    lvitem.pszText = buffer;
                    SendMessage(pnmh->hwndFrom, LVM_GETITEMTEXT, (WPARAM)pnmv->iItem, (LPARAM)&lvitem);

                    if (OpenClipboard(NULL))
                    {
                        EmptyClipboard();

                        HGLOBAL copied_string = GlobalAlloc(GMEM_MOVEABLE, (_tcslen(buffer) + 1) * sizeof(TCHAR));
                        if (copied_string != NULL)
                        {
                            LPTSTR string_copied = (LPTSTR)GlobalLock(copied_string);
                            _tcscpy_s(string_copied, _tcslen(buffer) + 1, buffer);
                            GlobalUnlock(copied_string);
                            SetClipboardData(CF_UNICODETEXT, copied_string);
                        }
                        CloseClipboard();

                        LPSTR lpstr = (LPSTR)GlobalLock(copied_string);
                        if (lpstr != NULL)
                        {
                            int len = strlen(lpstr) + 1;
                            int wlen = MultiByteToWideChar(CP_ACP, 0, lpstr, len, NULL, 0);
                            LPWSTR lpwstr = new WCHAR[wlen];
                            MultiByteToWideChar(CP_ACP, 0, lpstr, len, lpwstr, wlen);
                            GlobalUnlock(copied_string);
                            LPCWSTR lpcwstr = lpwstr;
                            delete[] lpwstr;

                            LPWSTR lp_global_variable = static_cast<LPWSTR>(GlobalLock(copied_string));
                            std::wstring window_box_message = L"\"";
                            window_box_message += lp_global_variable;
                            window_box_message += L"\" has been added to your clipboard";
                            GlobalUnlock(copied_string);
                            MessageBox(0, window_box_message.c_str(), L"Password Vault", MB_OK | MB_ICONINFORMATION);
                        }
                    }
                }
            }
            break;
        }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            
            switch (wmId)
            {
            case ID_PUSH_BUTTON: {
                if(HIWORD(wParam) == BN_CLICKED)
                    create_temporary_additem_box();

                break;
            }
            case IDM_ABOUT: {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            }
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            TCHAR greeting[] = _T("All valuable cards, passcodes, and texts are stored in the table below:");

            // TODO: Add any drawing code that uses hdc here...
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(0, 0, 0));
            TextOut(hdc, 15, 15, greeting, _tcslen(greeting));


            EndPaint(hWnd, &ps);
        }
        break;
    case WM_CLOSE:
    {
        delete_stored_data();
        withdraw_items_files(hList);
        update_encrypted_file();

        DestroyWindow(hWnd);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG: 
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
