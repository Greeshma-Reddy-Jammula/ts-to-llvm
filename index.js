const babel = require('./babel_parser');
const addon = require('./build/Release/addon');

const ast = babel.parseTSFile('example.ts');
addon.compile(JSON.stringify(ast.program));  // Pass real Babel AST
