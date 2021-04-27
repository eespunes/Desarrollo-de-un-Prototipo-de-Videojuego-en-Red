let players = new Map(),
    races = [],
    playerDB = require('../Database/DatabaseController');
const Race = require("./Race");

const socket = io();

function checkRaces() {
    for (const race in races) {
        if (race.hasCapacity)
            return race.getID
    }
    return -1
}

function createRace(levelID) {
    if (!playerDB.registerRace(levelID)) {
        return -1
    }
    let race = new Race(races.length + 1, levelID);
    races.push(race)

    return race.getID;
}

function usernameInRace(username) {
    for (const player in players.keys()) {
        if (player === username)
            return true
    }
    return false
}

function getRace(raceID) {
    for (let i = 0; i < races.length; i++) {
        let race = races.pop()
        races.unshift(race)
        if (race.getID === raceID)
            return race
    }
    return null;
}

exports.addPlayerToRace = function (username, levelID, res) {
    let raceID = checkRaces();

    if (raceID === -1) {
        raceID = createRace(levelID);
        if (raceID === -1) {
            res.sendStatus(417)
            return;
        }
    }

    if (usernameInRace(username)) {
        res.sendStatus(409)
        return;
    }

    let addedCorrectly = playerDB.addPlayerToRace(username, raceID);
    if (!addedCorrectly) {
        res.sendStatus(417)
        return;
    }

    console.log("Player Added to the race")

    players.set(username, raceID)
    let socketID = getRace(raceID).addPlayer(username)
    res.status(200).send(socketID)
};

exports.removePlayerFromRace = function (username, res) {
    if (!usernameInRace(username)) {
        res.sendStatus(409)
        return;
    }
    let race = getRace(players.get(username))
    race.removePlayer(username)
}
