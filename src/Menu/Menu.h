//
// Created by Matthew on 6/06/2024.
//

#pragma once

#include <Utils/List.h>
#include <Utils/utils.h>
#include <debug.h>
#include <ti/flags.h>
#include <ti/getcsc.h>
#include <ti/screen.h>

#include "MainMenuElements/MenuItemClose.h"
#include "MenuItem.h"

namespace bp {

class Menu {
    char title[20]{};

    unsigned int selectedItem;
    List<MenuItem> menuItems;
    void DrawMenu() const;
    void MoveCursor(int unitsUp);

    public:
    explicit Menu(const char* title);
    ~Menu();
    void Run();
    void AddMenuItem(MenuItem& menuItem);
    void GetTitle(char* out) const;
    void AppendTitle(char* out) const;
};

}  // namespace bp
