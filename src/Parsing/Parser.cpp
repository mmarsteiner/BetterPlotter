#include "Parser.h"

namespace tiparser {

AST* Parser::Parse(const char* varName) {
    if (Token::tokenTypes[OS_TOK_ADD] != TT_B1) {
        Token::InitTokenTypes();
    }
    ti_var_t var = ti_OpenVar(varName, "r", OS_TYPE_EQU);
    if (!var) {
        dbg_printf("Error, could not read variable %s\n", varName);
        return nullptr;
    }
    rawData = static_cast<uint8_t*>(ti_GetDataPtr(var));
    rawSize = ti_GetSize(var);
    LexInput();
    location = 0;
    return START();
}

void Parser::LexInput() {
    uint16_t tokIdx = 0;
    for (uint16_t i = 0; i < rawSize; i++) {
        uint8_t tok = rawData[i];
        if (Token::tokenTypes[tok] != 0) {
            tokens[tokIdx].SetCode(tok);
            ++tokIdx;
        } else if (IsNumTok(tok)) {
            double val = 0;
            while (IsNumTok(rawData[i])) {
                val *= 10.0;
                val += TokToInt(rawData[i]);
                ++i;
            }
            if (rawData[i] == OS_TOK_DECIMAL_POINT) {
                double multiplier = 0.1;
                ++i;
                while (IsNumTok(rawData[i])) {
                    val += multiplier * TokToInt(rawData[i]);
                    multiplier *= 0.1;
                }
            }
            --i;  // for loop will increment it again, at this point we are already a token ahead of the processed input
            tokens[tokIdx].SetVal(val);
            ++tokIdx;
        } else if (tok == OS_TOK_2BYTE) {
            if (rawData[i + 1] == OS_TOK_CONST_E) {
                tokens[tokIdx].SetCode(OS_TOK_E);
                ++tokIdx;
                ++i;
            }
        } else if (tok == OS_TOK_2BYTE_EXT) {
            if (rawData[i + 1] == 0x2E) {
                tokens[tokIdx].SetCode(OS_TOK_DIVIDE);
                ++tokIdx;
                ++i;
            }
        }
    }
}

int Parser::TokToInt(uint8_t tok) {
    return tok - OS_TOK_0;
}

bool Parser::IsNumTok(uint8_t tok) {
    return OS_TOK_0 <= tok && tok <= OS_TOK_9;
}

void Parser::ConsumeToken() {
    ++location;
}

int Parser::ExpectToken(uint8_t code) {
    if (tokens[location].isEquivalentTo(code)) {
        ConsumeToken();
        return 0;
    } else {
        return 1;
    }
}

// M -> Expression M'
AST* Parser::START() {
    AST* expr1 = EXPRESSION_HEAD();
    AST_Triple_Expr* res = TAIL();
    if (res == nullptr) {
        return expr1;
    } else {
        res->x = expr1;
        return res;
    }
}

// M' -> OS_TOK_COMMA Expression OS_TOK_COMMA Expression | empty
AST_Triple_Expr* Parser::TAIL() {
    if (tokens[location].isEquivalentTo(OS_TOK_COMMA)) {
        // M' -> OS_TOK_COMMA Expression OS_TOK_COMMA Expression
        ConsumeToken();
        AST* expr2 = EXPRESSION_HEAD();
        if (expr2 == nullptr || ExpectToken(OS_TOK_COMMA) != 0) {
            dbg_printf("Invalid syntax when parsing triple expression\n");
            return nullptr;
        }
        AST* expr3 = EXPRESSION_HEAD();
        if (expr3 == nullptr) {
            dbg_printf("Invalid syntax when parsing triple expression\n");
            return nullptr;
        }
        AST_Triple_Expr* res = new AST_Triple_Expr(nullptr, expr2, expr3);
        return res;
    } else {
        // M' -> empty
        return nullptr;
    }
}

// Expression -> Term (Expression')*
AST* Parser::EXPRESSION_HEAD() {
    AST* left = TERM_HEAD();
    if (left == nullptr) {
        dbg_printf("Error parsing expression head: could not find term.\n");
        return nullptr;
    }
    if (tokens[location].tokenType < TT_B1 || tokens[location].isEquivalentTo(OS_TOK_RIGHT_PAREN)) {
        return left;
    }
    ASTBinOp* root = EXPRESSION_TAIL();
    if (root == nullptr) {
        dbg_printf("Error parsing expression head: expected expression tail.\n");
        delete left;
        return nullptr;
    }
    root->left = left;
    while (tokens[location].tokenType == TT_B1) {
        ASTBinOp* eTail = EXPRESSION_TAIL();
        if (eTail == nullptr) {
            dbg_printf("Error parsing expression, expression tail was in an incorrect format.\n");
            delete root;
            return nullptr;
        }
        eTail->left = root;
        root = eTail;
    }
    return root;
}

// Expression' -> B1 Term | empty
// this will return a binary op node whose left pointer is null and whose right pointer points to T
ASTBinOp* Parser::EXPRESSION_TAIL() {
    if (tokens[location].tokenType < TT_B1) {  // comma or end token
        return nullptr;                        // E' -> empty
    }
    // E' -> B1 T
    ASTBinOp* op = B1();
    AST* right = TERM_HEAD();
    if (op == nullptr || right == nullptr) {
        dbg_printf("Error in parsing expression tail, could not successfully find B1 and TERM.\n");
        delete op;  // just to be safe
        delete right;
        return nullptr;
    }
    op->right = right;
    return op;
}

// B1 -> OS_TOK_ADD | OS_TOK_SUBTRACT
ASTBinOp* Parser::B1() {
    if (tokens[location].tokenType != TT_B1) {
        dbg_printf("Error parsing B1, expected OS_TOK_ADD or OS_TOK_SUBTRACT but got token of type %d.\n", tokens[location].tokenType);
        return nullptr;
    }
    ASTBinOp* res = new ASTBinOp(nullptr, nullptr, tokens[location].getCode());
    ConsumeToken();
    return res;
}

// Term -> Factor (Term')*
AST* Parser::TERM_HEAD() {
    AST* left = FACTOR_HEAD();
    if (left == nullptr) {
        dbg_printf("Error parsing term head: could not find left factor.\n");
        return nullptr;
    }
    if (tokens[location].tokenType < TT_B2 || tokens[location].isEquivalentTo(OS_TOK_RIGHT_PAREN)) {  // no extra factors to grab
        return left;
    }
    ASTBinOp* root = TERM_TAIL();
    if (root == nullptr) {
        dbg_printf("Error parsing term: expected term tail.\n");
        delete left;
        return nullptr;
    }
    root->left = left;
    while (tokens[location].tokenType >= TT_B2 && !tokens[location].isEquivalentTo(OS_TOK_RIGHT_PAREN)) {
        ASTBinOp* tTail = TERM_TAIL();
        if (tTail == nullptr) {
            dbg_printf("Error parsing term: could not find term tail\n");
            delete root;
            return nullptr;
        }
        tTail->left = root;
        root = tTail;
    }
    return root;
}

// Term' -> B2 Factor | Factor | empty
ASTBinOp* Parser::TERM_TAIL() {
    if (tokens[location].tokenType < TT_B2 || tokens[location].isEquivalentTo(OS_TOK_RIGHT_PAREN)) {  // if there is a B1 token or a comma token, then T' -> empty
        return nullptr;
    } else if (tokens[location].tokenType == TT_B2) {
        // T' -> B2 F
        ASTBinOp* op = B2();
        AST* right = FACTOR_HEAD();
        if (right == nullptr) {  // op can't be nullptr because we just checked that the current token was a B2 operator
            dbg_printf("Error parsing term tail: could not find right factor.\n");
            delete op;
            return nullptr;
        }
        op->right = right;
        return op;
    } else {
        // T' -> F (implied multiplication)
        AST* right = FACTOR_HEAD();
        if (right == nullptr) {
            dbg_printf("Error parsing term tail: could not find factor for implied multiplication.\n");
            return nullptr;
        }
        ASTBinOp* op = new ASTBinOp(nullptr, right, OS_TOK_MULTIPLY);
        return op;
    }
}

ASTBinOp* Parser::B2() {
    if (tokens[location].tokenType != TT_B2) {
        dbg_printf("Error parsing B2, expected OS_TOK_MULTIPLY or OS_TOK_DIVIDE but got token of type %d.\n", tokens[location].tokenType);
        return nullptr;
    }
    ASTBinOp* res = new ASTBinOp(nullptr, nullptr, tokens[location].getCode());
    ConsumeToken();
    return res;
}

// Factor -> Base F'
AST* Parser::FACTOR_HEAD() {
    ASTBinOp* neg = nullptr;
    if (tokens[location].isEquivalentTo(OS_TOK_NEGATIVE)) {
        neg = new ASTBinOp(nullptr, new ASTConst(-1.0), OS_TOK_MULTIPLY);
        ConsumeToken();
    }
    AST* left = BASE();
    if (left == nullptr) {
        dbg_printf("Error parsing factor: could not find base.\n");
        delete neg;
        return nullptr;
    }
    ASTBinOp* op = FACTOR_TAIL();
    if (op == nullptr) {
        if (neg != nullptr) {
            neg->left = left;
            return neg;
        }
        return left;
    }
    op->left = left;
    if (neg != nullptr) {
        neg->left = op;
        return neg;
    }
    return op;
}

// Factor' -> OS_TOK_SQUARE | OS_TOK_CUBE | OS_TOK_RECIPROCAL | OS_TOK_POWER OS_TOK_LEFT_PAREN Expression OS_TOK_RIGHT_PAREN | empty
ASTBinOp* Parser::FACTOR_TAIL() {
    if (tokens[location].isEquivalentTo(OS_TOK_SQUARE)) {
        ConsumeToken();
        return new ASTBinOp(nullptr, new ASTConst(2.0), OS_TOK_POWER);
    } else if (tokens[location].isEquivalentTo(OS_TOK_CUBE)) {
        ConsumeToken();
        return new ASTBinOp(nullptr, new ASTConst(3.0), OS_TOK_POWER);
    } else if (tokens[location].isEquivalentTo(OS_TOK_RECIPROCAL)) {
        ConsumeToken();
        return new ASTBinOp(nullptr, new ASTConst(-1.0), OS_TOK_POWER);
    } else if (tokens[location].isEquivalentTo(OS_TOK_POWER)) {
        ConsumeToken();
        int err = ExpectToken(OS_TOK_LEFT_PAREN);
        if (err == 1) {
            dbg_printf("Error parsing exponent: could not find left parenthesis.\n");
            return nullptr;
        }
        AST* right = EXPRESSION_HEAD();
        if (right == nullptr) {
            dbg_printf("Error parsing exponent: could not find expression.\n");
            return nullptr;
        }
        err = ExpectToken(OS_TOK_RIGHT_PAREN);
        if (err == 1) {
            dbg_printf("Error parsing exponent: could not find closing parenthesis.\n");
            delete right;
            return nullptr;
        }
        return new ASTBinOp(nullptr, right, OS_TOK_POWER);
    } else {
        // Factor' -> empty
        return nullptr;
    }
}

// Base -> Number | Variable | OS_TOK_E | OS_TOK_PI | Func Expression OS_TOK_RIGHT_PAREN | OS_TOK_LEFT_PAREN Expression OS_TOK_RIGHT_PAREN
AST* Parser::BASE() {
    if (tokens[location].tokenClass == NUM_TOKEN) {
        ASTConst* ret = new ASTConst(tokens[location].getVal());
        ConsumeToken();
        return ret;
    } else if (tokens[location].tokenType == TT_VAR) {
        AST* ret = VARIABLE();  // no nullptr check necessary because we just checked that it is a variable token
        return ret;
    } else if (tokens[location].isEquivalentTo(OS_TOK_E)) {
        ConsumeToken();
        return new ASTConst(2.7182818284);
    } else if (tokens[location].isEquivalentTo(OS_TOK_PI)) {
        ConsumeToken();
        return new ASTConst(3.1415926536);
    } else if (tokens[location].tokenType == TT_FUNC) {
        ASTFunc* func = FUNC();
        AST* operand = EXPRESSION_HEAD();
        if (operand == nullptr) {
            dbg_printf("Error parsing function body: could not find expression.\n");
            delete func;
            return nullptr;
        }
        int err = ExpectToken(OS_TOK_RIGHT_PAREN);
        if (err == 1) {
            dbg_printf("Error parsing function: could not find right parenthesis.\n");
            delete func;
            delete operand;
            return nullptr;
        }
        func->operand = operand;
        return func;
    } else {
        int err = ExpectToken(OS_TOK_LEFT_PAREN);
        if (err == 1) {
            dbg_printf("Error parsing base: expected left parenthesis at %d.\n", location);
            return nullptr;
        }
        AST* expr = EXPRESSION_HEAD();
        if (expr == nullptr) {
            dbg_printf("Error parsing base: expected expression inside parentheses.\n");
            return nullptr;
        }
        err = ExpectToken(OS_TOK_RIGHT_PAREN);
        if (err == 1) {
            dbg_printf("Error parsing base: expected right parenthesis.\n");
            delete expr;
            return nullptr;
        }
        return expr;
    }
}

ASTVar* Parser::VARIABLE() {
    if (tokens[location].tokenType != TT_VAR) {
        dbg_printf("Error: expected variable\n");
        return nullptr;
    }
    uint8_t code = tokens[location].getCode();
    ConsumeToken();
    return new ASTVar(code);
}

ASTFunc* Parser::FUNC() {
    if (tokens[location].tokenType != TT_FUNC) {
        dbg_printf("Error: Expected func token.\n");
        return nullptr;
    }
    uint8_t op = tokens[location].getCode();
    ConsumeToken();
    return new ASTFunc(op, nullptr);
}
}  // namespace tiparser
