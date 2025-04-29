//
// Created by Matthew on 6/06/2024.
//

#pragma once

#include <Menu/MenuItem.h>

#include <cstring>

namespace bp {
class MenuItemClose final : public MenuItem {
    public:
    MenuItemClose() = default;

    ~MenuItemClose() override = default;

    bool Run() override { return true; }

    void GetLabel(char *out) override { strcpy(out, "Close"); }

    void AppendLabel(char *out) override { strcat(out, "Close"); }
};
}  // namespace bp
