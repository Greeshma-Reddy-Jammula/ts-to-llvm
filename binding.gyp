{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "addon.cpp" ],
      "include_dirs": ["<!(node -p \"require('node-addon-api').include\")"],
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      "cflags_cc": ["-std=c++17"],
      "libraries": ["-lLLVM"]
    }
  ]
}
