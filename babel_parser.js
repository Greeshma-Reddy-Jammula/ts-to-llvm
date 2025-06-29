const parser = require('@babel/parser');
const fs = require('fs');

function parseTSFile(filename) {
  const code = fs.readFileSync(filename, 'utf-8');
  return parser.parse(code, {
    sourceType: 'module',
    plugins: ['typescript']
  });
}

module.exports = { parseTSFile };
