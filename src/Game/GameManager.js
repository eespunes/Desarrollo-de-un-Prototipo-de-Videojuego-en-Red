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

function createRace(levelID) {
    if (!playerDB.registerRace(levelID)) {
        return -1
    }
    racesLength++
    let race = new Race(racesLength, levelID);
    races.push(race)

    return race.getID;
}

function usernameInRace(username) {
    // for (let i = 0; i < races.length; i++) {
    //     let race = races.pop()
    //     races.unshift(race)
    //     if (race.hasPlayer(username)) {
    //         return race.getID
    //     }
    // }
    return -1;
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

exports.addPlayerToRace = function (username, levelID) {
    let raceID = checkRaces();

    if (raceID === -1) {
        raceID = createRace(levelID);
        if (raceID === -1) {
            return '{ "status":"417", "race":""}';
        }
    }
    if (usernameInRace(username) !== -1) {
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

exports.removePlayerFromRace = function (username, res) {
    let raceID = usernameInRace(username);
    if (raceID === -1) {
        return '{ "status":"409"}';
    }
    let race = getRace(raceID)
    race.removePlayer(username)
    return '{ "status":"200"}';
}

exports.getPlayersFromRace = function (raceID) {
    return getRace(raceID).getPlayers;
}
