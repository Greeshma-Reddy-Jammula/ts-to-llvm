# ts-to-llvm
# TypeScript to LLVM IR Compiler

A  compiler that reads a small TypeScript-like language and emits LLVM IR using the LLVM C++ API.

ts-to-llvm/
├── example.ts✅ fake TS input
├── parser.js ✅ converts to parsed.json (AST)
├── parsed.json ✅ used by LLVM C++ backend
├── codegen.cpp✅ LLVM IR codegen
├── CMakeLists.txt✅ LLVM build setup
├── .gitignore ✅ build ignore
└── README.md✅ usage steps

NEW Files to Add

 ts-to-llvm/
      ├── addon.cpp 🆕 C++ Node.js addon
      ├── binding.gyp🆕 node-gyp config
      ├── index.js🆕 main runner (calls addon + Babel)
      ├── babel_parser.js🆕 real Babel TypeScript parser

Final Structure

ts-to-llvm/
├── example.ts✅ input code
├── babel_parser.js🆕 Babel-based parser
├── index.js🆕 Node runner
├── addon.cpp🆕 C++ LLVM backend
├── binding.gyp🆕 node-gyp build
├── codegen.cpp🟡 optional legacy IR generator
├── parser.js✅ old fake parser
├── parsed.json✅ old AST (now optional)
├── CMakeLists.txt ✅ LLVM build (can use in addon too)
├── .gitignore✅ (add `node_modules/` and `build/`)
└── README.md✅ update to show real-time flow

✅ node-gyp → for real-time compiler using Node.js + addon.cpp
✅ cmake → for old, offline version using codegen.cpp


Input

      ```ts
      let x = 5;
      if (x % 2 == 0) {
        print("Even");
      } else {
        print("Odd");
      }
How to Build

Install All Dependencies

            # System dependencies (Ubuntu/Debian)
            sudo apt install llvm clang cmake g++ nodejs npm

            # Node.js dependencies
            npm install -g node-gyp
            npm install
            npm install node-addon-api
Build the Real-time Node Addon (LLVM + Node.js)

            npx node-gyp configure build
Run the Compiler (Realtime)

            node index.js
Build Legacy codegen.cpp using CMake
(You only need this if you want to run the old parser.js + codegen.cpp method.)

            mkdir build && cd build
            cmake ..
            make
            ./ts_to_llvm

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
                    %1 = load i32, i32* %x
                    %2 = srem i32 %1, 2
                    %3 = icmp eq i32 %2, 0
                    br i1 %3, label %then, label %else
                  then:
                    call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.Even, i32 0, i32 0))
                    br label %merge
                  else:
                    call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.Odd, i32 0, i32 0))
                    br label %merge
                  merge:
                    ret i32 0
                  }



