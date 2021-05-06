// const util = require("util");
// const {parentPort, workerData} = require("worker_threads");
const io = require("../index")();

let id = "",
    levelId = "",
    started = false,
    players = [],
    time = 60;

process.on("message", data => {
    console.log("\t\tMessage arrived!!")
    const messageType = data.id;
    if (messageType === 'hasCapacity') {
        console.log("hasCapacity")
        process.send( hasCapacity());
    }
    else if (messageType === 'getID') {
        console.log("getID")
        process.send({getID: getID()});
    }
    else if (messageType === 'hasPlayer') {
        console.log("hasPlayer")
        process.send(hasPlayer(data.username));
    }
    else if (messageType === 'addPlayer') {
        console.log("addPlayer")
        process.send(addPlayer(data.username));
    }
    else if (messageType === 'removePlayer') {
        console.log("removePlayer")
        process.send(removePlayer(data.username));
    }
    else if (messageType === 'getPlayers') {
        console.log("getPlayers")
        process.send({getPlayers: getPlayers()});
    }else if (messageType === 'init'){
        raceInit()
        process.send(id)
    }

});

function raceInit(raceID,levelID) {
    id = raceID;
    let idString = "R"
    if (id < 10)
        idString += "00"
    else if (id < 100)
        idString += "0"

    id = idString + id
    levelId = levelID
    racing()

    console.log(id+" race created")
}

function addPlayer(username) {
    players.push(username)
}

function hasCapacity() {
    if (started)
        return false;
    if (players.length === 12)
        return false
    return true
}

function hasPlayer(username) {
    for (let i = 0; i < players.length; i++) {
        let player = players.pop()
        players.unshift(player)
        if (player === username)
            return true
    }
    return false
}

function removePlayer(username) {
    for (let i = 0; i < players.length; i++) {
        let player = players.pop()
        if (player === username)
            return
        players.unshift(player)
    }
}

function racing() {
    searchingPlayers()
    informPlayers()
    stopRace()
}

function searchingPlayers() {
    // while (true) {
    //     console.log("caca")
    //     if (players.length > 1)
    //         setTimeout(() => {
    //             io.emit(this.id, "Buscando jugadores...")
    //         }, 100);
    //     else
    //         setTimeout(() => {
    //             io.emit(this.id, time)
    //         }, 1000);
    // }
}

function informPlayers() {
    io.emit(/* ... */);
}

function startRace() {
    this.started = true
}

function stopRace() {

}

function getID() {
    return id;
}

function getPlayers() {
    return players;
}

