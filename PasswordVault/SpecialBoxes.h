#ifndef SPECIALBOXES_H
#define SPECIALBOXES_H

#include <commctrl.h>

#define ID_REMOVE_ITEM 1069

int show_context_menu(HWND hWnd, int x, int y, int item_index) {
    HMENU hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, ID_REMOVE_ITEM, L"Remove item");

    int selection = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, x, y, 0, hWnd, NULL);

    if (selection == ID_REMOVE_ITEM)
    {
        if (item_index != -1) {
            ListView_DeleteItem(hWnd, item_index);
            item_index = -1;
        }
    }

    DestroyMenu(hMenu);
    return item_index;
}

#endif

