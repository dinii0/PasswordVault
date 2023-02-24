#ifndef ADDITEMBOX_H
#define ADDITEMBOX_H

#include <windows.h>
#include <commctrl.h>

#define HANDLER_PUSH_ID 1140

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define SECONDAL_CLASSNAME L"AdditionalBox"

HWND hwnd_name;
HWND hwnd_pass;
HWND list_handler;

HINSTANCE instance_handler;

LRESULT CALLBACK AnotherWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

bool hardcoded_checking = false;

void add_items(HWND hwnd, TCHAR username[], TCHAR password[]) {
    LVITEM lvItem;
    lvItem.mask = LVIF_TEXT;

    lvItem.iItem = ListView_GetItemCount(list_handler);
    lvItem.iSubItem = 0;
    lvItem.pszText = (PTSTR) username;
    SendMessage(list_handler, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

    lvItem.iSubItem = 1;
    lvItem.pszText = (PTSTR) password;
    SendMessage(list_handler, LVM_SETITEM, 0, (LPARAM)&lvItem);

    hardcoded_checking = false;
    DestroyWindow(hwnd);
}

void withdraw_handler(HWND hwnd) {
    list_handler = hwnd;
}

int is_input_valid(TCHAR text[]) {
    if (text[0] == '\0') {
        return -2;
    }

    for (int i = 0; text[i] != '\0'; i++) {
        if (isspace(text[i]))
            return -1;
    }

    return 0;
}

void adjust_window_elements(HWND hwndParent) {

    RECT rect;
    GetClientRect(hwndParent, &rect);

    hwnd_name = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, ((rect.right - rect.left - 200) / 2), 32, 200, 25, hwndParent, NULL, instance_handler, NULL);
    hwnd_pass = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, ((rect.right - rect.left - 200) / 2), 103, 200, 25, hwndParent, NULL, instance_handler, NULL);
    HWND hwndButton = CreateWindow(L"BUTTON", L"Add", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, ((rect.right - rect.left - 500) / 2), 180, 500, 80, hwndParent, (HMENU)HANDLER_PUSH_ID, instance_handler, NULL);
}

void create_temporary_additem_box() {
    if (hardcoded_checking)
    {
        MessageBox(NULL, (LPCWSTR)L"A Window is already open\nFinish the previous process before pushing the button", (LPCWSTR)L"Warning", MB_ICONWARNING | MB_OK);
        return;
    }

    WNDCLASS newWindowClass = { 0 };
    HBRUSH hHeaderBrush = CreateSolidBrush(RGB(255, 0, 0));

    newWindowClass.lpfnWndProc = AnotherWndProc;
    newWindowClass.hInstance = instance_handler;
    newWindowClass.hIcon = LoadIcon(instance_handler, MAKEINTRESOURCE(IDI_SMALL));
    newWindowClass.lpszClassName = SECONDAL_CLASSNAME;
    newWindowClass.hbrBackground = CreateSolidBrush(RGB(220, 220, 220));
    newWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClass(&newWindowClass);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN), screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int postiotional_dimension_x = (screenWidth - WINDOW_WIDTH) / 2, postiotional_dimension_y = (screenHeight - WINDOW_HEIGHT) / 2;

    HWND newWindow = CreateWindow(SECONDAL_CLASSNAME, L"Add an item", WS_OVERLAPPEDWINDOW | WS_BORDER, postiotional_dimension_x, postiotional_dimension_y, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, instance_handler, nullptr);

    SetWindowLong(newWindow, GWL_STYLE, GetWindowLong(newWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX);
    SetWindowLong(newWindow, GWL_STYLE, GetWindowLong(newWindow, GWL_STYLE) & ~WS_THICKFRAME);
    ShowWindow(newWindow, SW_SHOWDEFAULT);

    UpdateWindow(newWindow);
    adjust_window_elements(newWindow);

    hardcoded_checking = true;
}

LRESULT CALLBACK AnotherWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = GetWindowDC(hwnd);

    switch (message)
    {
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);

        TCHAR string_name[] = _T("Write a name or a short description about your passcode:");
        TCHAR string_code[] = _T("Write the actual passcode:");

        RECT rect;
        GetClientRect(hwnd, &rect);

        SIZE textSize;
        GetTextExtentPoint32(hdc, string_name, static_cast<int>(_tcslen(string_name)), &textSize);
        int screenWidth = (rect.right - rect.left - textSize.cx) / 2;

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));
        TextOut(hdc, screenWidth, 15, string_name, static_cast<int>(_tcslen(string_name)));

        GetTextExtentPoint32(hdc, string_code, static_cast<int>(_tcslen(string_code)), &textSize);
        screenWidth = (rect.right - rect.left - textSize.cx) / 2;

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));
        TextOut(hdc, screenWidth, 85, string_code, static_cast<int>(_tcslen(string_code)));

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        if (wmId == HANDLER_PUSH_ID) {
            TCHAR buffer_name[65], buffer_pass[65];

            GetWindowText(hwnd_name, buffer_name, sizeof(buffer_name));
            GetWindowText(hwnd_pass, buffer_pass, sizeof(buffer_pass));

            int stored_returns = is_input_valid(buffer_name);

            switch (stored_returns)
            {
                case -2: {
                    MessageBox(hwnd, L"Strings cannot contain less than 5 characters", L"Error", 0);
                    return 0;
                }
                case -1: {
                    MessageBox(hwnd, L"Strings cannot contain spaces", L"Error", 0);
                    return 0;
                }
            }

            stored_returns = is_input_valid(buffer_pass);

            switch (stored_returns)
            {
                case -2: {
                    MessageBox(hwnd, L"Strings cannot contain less than 5 characters", L"Error", 0);
                    return 0;
                }
                case -1: {
                    MessageBox(hwnd, L"Strings cannot contain spaces", L"Error", 0);
                    return 0;
                }
            }

            add_items(hwnd, buffer_name, buffer_pass);
            return 0;
        }
        else if (wmId == IDM_EXIT) {
            hardcoded_checking = false;
            DestroyWindow(hwnd);
            break;
        }
        else {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }
    case WM_CLOSE:
        hardcoded_checking = false;
        DestroyWindow(hwnd);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

#endif