let playersMap = new Map(),
    races = [],
    racesLength = 0,
    playerDB = require('../Database/DatabaseController');
const Race = require("./Race");

function checkRaces() {
    for (let i = 0; i < races.length; i++) {
        let race = races.pop()
        races.unshift(race)
        if (race.hasCapacity) {
            return race.getID
        }
    }
    return -1;
}

function createRace(levelID, io) {
    if (!playerDB.registerRace(levelID)) {
        return -1
    }
    racesLength++
    let race = new Race(racesLength, levelID, io);
    races.push(race)

    return race.getID;
}

function usernameInRace(username, raceID) {
    const race = getRace(raceID);
    if (!race) return false;
    return race.hasPlayer(username);
}

function getRace(raceID) {
    for (let i = 0; i < races.length; i++) {
        let race = races.pop()
        races.unshift(race)
        if (race.getID === raceID) {
            return race
        }
    }
    return null;
}

exports.addPlayerToRace = function (username, levelID, io) {
    let raceID = checkRaces();
    if (raceID === -1) {
        raceID = createRace(levelID, io);
        if (raceID === -1) {
            return '{ "status":"417", "race":""}';
        }
    }
    if (usernameInRace(username, raceID)) {
        return '{ "status":"409", "race":""}';
    }
    let addedCorrectly = playerDB.addPlayerToRace(username, raceID);
    if (!addedCorrectly) {
        return '{ "status":"417", "race":""}';
    }

    getRace(raceID).addPlayer(username)
    playersMap.set(username, raceID)

    return '{ "status":200, "race":"' + raceID + '"}'
};

exports.removePlayerFromRace = function (username, raceID) {
    let isInRace = usernameInRace(username, raceID);
    if (isInRace === false) {
        return '{ "status":"409"}';
    }
    getRace(raceID).removePlayer(username)
    return '{ "status":"200"}';
}

exports.getPlayersFromRace = function (raceID) {
    let race = getRace(raceID);
    if (race !== null)
        return race.getPlayers;
    else return []
}

exports.getAllRaces = function () {
    let returnRaces = []
    for (let i = 0; i < races.length; i++) {
        let race = races.pop()
        races.unshift(race)
        returnRaces.push(race.getID)
    }
    return returnRaces;
}

exports.addMessageToThePlayer = function (raceID, username, message) {
    getRace(raceID).addMessageToThePlayer(username, message)
}
