let playersMap = new Map(),
    races = [],
    racesLength = 0,
    playerDB = require('../Database/DatabaseController');
const {fork} = require('child_process');

getID = async function (race) {
    console.log("\tGetting ID")
    race.send({id: 'getID'});
    console.log("\tMessage sended")
    race.on("message", result => {
        console.log("\tGot Race ID:" + result.raceID)
        return result.getID;
    });
}

function checkRaces() {
    for (let i = 0; i < races.length; i++) {
        let race = races.pop()
        races.unshift(race)
        race.send({id: 'hasCapacity'});
        race.on("message", hasCapacity => {
            if (hasCapacity)
                return getID(race)
        });
    }
    return -1;
}

function createRace(levelID) {
    if (!playerDB.registerRace(levelID)) {
        return -1
    }
    racesLength++
    const race = fork("./src/Game/Race.js");
    console.log(race)
    races.push(race)
    race.send({id: 'init', raceID: racesLength, levelID: levelID});
    race.on("message", raceID => {
        console.log("\tGot Race ID:" + raceID)
        return raceID;
    });
}

function usernameInRace(username) {
    for (let i = 0; i < races.length; i++) {
        let race = races.pop()
        races.unshift(race)
        race.send({id: 'hasPlayer', username: username});
        race.on("message", hasPlayer => {
            if (hasPlayer)
                return getID(race)
        });
    }
    return -1;
}

function getRace(raceID) {
    for (let i = 0; i < races.length; i++) {
        let race = races.pop()
        races.unshift(race)
        if (getID(race) === raceID)
            return race
    }
    return null;
}

exports.addPlayerToRace = function (username, levelID, io) {
    console.log("Checking Races")
    let raceID = checkRaces();
    if (raceID === -1) {
        console.log("Creating the race")
        raceID = createRace(levelID, io);
        console.log("Race Created: " + raceID)
        if (raceID === -1) {
            return '{ "status":"417", "race":""}';
        }
    }
    console.log("RaceID: " + raceID)
    // if (usernameInRace(username) !== -1) {
    //     return '{ "status":"409", "race":""}';
    // }
    //
    // let addedCorrectly = playerDB.addPlayerToRace(username, raceID);
    // if (!addedCorrectly) {
    //     return '{ "status":"417", "race":""}';
    // }
    //
    // const race = getRace(raceID);
    // race.postMessage({id: 'addPlayer', username: username});
    // race.on("message", result => {
    //     return result.addPlayer;
    // });
    // playersMap.set(username, raceID)
    //
    return '{ "status":200, "race":"' + raceID + '"}'
};

exports.removePlayerFromRace = function (username, res) {
    let raceID = usernameInRace(username);
    if (raceID === -1) {
        return '{ "status":"409"}';
    }

    let race = getRace(raceID)
    race.send({id: 'removePlayer', username: username});
    race.on("message", removePlayer => {
        return removePlayer;
    });
    return '{ "status":"200"}';
}

exports.getPlayersFromRace = function (raceID) {
    // let race = getRace(raceID)
    // race.postMessage({id: 'getPlayers'});
    // race.on("message", result => {
    //     return result.getPlayers;
    // });
    return ["Papo"]
}
