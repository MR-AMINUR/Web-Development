// console.log("Hello from VS Code!");

var myName = "Aminur";
// console.log(myName);

var1 = 8;
var2 = 5;

var3 = var1 + var2;
//console.log(var3 );

const fs = require("fs");

// fs.writeFile("message.txt", "Hello from NodeJS!", (err) => {
//     if (err) throw err;
//     console.log("The file has been saved!");
// });

fs.readFile("./message.txt", "utf8", (err, data) => {
    if (err) throw err;
    console.log(data);
});