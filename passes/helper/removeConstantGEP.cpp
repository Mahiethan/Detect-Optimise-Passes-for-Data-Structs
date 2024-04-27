// Removes any constant GEP instructions before applying the detection and optimisation passes
// Not removing any constant GEP instructions means that the optimisations may not be applied correctly, breaking the functionality of the program.

#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace std;

LLVMContext helper_Context;
IRBuilder<> helper_Builder(helper_Context);

namespace {

struct removeConstantGEP : public PassInfoMixin<removeConstantGEP> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        ConstantExpr* foundConstantGEP = nullptr; // stores constant GEP that will be replaced with new GEP
        Instruction* loadedGEP = nullptr; // stores LoadInst of the constant GEP that will be replaced with new LoadInst
        Type* loadedGEPType = nullptr; // attribute fetched from old LoadInst in order to create an equivalent LoadInst

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
                        Value* operand = LI->getPointerOperand();

                        if(auto *CGEP = dyn_cast<ConstantExpr>(operand))
                        {
                          //save this constant GEP in a global variable
                          foundConstantGEP = CGEP;
                          loadedGEP = LI;
                          loadedGEPType = LI->getType();
                        }
                      }
                      else if(auto *SI = dyn_cast<StoreInst>(&I))
                      {
                        Value* operand = SI->getPointerOperand();

                        if(auto *CGEP = dyn_cast<ConstantExpr>(operand))
                        {
                          //for StoreInst, we simply replace this operand with a separate GEP inst

                          Instruction* newInst = CGEP->getAsInstruction(SI);
                          GetElementPtrInst* newGEP = cast<GetElementPtrInst>(newInst);

                          if(auto *CGEP = dyn_cast<ConstantExpr>(newGEP->getPointerOperand()))
                          {
                            Instruction* newPtrInst = CGEP->getAsInstruction(newInst);
                            GetElementPtrInst* newPtrGEP = cast<GetElementPtrInst>(newPtrInst);
                            newGEP->setOperand(0,newPtrGEP);
                          }

                          SI->setOperand(SI->getPointerOperandIndex(),newInst);
                          constantGEPCount++;

                        }

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
                          Value* argument = arg->get(); //get argument
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

                        //create a new GEP, copying all the operands from the old GEP to this one
                        vector<Value*> indices;
                        int operandCount = 0;
                        for(int i = 1; i < 3; i++)
                        {
                          indices.push_back(newGEP->getOperand(i));
                        }

                        ArrayRef<Value*> indexList = ArrayRef(indices);

                        Type* elemType;

                        if(auto *AT = dyn_cast<ArrayType>(newGEP->getSourceElementType()))
                        {
                          elemType = AT->getArrayElementType();
                          if(!elemType->isStructTy())
                            break;
                        }
                        else
                        {
                          newLoad = new LoadInst(loadedGEPType,newInst,"",&I);

                          (&I)->setOperand(operandIndex,newLoad);

                          if(newInst->getNumUses() == 0)
                           newInst->eraseFromParent();
        
                          //remove the loadInst
                          if(loadedGEP->getNumUses() == 0)
                              loadedGEP->eraseFromParent();

                          constantGEPCount++;

                          break;
                        }
                        
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

                        updatedGEP = GetElementPtrInst::CreateInBounds(elemType, sourceGEP, indexList, "", &I); // create new GEP
                          
                        newLoad = new LoadInst(loadedGEPType,updatedGEP,"",&I); // create new LoadInst for the new GEP

                        (&I)->setOperand(operandIndex,newLoad);

                        //removing old GEP if not used elsewhere
                        if(newInst->getNumUses() == 0)
                          newInst->eraseFromParent();
        
                        //remove the LoadInst for the old GEP
                        if(loadedGEP->getNumUses() == 0)
                            loadedGEP->eraseFromParent();

                        // reset these temporary values
                        foundConstantGEP = nullptr;
                        loadedGEP = nullptr;

                        constantGEPCount++; // increment counter
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