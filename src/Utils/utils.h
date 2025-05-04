//
// Created by Matthew on 6/06/2024.
//

#pragma once

#include <debug.h>
#include <fileioc.h>
#include <ti/getcsc.h>
#include <ti/real.h>
#include <ti/screen.h>

#include <cstdlib>
#include <cstring>

namespace bp {
void IntToString(int i, char *out);
void DoubleToString(double d, char *out);
void BoolToString(bool b, char *out);
int GetNumFromKeyCode(sk_key_t key);
void DebugPrintByte(unsigned char byte);
void ClearNChars(size_t numChars);  // prints N clear characters at the current cursor pos
uint32_t StrToUInt(const char *str);
double StrToDouble(const char *str);
}  // namespace bp
