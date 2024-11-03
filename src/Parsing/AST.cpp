#include "AST.h"

namespace tiparser {
BinOpPtr AST::binOps[256] = {};
UOpPtr AST::uOps[256] = {};
void AST::InitOpPtrs() {
    binOps[OS_TOK_ADD] = [](double a, double b) { return a + b; };
    binOps[OS_TOK_SUBTRACT] = [](double a, double b) { return a - b; };
    binOps[OS_TOK_MULTIPLY] = [](double a, double b) { return a * b; };
    binOps[OS_TOK_DIVIDE] = [](double a, double b) { return a / b; };
    binOps[OS_TOK_POWER] = [](double a, double b) { return pow(a, b); };

    uOps[OS_TOK_LN] = &log;
    uOps[OS_TOK_EXP] = &exp;
    uOps[OS_TOK_LOG] = &log10;
    uOps[OS_TOK_INV_LOG] = [](double a) { return pow(10.0, a); };
    uOps[OS_TOK_SIN] = &sin;
    uOps[OS_TOK_INV_SIN] = &asin;
    uOps[OS_TOK_COS] = &cos;
    uOps[OS_TOK_INV_COS] = &acos;
    uOps[OS_TOK_TAN] = &tan;
    uOps[OS_TOK_INV_TAN] = &atan;
    uOps[OS_TOK_SINH] = &sinh;
    uOps[OS_TOK_INV_SINH] = &asinh;
    uOps[OS_TOK_COSH] = &cosh;
    uOps[OS_TOK_INV_COSH] = &acosh;
    uOps[OS_TOK_TANH] = &tanh;
    uOps[OS_TOK_INV_TANH] = &atanh;
    uOps[OS_TOK_SQRT] = &sqrt;
}
}  // namespace tiparser