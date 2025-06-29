#include <napi.h>
#include <nlohmann/json.hpp>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"

using json = nlohmann::json;
using namespace llvm;

LLVMContext Context;
IRBuilder<> Builder(Context);
std::unique_ptr<Module> TheModule;
Function *MainFunction;
std::map<std::string, AllocaInst*> NamedValues;
FunctionCallee Printf;

void initModule() {
    TheModule = std::make_unique<Module>("ts-to-llvm-addon", Context);
    FunctionType *funcType = FunctionType::get(Type::getInt32Ty(Context), false);
    MainFunction = Function::Create(funcType, Function::ExternalLinkage, "main", TheModule.get());
    BasicBlock *BB = BasicBlock::Create(Context, "entry", MainFunction);
    Builder.SetInsertPoint(BB);

    // printf setup
    Printf = TheModule->getOrInsertFunction("printf",
        FunctionType::get(IntegerType::getInt32Ty(Context), PointerType::get(Type::getInt8Ty(Context), 0), true));
}

AllocaInst *createEntryBlockAlloca(std::string VarName) {
    IRBuilder<> TmpB(&MainFunction->getEntryBlock(), MainFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(Type::getInt32Ty(Context), 0, VarName);
}

void handleLet(const json &node) {
    std::string var = node["name"];
    int value = node["value"];
    AllocaInst *alloca = createEntryBlockAlloca(var);
    Builder.CreateStore(ConstantInt::get(Type::getInt32Ty(Context), value), alloca);
    NamedValues[var] = alloca;
}

void handlePrint(const std::string &msg) {
    Value *str = Builder.CreateGlobalStringPtr(msg + "\n");
    Builder.CreateCall(Printf, str);
}

void handleIf(const json &node) {
    std::string var = node["condition"]["left"];
    std::string op = node["condition"]["op"];
    int rhs = node["condition"]["right"];

    Value *L = Builder.CreateLoad(Type::getInt32Ty(Context), NamedValues[var]);
    Value *R = ConstantInt::get(Type::getInt32Ty(Context), rhs);

    Value *Cond;
    if (op == "%==0") {
        Value *Mod = Builder.CreateSRem(L, R);
        Cond = Builder.CreateICmpEQ(Mod, ConstantInt::get(Type::getInt32Ty(Context), 0));
    } else {
        Cond = Builder.CreateICmpEQ(L, R);
    }

    BasicBlock *ThenBB = BasicBlock::Create(Context, "then", MainFunction);
    BasicBlock *ElseBB = BasicBlock::Create(Context, "else");
    BasicBlock *MergeBB = BasicBlock::Create(Context, "merge");

    Builder.CreateCondBr(Cond, ThenBB, ElseBB);

    Builder.SetInsertPoint(ThenBB);
    handlePrint(node["then"]["print"]);
    Builder.CreateBr(MergeBB);

    MainFunction->getBasicBlockList().push_back(ElseBB);
    Builder.SetInsertPoint(ElseBB);
    handlePrint(node["else"]["print"]);
    Builder.CreateBr(MergeBB);

    MainFunction->getBasicBlockList().push_back(MergeBB);
    Builder.SetInsertPoint(MergeBB);
}

Napi::Value Compile(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::string jsonStr = info[0].As<Napi::String>();
    json program = json::parse(jsonStr);

    initModule();

    for (const auto &stmt : program["body"]) {
        if (stmt["type"] == "VariableDeclaration") {
            auto decl = stmt["declarations"][0];
            handleLet({{"name", decl["id"]["name"]}, {"value", decl["init"]["value"]}});
        } else if (stmt["type"] == "IfStatement") {
            json condition;
            auto binExpr = stmt["test"];
            if (binExpr["type"] == "BinaryExpression") {
                condition = {
                    {"left", binExpr["left"]["name"]},
                    {"op", "%==0"},
                    {"right", binExpr["right"]["value"]}
                };
            }

            json thenBranch = stmt["consequent"]["expression"]["arguments"][0];
            json elseBranch = stmt["alternate"]["expression"]["arguments"][0];

            handleIf({
                {"condition", condition},
                {"then", { {"print", thenBranch["value"]} }},
                {"else", { {"print", elseBranch["value"]} }}
            });
        }
    }

    Builder.CreateRet(ConstantInt::get(Type::getInt32Ty(Context), 0));

    std::error_code EC;
    raw_fd_ostream out("output.ll", EC, sys::fs::OF_None);
    TheModule->print(out, nullptr);

    return Napi::String::New(env, "LLVM IR written to output.ll");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("compile", Napi::Function::New(env, Compile));
    return exports;
}

NODE_API_MODULE(addon, Init)
