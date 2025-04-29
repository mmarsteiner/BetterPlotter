//
// Created by mmars on 23/06/2024.
//

#pragma once

#include <string.h>

#include "../MenuItem.h"

namespace bp {
typedef bool (*MenuFunction)();

class MenuItemGeneric final : public MenuItem {
    char label[20]{};
    MenuFunction menuFunc;

    public:
    MenuItemGeneric(const char *label, MenuFunction menuFunc)
        : menuFunc{menuFunc} {
        strcpy(this->label, label);
    }
    ~MenuItemGeneric() override = default;

    bool Run() override { return menuFunc(); }

    void GetLabel(char *out) override { strcpy(out, label); }

    void AppendLabel(char *out) override { strcat(out, label); }
};
}  // namespace bp