# ts-to-llvm
# TypeScript to LLVM IR Compiler

A  compiler that reads a small TypeScript-like language and emits LLVM IR using the LLVM C++ API.

Input

      ```ts
      let x = 5;
      if (x % 2 == 0) {
        print("Even");
      } else {
        print("Odd");
      }
How to Build
  Install dependencies:
                        sudo apt install llvm clang cmake g++ nodejs npm
                        npm install -g node-gyp
                        npm install
Build LLVM project:

                      mkdir build && cd build
                      cmake ..
                      make
How to Run
      
# Step 1: Generate JSON from TypeScript-like code
              node parser.js

# Step 2: Run the LLVM code generator
              ./build/ts_to_llvm
Example File Tree After Running

                  ts-to-llvm/
                  ├── build/
                  │   ├── ts_to_llvm      ← compiled executable
                  │   ├── output.ll       ← LLVM IR file 

How to View the IR

      cat build/output.ll
or open it in any code editor.

 Sample Output (output.ll)
llvm

                  ; ModuleID = 'ts-to-llvm'
                  define i32 @main() {
                  entry:
                    %x = alloca i32
                    store i32 5, i32* %x
                    ...
                    call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.Even, i32 0, i32 0))
                    ret i32 0
                  }
