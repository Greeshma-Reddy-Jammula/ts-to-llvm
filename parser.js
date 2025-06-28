const fs = require('fs');

const output = [
  { type: "let", name: "x", value: 5 },
  {
    type: "if",
    condition: { left: "x", op: "%==0", right: 2 },
    then: { print: "Even" },
    else: { print: "Odd" }
  }
];

fs.writeFileSync("parsed.json", JSON.stringify(output, null, 2));
console.log("Parsed successfully.");
