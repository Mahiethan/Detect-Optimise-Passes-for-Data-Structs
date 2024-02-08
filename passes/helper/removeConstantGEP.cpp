#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace std;

LLVMContext helper_Context;
IRBuilder<> helper_Builder(helper_Context);

namespace {

//For Function Pass, use run(Function &F, FunctionAnalysisManager &FM)
struct removeConstantGEP : public PassInfoMixin<removeConstantGEP> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        ConstantExpr* foundConstantGEP = nullptr;
        Instruction* loadedGEP = nullptr;
        Type* loadedGEPType = nullptr;

        DataLayout* TD = new DataLayout(&M);
        int constantGEPCount = 0;

        for (auto &F : M) 
        { 
            for (auto &B : F)  
            { 
                bool foundAoS = false;
                bool lookForStore = false;
                GetElementPtrInst* bitfieldGEP;
                Type* actualType;
                for (auto &I : B) /// iterate through all instructions
                {
                    if(lookForStore == true) //if a value is being stored in a bitfield of a struct, may need to change its type
                    {
                      if(auto *SI = dyn_cast<StoreInst>(&I)) //find a Store instruction for a bitfield
                      {
                        Value* operand = SI->getOperand(0); //get operand that is being stored in bitfield
                        
                        int origSize = TD->getTypeSizeInBits(operand->getType())/8; //get size of this operand to be stored

                        int newSize = TD->getTypeSizeInBits(actualType)/8; //get intended size that should be stored in bitfield


                        if(origSize != newSize) //change store operand only if allocated size is greater than actual size - to make sure the data fits in a cache line without overflowing - it doesn't matter if the size is less that actual, this is guaranteed to fit
                        {
                          TruncInst* newTrunc = new TruncInst(operand,Type::getIntNTy(helper_Context,newSize*8),"",&I); 
                          SI->setOperand(0,newTrunc);
                        }

                        lookForStore = false;
                      }
                    }
                    if(foundConstantGEP == nullptr & loadedGEP == nullptr)
                    {
                      if(auto *LI = dyn_cast<LoadInst>(&I))
                      {
                        // LI->print(errs());
                        // errs()<<"\n";
                        Value* operand = LI->getPointerOperand();
                        // operand->print(errs());
                        // errs()<<"\n";

                        if(auto *CGEP = dyn_cast<ConstantExpr>(operand))
                        {
                          // CGEP->print(errs());
                          // errs()<<"\nyes\n";
                          // Instruction * inst = CGEP->getAsInstruction();
                          // inst->print(errs());
                          // llvm::ReplaceInstWithInst(&I,inst);
                          // CGEP->getAsInstruction();

                          //save this CGEP in a global variable
                          foundConstantGEP = CGEP;
                          loadedGEP = LI;
                          loadedGEPType = LI->getType();

                          //continue to next instruction, which should be a callInst, ONLY DO THIS is CGEP is NOT nullptr
                          //get all function arguments
                          //if a function argument matches this CGEP, replace that function argument with this
                          
                          //get the replaced instruction before replacement
                            //get all of its uses
                            //if none, remove it using removeParent() - hopefully works

                        }

                        // if(auto *GEP = dyn_cast<GEPOperator>(operand))
                        // {
                        //   // if(GEP->getSourceElementType() == allStructs.at(i))
                        //   // {
                        //     errs()<<"yhh\n";
                        //     Value* ptrOperand = GEP->getPointerOperand();
                            
                        //     // GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(reorder_Context,APInt(32,7)));
                        //     // GEP->print(errs());

                        //   // }
                        //   // GEP->getSourceElementType()->print(errs());
                        //   // errs()<<"\n";

                        // }
                        // else
                        // {
                        //   errs()<<"no\n";
                        // }
                      }
                    }
                    else
                    {
                      bool proceed = false;
                      int operandIndex = 0;
                      bool isCallInst = false;
                      bool isCastInst = false;

                      if(auto *CI = dyn_cast<CallInst>(&I))
                      {
                        for(auto arg = CI->arg_begin(); arg != CI->arg_end(); arg++) //get all function arguments
                        {
                          Value* argument = arg->get(); //get argument o
                          if(argument == loadedGEP)
                          {
                            proceed = true;
                            isCallInst = true;
                            break;
                          }
                          operandIndex++;
                        }
                      }
                      else if(auto *CI = dyn_cast<CastInst>(&I))
                      {
                        if(CI->getOperand(0) == loadedGEP)
                        {
                          proceed = true;
                          isCastInst = true;
                          operandIndex = 0;
                        }
                      }

                      if(proceed)
                      {
                        Instruction* newInst = foundConstantGEP->getAsInstruction(&I);
                        GetElementPtrInst* newGEP = cast<GetElementPtrInst>(newInst);
                        GetElementPtrInst* updatedGEP = nullptr;
                        LoadInst* newLoad = nullptr;

                        // errs()<<newGEP->getNumOperands()<<"\n";

                        //if there is one less operand in GEP, add i32 0 as last operand
                          //cant really add a new operand (out of range error)
                          // newGEP->setOperand(4,ConstantInt::get(reorder_Context,APInt(32,0)));
                          
                        //so create a new GEP, copying all the operands from the old GEP to this one
                        vector<Value*> indices;
                        int operandCount = 0;
                        for(int i = 1; i < 3; i++)
                        {
                          // newGEP->getOperand(i)->print(errs());
                          // errs()<<"\n";
                          indices.push_back(newGEP->getOperand(i));
                        }
                        // indices.push_back(ConstantInt::get(reorder_Context,APInt(32,0)));
                        ArrayRef<Value*> indexList = ArrayRef(indices);

                        Type* elemType;

                        if(auto *AT = dyn_cast<ArrayType>(newGEP->getSourceElementType()))
                        {
                          elemType = AT->getArrayElementType();
                          if(!elemType->isStructTy())
                            break;
                        }
                        else
                          break;

                        
                        GetElementPtrInst* sourceGEP = GetElementPtrInst::CreateInBounds(newGEP->getSourceElementType(), newGEP->getOperand(0), indexList, "", &I);
                        
                        indices.clear();
                        
                        indices.push_back(ConstantInt::get(helper_Context,APInt(32,0)));

                        if(newGEP->getNumOperands() == 3)
                        {
                          indices.push_back(ConstantInt::get(helper_Context,APInt(32,0)));
                        }
                        else
                          indices.push_back(newGEP->getOperand(3));

                        indexList = ArrayRef(indices);

                        updatedGEP = GetElementPtrInst::CreateInBounds(elemType, sourceGEP, indexList, "", &I);
                          
                          newLoad = new LoadInst(loadedGEPType,updatedGEP,"",&I);

                        (&I)->setOperand(operandIndex,newLoad);

                        //removing old GEP if not used elsewhere
                        if(newInst->getNumUses() == 0)
                          newInst->eraseFromParent();
        
                        //remove the loadInst
                        if(loadedGEP->getNumUses() == 0)
                            loadedGEP->eraseFromParent();

                        foundConstantGEP = nullptr;
                        loadedGEP = nullptr;
                        constantGEPCount++;
                      }
                    }

                }
            }
        }
        
        errs()<<"\n-------------------------- REMOVED "<<constantGEPCount<<" CONSTANT GEP INSTRUCTIONS --------------------------\n\n";
        //Set to ::all() if IR is unchanged, otherwise ::none()
        return PreservedAnalyses::none();
    };
};

}