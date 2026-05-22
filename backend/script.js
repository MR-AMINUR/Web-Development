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

console.log(id === anotherId);
// Reference (Non primitive)

// Array, Objects, Functions

const heros = ["Ironman", "Captain America", "Black Widow"];

let myObj = {
    name = "aminur",
    age:23,
}

const myFunction = function () {
    console.log("Hello India");
    
}

console.log(myFunction);

