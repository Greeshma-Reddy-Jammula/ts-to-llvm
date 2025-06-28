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
