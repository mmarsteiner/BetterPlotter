#pragma once

#include <debug.h>
#include <ti/tokens.h>

#include <cmath>
#include <cstdint>

namespace tiparser {

typedef double (*BinOpPtr)(double, double);
typedef double (*UOpPtr)(double);

// all instances of AST must be created with the new keyword
struct AST {
    virtual ~AST() = default;
    virtual void dbgPrint() = 0;
    virtual double Eval(size_t numVars, const uint8_t* vars, const double* vals) const = 0;
    static void InitOpPtrs();

    protected:
    static BinOpPtr binOps[256];
    static UOpPtr uOps[256];
};
struct AST_Triple_Expr : AST {  // so parametric equations can be typed as a comma separated list of 3 expressions
    AST* x;
    AST* y;
    AST* z;
    AST_Triple_Expr(AST* x, AST* y, AST* z) : x{x}, y{y}, z{z} {}
    void dbgPrint() override {
        x->dbgPrint();
        dbg_printf(", ");
        y->dbgPrint();
        dbg_printf(", ");
        z->dbgPrint();
    }
    double Eval(size_t, const uint8_t*, const double*) const override {
        return 0.0 / 0.0;  // will return NaN because this is a triple expression
    }
    // out must be able to hold at least 3 values
    void TripleEval(size_t numVars, const uint8_t* vars, const double* vals, double* out) const {
        out[0] = x->Eval(numVars, vars, vals);
        out[1] = y->Eval(numVars, vars, vals);
        out[2] = z->Eval(numVars, vars, vals);
    }
};
struct ASTBinOp : AST {
    AST* left;
    AST* right;
    uint8_t op;
    ASTBinOp(AST* left, AST* right, uint8_t op) : left{left}, right{right}, op{op} {}
    ~ASTBinOp() override {
        delete left;
        delete right;
    }
    void dbgPrint() override {
        dbg_printf("0x%X(", op);
        if (left == nullptr) {
            dbg_printf("nullptr");
        } else {
            left->dbgPrint();
        }
        dbg_printf(", ");
        if (right == nullptr) {
            dbg_printf("nullptr");
        } else {
            right->dbgPrint();
        }
        dbg_printf(")");
    }
    double Eval(size_t numVars, const uint8_t* vars, const double* vals) const override {
        double lVal = left->Eval(numVars, vars, vals);
        double rVal = right->Eval(numVars, vars, vals);
        return binOps[op](lVal, rVal);
    }
};
struct ASTFunc : AST {
    AST* operand;
    uint8_t op;
    ASTFunc(uint8_t op, AST* operand) : operand{operand}, op{op} {}
    ~ASTFunc() override {
        delete operand;
    }
    void dbgPrint() override {
        dbg_printf("0x%X(", op);
        operand->dbgPrint();
        dbg_printf(")");
    }
    double Eval(size_t numVars, const uint8_t* vars, const double* vals) const override {
        double operandVal = operand->Eval(numVars, vars, vals);
        return uOps[op](operandVal);
    }
};
struct ASTVar : AST {
    uint8_t var;
    ASTVar(uint8_t code) : var{code} {}
    void dbgPrint() override {
        dbg_printf("0x%X", var);
    }
    double Eval(size_t numVars, const uint8_t* vars, const double* vals) const override {
        for (size_t i = 0; i < numVars; i++) {
            if (vars[i] == var) {
                return vals[i];
            }
        }
        return 0.0 / 0.0;
    }
};
struct ASTConst : AST {
    double val;
    ASTConst(double val) : val{val} {}
    void dbgPrint() override {
        dbg_printf("%.4f", val);
    }
    double Eval(size_t, const uint8_t*, const double*) const override {
        return val;
    }
};
}  // namespace tiparser