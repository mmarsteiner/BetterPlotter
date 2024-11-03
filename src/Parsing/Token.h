#pragma once
#include <debug.h>
#include <ti/tokens.h>

#include <cstdint>

// we will use OS_TOK_E to replace OS_TOK_CONST_E, which is a 2 byte code since we will not need to use E as a variable in this program
#define NEW_TOK_E OS_TOK_E

namespace tiparser {
enum TokenClass {
    TI_TOKEN,
    NUM_TOKEN,
    END_TOKEN
};
enum TokenType {
    TT_END,
    TT_COMMA,
    TT_B1,
    TT_B2,
    TT_NEG,
    TT_POW,
    TT_FUNC,
    TT_VAR,
    TT_CONST,
    TT_PAREN,
    TT_SPEC_POW,
};
struct Token {
    static TokenType tokenTypes[256];
    TokenClass tokenClass;
    TokenType tokenType;
    Token(uint8_t code) {
        inner.code = code;
        tokenClass = TI_TOKEN;
        tokenType = tokenTypes[code];
    }
    Token(double val) {
        inner.val = val;
        tokenClass = NUM_TOKEN;
        tokenType = TT_CONST;
    }
    Token() {
        tokenClass = END_TOKEN;
        tokenType = TT_END;
    }
    bool isEquivalentTo(uint8_t code) {
        if (tokenClass != TI_TOKEN) {
            return false;
        }
        return inner.code == code;
    }
    bool operator==(const Token& other) {
        if (tokenClass != other.tokenClass) {
            return false;
        }
        if (tokenClass == TI_TOKEN) {
            return inner.code == other.inner.code;
        }
        if (tokenClass == END_TOKEN) {
            return other.tokenClass == END_TOKEN;
        }
        if (tokenClass == NUM_TOKEN) {
            return inner.val == other.inner.val;
        }
        return false;
    }
    void SetCode(uint8_t code) {
        tokenClass = TI_TOKEN;
        tokenType = tokenTypes[code];
        inner.code = code;
    }
    void SetVal(double val) {
        tokenClass = NUM_TOKEN;
        tokenType = TT_CONST;
        inner.val = val;
    }
    uint8_t getCode() {
        if (tokenClass == TI_TOKEN) {
            return inner.code;
        } else {
            dbg_printf("Error: requested code for a non-ti-coded token of class %d.\n", tokenClass);
            return 0;
        }
    }
    double getVal() {
        if (tokenClass == NUM_TOKEN) {
            return inner.val;
        } else {
            dbg_printf("Error: requested value for a non-number token of class %d.\n", tokenClass);
            return 0;
        }
    }
    // there has to be a better way to do this
    static void InitTokenTypes() {
        for (int i = 0; i < 256; i++) {
            tokenTypes[i] = TT_END;
        }
        tokenTypes[OS_TOK_ADD] = TT_B1;
        tokenTypes[OS_TOK_SUBTRACT] = TT_B1;
        tokenTypes[OS_TOK_MULTIPLY] = TT_B2;
        tokenTypes[OS_TOK_DIVIDE] = TT_B2;
        tokenTypes[OS_TOK_POWER] = TT_POW;
        tokenTypes[OS_TOK_COMMA] = TT_COMMA;
        tokenTypes[OS_TOK_E] = TT_CONST;
        tokenTypes[OS_TOK_PI] = TT_CONST;
        tokenTypes[OS_TOK_X] = TT_VAR;
        tokenTypes[OS_TOK_Y] = TT_VAR;
        tokenTypes[OS_TOK_Z] = TT_VAR;
        tokenTypes[OS_TOK_S] = TT_VAR;
        tokenTypes[OS_TOK_T] = TT_VAR;
        tokenTypes[OS_TOK_THETA] = TT_VAR;
        tokenTypes[OS_TOK_LN] = TT_FUNC;
        tokenTypes[OS_TOK_EXP] = TT_FUNC;
        tokenTypes[OS_TOK_LOG] = TT_FUNC;
        tokenTypes[OS_TOK_INV_LOG] = TT_FUNC;
        tokenTypes[OS_TOK_SIN] = TT_FUNC;
        tokenTypes[OS_TOK_INV_SIN] = TT_FUNC;
        tokenTypes[OS_TOK_COS] = TT_FUNC;
        tokenTypes[OS_TOK_INV_COS] = TT_FUNC;
        tokenTypes[OS_TOK_TAN] = TT_FUNC;
        tokenTypes[OS_TOK_INV_TAN] = TT_FUNC;
        tokenTypes[OS_TOK_SINH] = TT_FUNC;
        tokenTypes[OS_TOK_INV_SINH] = TT_FUNC;
        tokenTypes[OS_TOK_COSH] = TT_FUNC;
        tokenTypes[OS_TOK_INV_COSH] = TT_FUNC;
        tokenTypes[OS_TOK_TANH] = TT_FUNC;
        tokenTypes[OS_TOK_INV_TANH] = TT_FUNC;
        tokenTypes[OS_TOK_SQRT] = TT_FUNC;
        tokenTypes[OS_TOK_LEFT_PAREN] = TT_PAREN;
        tokenTypes[OS_TOK_RIGHT_PAREN] = TT_PAREN;
        tokenTypes[OS_TOK_SQUARE] = TT_SPEC_POW;
        tokenTypes[OS_TOK_CUBE] = TT_SPEC_POW;
        tokenTypes[OS_TOK_RECIPROCAL] = TT_SPEC_POW;
        tokenTypes[OS_TOK_NEGATIVE] = TT_NEG;
    }
    union {
        uint8_t code;
        double val;
    } inner;
};
}  // namespace tiparser