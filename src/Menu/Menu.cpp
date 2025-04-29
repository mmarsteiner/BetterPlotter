//
// Created by Matthew on 6/06/2024.
//

#include "Menu.h"

namespace bp {
void Menu::DrawMenu() const {
    os_DisableCursor();
    os_ClrHomeFull();
    os_SetFlag(APP, TEXTSAVE);
    os_PutStrFull(title);
    for (int i = 0; static_cast<size_t>(i) < menuItems.Size(); i++) {
        os_SetCursorPos(i + 1, 0);
        char line[50];
        IntToString(i + 1, line);
        strcat(line, ":");
        menuItems.Get(i)->AppendLabel(line);
        os_PutStrFull(line);
    }
    os_SetCursorPos(selectedItem, 0);
    os_EnableCursor();
}

// Units up should equal 1 to move the cursor up and -1 to move the cursor down
void Menu::MoveCursor(const int unitsUp) {
    os_DisableCursor();
    if (unitsUp < 0) {
        if (selectedItem < menuItems.Size()) {
            os_SetCursorPos(++selectedItem, 0);
        } else {
            selectedItem = 1;
            os_SetCursorPos(selectedItem, 0);
        }
    } else {
        if (selectedItem > 1) {
            os_SetCursorPos(--selectedItem, 0);
        } else {
            selectedItem = menuItems.Size();
            os_SetCursorPos(selectedItem, 0);
        }
    }
    os_EnableCursor();
}

Menu::Menu(const char* title) {
    selectedItem = 1;
    strcpy(this->title, title);
}

Menu::~Menu() {}

void Menu::Run() {
    DrawMenu();
    sk_key_t key;
    while ((key = os_GetCSC()) != sk_Clear) {
        switch (key) {
            case sk_Enter: {
                if (menuItems.Get(selectedItem - 1)->Run()) {
                    return;
                }
                DrawMenu();
                break;
            }
            case sk_Up:
                MoveCursor(1);
                break;
            case sk_Down:
                MoveCursor(-1);
                break;
            default: {
                int num = GetNumFromKeyCode(key);
                if (num >= 1 && static_cast<size_t>(num) <= menuItems.Size()) {
                    if (menuItems.Get(num - 1)->Run()) {
                        return;
                    }
                    DrawMenu();
                    break;
                }
            }
        }
    }
}

void Menu::AddMenuItem(MenuItem& menuItem) { menuItems.Add(&menuItem); }

void Menu::AppendTitle(char* out) const { strcat(out, title); }

void Menu::GetTitle(char* out) const { strcpy(out, title); }
}  // namespace bp