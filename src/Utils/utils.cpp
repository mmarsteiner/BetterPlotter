//
// Created by Matthew on 6/06/2024.
//

#include "utils.h"

int mallocCounter = 0;

void bp::IntToString(const int i, char *out) {
    real_t tmp = os_FloatToReal(static_cast<float>(i));
    os_RealToStr(out, &tmp, 8, 1, 0);
}

void bp::DoubleToString(const double d, char *out) {
    real_t tmp = os_FloatToReal(static_cast<float>(d));
    os_RealToStr(out, &tmp, 8, 1, 2);
}

void bp::BoolToString(const bool b, char *out) {
    if (b) {
        strcpy(out, "True");
    } else {
        strcpy(out, "False");
    }
}

int bp::GetNumFromKeyCode(sk_key_t key) {
    switch (key) {
        case sk_0:
            return 0;
        case sk_1:
            return 1;
        case sk_2:
            return 2;
        case sk_3:
            return 3;
        case sk_4:
            return 4;
        case sk_5:
            return 5;
        case sk_6:
            return 6;
        case sk_7:
            return 7;
        case sk_8:
            return 8;
        case sk_9:
            return 9;
        default:
            return -1;
    }
}

void bp::DebugPrintByte(unsigned char byte) {
    for (int i = 0; i < 8; i++) {
        dbg_printf("%d", !!((byte << i) & 0x80));
    }
}

void bp::ClearNChars(size_t numChars) {
    char clearBuff[20];
    for (size_t i = 0; i < numChars; ++i) {
        strcat(clearBuff, " ");
    }
    os_PutStrFull(clearBuff);
}

// assumes that str contains a string with only digits 0-9
uint32_t bp::StrToUInt(const char *str) {
    uint32_t res = 0;
    const size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        res *= 10;
        res += str[i] - '0';
    }
    return res;
}

// assumes that str contains a valid double
double bp::StrToDouble(const char *str) {
    const real_t tmp = os_StrToReal(str, nullptr);
    return os_RealToFloat(&tmp);
}

double bp::fmax3(double d1, double d2, double d3) {
    if (d1 > d2) {
        if (d1 > d3) {
            return d1;
        } else {
            return d3;
        }
    } else {
        if (d2 > d3) {
            return d2;
        } else {
            return d3;
        }
    }
}
