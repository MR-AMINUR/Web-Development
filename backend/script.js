// console.log("Aminur");
const accountId = 12234 
let email = "aminur@gmail.com"
var password = "0000"
// console.log(accountId);

/*
    Prefer not to use var
    because of issue in block scope and functional scope 
*/ 

//console.table([accountId, email, password])

/* ---------------Conversion Operation --------------------*/
let score = "33abc"

//console.log(typeof score);
//console.log(typeof(score));


let valueInNumber = Number(score)
//console.log(typeof valueInNumber);
//console.log(valueInNumber);

// "33" => 33
// "33abc" => NaN
// true => 1; false => 0

let isLoggedIn = 1

let booleanIsLoggedIn = Boolean(isLoggedIn)
//console.log(booleanIsLoggedIn);

// 1 -> true 0 -> false
// Empty String -> false
// full String -> true 

/* --------------- Operations ----------------*/

let value = 3
let neg = -value
// console.log(neg);

// console.log("1"+ 2 + 2);
// console.log(1+ 2 + "2");    // Bad practice , use parentheses instead

// Strict Comparator

//console.log("2" === "2")

// Primitive 

// 7 types : String, Number, Boolean, null, undefined, Symbol, BigInt
const id = Symbol('123')
const anotherId = Symbol('123')

//console.log(id === anotherId);
// Reference (Non primitive)

// Array, Objects, Functions

const heros = ["Ironman", "Captain America", "Black Widow"];

// let myObj = {
//     name = "aminur",
//     age:23,
// }

const myFunction = function () {
    console.log("Hello India");
    
}

//console.log(typeof myFunction);

/*-----------------------DataTypes----------------------*/

// Stack (Primitive), Heap (Non-Primitive)

let mymediaName = "aminur09"

let anotherName = mymediaName
anotherName = "ramiz02"

//console.log(mymediaName);
//console.log(anotherName);

let userOne = {
    email: "aminur01mallick@gmail.com",
    upi: "user@okhdfcbank"
}

let userTwo = userOne
userTwo.email = "aminur09rahaman@gmail.com"

//console.log(userOne.email);
//console.log(userTwo.email);

/*---------------------Strings---------------------*/

const gameName = new String('AminurRM')

// console.log(gameName[0]);
// console.log(gameName.__proto__);

// console.log(gameName.length);
// console.log(gameName.toUpperCase());

// const newString = gameName.substring(-5, 4)
// console.log(newString);

// const anotherString = gameName.slice(3, 1);
// console.log(anotherString);
 
/*----------------Numbers & Maths--------------------- */

const marks = 400
//console.log(marks);

const balance = new Number(100)
//console.log(balance);

//console.log(balance.toString().length);
//console.log(balance.toFixed(1));

const otherNumber = 123.8954

//console.log(otherNumber.toPrecision(4));

//console.log(Math.min(4, 3, 6, 8));

// console.log(Math.random());
// console.log((Math.random()*10) + 1 );
// console.log(Math.floor(Math.random()*10)+1);

const min = 10
const max = 20

//console.log(Math.floor(Math.random() * (max - min + 1)) + min)

/*------------------Functions & Parameters--------------*/

function addTwoNumbers( number1,  number2) {

    return number1 + number2
}

//console.log(addTwoNumbers(4, 5))

function loginUserMessage(username) {
    return `${username} good to see you`
}

//console.log(loginUserMessage("Aminur")) 
// "..." --> rest or spread operator 
function calculateCartPrice(...num1) {
    return num1
}

//console.log(calculateCartPrice(200, 400, 600, 900));

const myArray = [200, 400, 877, 666]

function returnSecondValue(getArray) {
    return getArray[1]
}

//console.log(returnSecondValue(myArray));

const user = {
    username : "Aminur",
    prices: 200
}

function handleObject(anyObject) {
    console.log(`Username is ${anyObject.username} and price is ${anyObject.prices}`);
}

handleObject(user)

