// console.log("Hello from VS Code!");

// var myName = "Aminur";
// // console.log(myName);

// var1 = 8;
// var2 = 5;

// var3 = var1 + var2;
//console.log(var3 );

//const fs = require("fs");

// fs.writeFile("message.txt", "Hello from NodeJS!", (err) => {
//     if (err) throw err;
//     console.log("The file has been saved!");
// });

// fs.readFile("./message.txt", "utf8", (err, data) => {
//     if (err) throw err;
//     console.log(data);
// });

// import superman from "superman";

// const name = superman.random();

// console.log(`I am ${name}!`);

/*
1. Use the inquirer npm package to get user input.
2. Use the qr-image npm package to turn the user entered url into a QR code image.
3. Create a txt file to save the user input using the native fs node module.
*/ 

// import inquirer from 'inquirer';
// import qr from "qr-image";
// import fs from "fs";

// inquirer
//   .prompt([
//     {
//         message: "Type in your URL: ",
//         name: "URL",
//     },
//   ])
//   .then((answers) => {
//     const url = answers.URL;
//     var qr_svg = qr.image(url);
//     qr_svg.pipe(fs.createWriteStream("qr_img.png"));
//   })
//   .catch((error) => {
//     if (error.isTtyError) {
//       // Prompt couldn't be rendered in the current environment
//     } else {
//       // Something else went wrong
//     }
//   });

var a = 5;
var b = 7;
var c = a / b;
console.log(c);