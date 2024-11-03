//
// Created by mmars on 11/06/2024.
//

#include "Setting.h"

#include <debug.h>
#include <ti/screen.h>

namespace bp {
void Setting::MoveCursorCol(int newCol) {
    os_DisableCursor();
    os_SetCursorPos(os_CurRow, newCol);
    os_EnableCursor();
}

void Setting::StartEditing() { GetValString(tmpEdit); }

uint8_t Setting::GetScreenLine(char *out) {
    GetLabel(out);
    strcat(out, "=");
    uint8_t ret = strlen(out) + 1;
    char valStr[9];
    GetValString(valStr);
    strcat(out, valStr);
    return ret;
}

uint8_t Setting::GetCursorCol() const {
    return strlen(label) + editingOffset +
           2;  // 1 for white space on left and 1 for = sign
}

void SettingDouble::ProcessInput(sk_key_t key) {
    if (key == sk_Up || key == sk_Down || key == sk_Enter) {
        val = StrToDouble(tmpEdit);
        editingOffset = 0;
    } else if (key == sk_Left && editingOffset != 0) {
        --editingOffset;
        MoveCursorCol(GetCursorCol());
    } else if (key == sk_Right && tmpEdit[editingOffset] != '\0') {
        ++editingOffset;
        MoveCursorCol(GetCursorCol());
    } else if (key == sk_Del && editingOffset > 0) {
        os_DisableCursor();
        // erase char to left and shift all other chars 1 to the left
        for (size_t i = editingOffset - 1; tmpEdit[i] != '\0'; ++i) {
            tmpEdit[i] = tmpEdit[i + 1];
        }
        strcat(tmpEdit, " ");
        --editingOffset;
        os_SetCursorPos(os_CurRow, GetCursorCol() - editingOffset);
        os_PutStrFull(tmpEdit);
        os_SetCursorPos(os_CurRow, GetCursorCol());
        os_EnableCursor();
        // remove extra space character
        size_t len = strlen(tmpEdit);
        tmpEdit[len - 1] = '\0';
    } else {
        int num;
        if (key == sk_DecPnt || key == sk_Chs) {
            os_DisableCursor();
            if (tmpEdit[editingOffset] == '\0') {
                tmpEdit[editingOffset + 1] = '\0';
            }
            char c = key == sk_DecPnt ? '.' : NEGATE_CHAR;
            tmpEdit[editingOffset] = c;
            ++editingOffset;
            os_SetCursorPos(os_CurRow, GetCursorCol() - editingOffset);
            os_PutStrFull(tmpEdit);
            os_SetCursorPos(os_CurRow, GetCursorCol());
            os_EnableCursor();
        } else if ((num = GetNumFromKeyCode(key)) >= 0) {
            os_DisableCursor();
            if (tmpEdit[editingOffset] == '\0') {
                tmpEdit[editingOffset + 1] = '\0';
            }
            tmpEdit[editingOffset] = '0' + num;
            ++editingOffset;
            os_SetCursorPos(os_CurRow, GetCursorCol() - editingOffset);
            os_PutStrFull(tmpEdit);
            os_SetCursorPos(os_CurRow, GetCursorCol());
            os_EnableCursor();
        }
    }
}

void SettingUInt::ProcessInput(sk_key_t key) {
    if (key == sk_Up || key == sk_Down || key == sk_Enter) {
        val = StrToUInt(tmpEdit);
        editingOffset = 0;
    } else if (key == sk_Left && editingOffset != 0) {
        --editingOffset;
        MoveCursorCol(GetCursorCol());
    } else if (key == sk_Right && tmpEdit[editingOffset] != '\0') {
        ++editingOffset;
        MoveCursorCol(GetCursorCol());
    } else if (key == sk_Del && editingOffset > 0) {
        os_DisableCursor();
        // erase char to left and shift all other chars 1 to the left
        for (size_t i = editingOffset - 1; tmpEdit[i] != '\0'; ++i) {
            tmpEdit[i] = tmpEdit[i + 1];
        }
        strcat(tmpEdit, " ");
        --editingOffset;
        os_SetCursorPos(os_CurRow, GetCursorCol() - editingOffset);
        os_PutStrFull(tmpEdit);
        os_SetCursorPos(os_CurRow, GetCursorCol());
        os_EnableCursor();
        // remove extra space character
        size_t len = strlen(tmpEdit);
        tmpEdit[len - 1] = '\0';
    } else {
        int num = GetNumFromKeyCode(key);
        if (num >= 0) {
            os_DisableCursor();
            if (tmpEdit[editingOffset] == '\0') {
                tmpEdit[editingOffset + 1] = '\0';
            }
            tmpEdit[editingOffset] = '0' + num;
            ++editingOffset;
            os_SetCursorPos(os_CurRow, GetCursorCol() - editingOffset);
            os_PutStrFull(tmpEdit);
            os_SetCursorPos(os_CurRow, GetCursorCol());
            os_EnableCursor();
        }
    }
}

void SettingBool::ProcessInput(sk_key_t key) {
    bool changeToFalse =
        key == sk_Clear || key == sk_Right || key == sk_0 || key == sk_Cos;
    bool changeToTrue = key == sk_Left || key == sk_1 || key == sk_4;
    if (changeToFalse || changeToTrue) {
        os_DisableCursor();
        val = changeToTrue;
        GetValString(tmpEdit);
        strcat(tmpEdit, " ");
        os_PutStrFull(tmpEdit);
        os_SetCursorPos(os_CurRow, GetCursorCol());
        os_EnableCursor();
        // remove extra space character
        size_t len = strlen(tmpEdit);
        tmpEdit[len - 1] = '\0';
    }
}
}  // namespace bp
