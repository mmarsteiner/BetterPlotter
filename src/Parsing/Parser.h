#pragma once

#include <debug.h>
#include <ti/tokens.h>

#include <cstdint>

#define MAX_TOKENS 100

#include <fileioc.h>

#include "AST.h"
#include "Token.h"

namespace tiparser {
class Parser {
    uint8_t* rawData;
    uint16_t rawSize;
    Token tokens[MAX_TOKENS];
    uint16_t location;

    void LexInput();
    int TokToInt(uint8_t tok);
    bool IsNumTok(uint8_t tok);

    void ConsumeToken();
    int ExpectToken(uint8_t code);

    AST* START();
    AST_Triple_Expr* TAIL();
    AST* EXPRESSION_HEAD();
    ASTBinOp* EXPRESSION_TAIL();
    AST* TERM_HEAD();
    ASTBinOp* TERM_TAIL();
    AST* FACTOR_HEAD();
    ASTBinOp* FACTOR_TAIL();
    AST* BASE();
    ASTVar* VARIABLE();
    ASTFunc* FUNC();
    ASTBinOp* B1();
    ASTBinOp* B2();

  public:
    AST* Parse(const char* varName);
};
}  // namespace tiparser