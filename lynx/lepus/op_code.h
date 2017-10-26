#ifndef LYNX_LEPUS_OP_CODE_H_
#define LYNX_LEPUS_OP_CODE_H_
namespace lepus {
    enum TypeOpCode {
        TypeOp_LoadNil = 1,             // A    A: register
        TypeOp_FillNil,                 // AB   A: start reg B: end reg [A,B)
        TypeOp_LoadBool,                // AB   A: register B: 1 true 0 false
        TypeOp_LoadInt,                 // A    A: register Next instruction opcode is const unsigned int
        TypeOp_LoadConst,               // ABx  A: register Bx: const index
        TypeOp_Move,                    // AB   A: dst register B: src register
        TypeOp_GetUpvalue,              // AB   A: register B: upvalue index
        TypeOp_SetUpvalue,              // AB   A: register B: upvalue index
        TypeOp_GetGlobal,               // ABx  A: value register Bx: const index
        TypeOp_SetGlobal,               // ABx  A: value register Bx: const index
        TypeOp_Closure,                 // ABx  A: register Bx: proto index
        TypeOp_Call,                    // ABC  A: register B: arg value count + 1 C: expected result count + 1
        TypeOp_VarArg,                  // AsBx A: register sBx: expected result count
        TypeOp_Ret,                     // AsBx A: return value start register sBx: return value count
        TypeOp_JmpFalse,                // AsBx A: register sBx: diff of instruction index
        TypeOp_JmpTrue,                 // AsBx A: register sBx: diff of instruction index
        TypeOp_JmpNil,                  // AsBx A: register sBx: diff of instruction index
        TypeOp_Jmp,                     // sBx  sBx: diff of instruction index
        TypeOp_Neg,                     // A    A: operand register and dst register
        TypeOp_Not,                     // A    A: operand register and dst register
        TypeOp_Len,                     // A    A: operand register and dst register
        TypeOp_Add,                     // ABC  A: dst register B: operand1 register C: operand2 register
        TypeOp_Sub,                     // ABC  A: dst register B: operand1 register C: operand2 register
        TypeOp_Mul,                     // ABC  A: dst register B: operand1 register C: operand2 register
        TypeOp_Div,                     // ABC  A: dst register B: operand1 register C: operand2 register
        TypeOp_Pow,                     // ABC  A: dst register B: operand1 register C: operand2 register
        TypeOp_Mod,                     // ABC  A: dst register B: operand1 register C: operand2 register
        TypeOp_Concat,                  // ABC  A: dst register B: operand1 register C: operand2 register
        TypeOp_Less,                    // ABC  A: dst register B: operand1 register C: operand2 register
        TypeOp_Greater,                 // ABC  A: dst register B: operand1 register C: operand2 register
        TypeOp_Equal,                   // ABC  A: dst register B: operand1 register C: operand2 register
        TypeOp_UnEqual,                 // ABC  A: dst register B: operand1 register C: operand2 register
        TypeOp_LessEqual,               // ABC  A: dst register B: operand1 register C: operand2 register
        TypeOp_GreaterEqual,            // ABC  A: dst register B: operand1 register C: operand2 register
        TypeOp_NewTable,                // A    A: register of table
        TypeOp_SetTable,                // ABC  A: register of table B: key register C: value register
        TypeOp_GetTable,                // ABC  A: register of table B: key register C: value register
        TypeOp_ForInit,                 // ABC  A: var register B: limit register    C: step register
        TypeOp_ForStep,                 // ABC  ABC same with TypeOp_ForInit, next instruction sBx: diff of instruction index
    };
    
    struct Instruction {
        unsigned int op_code_;
        
        Instruction() : op_code_(0) {
            
        }
        
        Instruction(TypeOpCode op_code, int a, int b, int c) : op_code_(op_code) {
            op_code_ = (op_code_ << 24) | ((a & 0xFF) << 16) | ((b & 0xFF) << 8) | (c & 0xFF);
        }
        
        Instruction(TypeOpCode op_code, int a, short b) : op_code_(op_code) {
            op_code_ = (op_code_ << 24) | ((a & 0xFF) << 16) | (static_cast<int>(b) & 0xFFFF);
        }
        
        Instruction(TypeOpCode op_code, int a, unsigned short b) : op_code_(op_code) {
            op_code_ = (op_code_ << 24) | ((a & 0xFF) << 16) | (static_cast<int>(b) & 0xFFFF);
        }
        
        void RefillsBx(short b) {
            op_code_ = (op_code_ & 0xFFFF0000) | (static_cast<int>(b) & 0xFFFF);
        }
        
    };
}

#endif
