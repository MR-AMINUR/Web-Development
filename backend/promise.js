const promiseOne = new Promise(function(resolve, reject) {
    // Do an async task
    // DB calls, cryptography, network
    setTimeout(() => {
        console.log('Async task is complete');
        resolve()
    }, 1000)
})

promiseOne.then(() => {
    console.log("Promise consumed");
})

new Promise((resolve, reject) => {
    setTimeout(() => {
        console.log("Async task 2");
        resolve()
    }, 1000)
}).then(() => {
    console.log("Async 2 resolved");
})

const promiseThree = new Promise((resolve, reject) => {
    setTimeout(() => {
        resolve({username: "Chai", email: "for@example.com"})
    }, 1000)
})

promiseThree.then((user) => {
    console.log(user)
})