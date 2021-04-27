class Race {
    id = ""
    levelId = ""
    started = false
    players = []
    socket = io();

    constructor(id, levelId,io) {
        let idString = "R"
        if (id < 10)
            idString += "00"
        else if (id < 100)
            idString += "0"

        this.id = idString + id
        this.levelId = levelId
    }

    addPlayer(username) {
        this.players.push(username)
    }

    get hasCapacity() {
        if (this.started)
            return false;
        if (this.players.length === 12)
            return false
        return true
    }

    removePlayer(username) {
        this.players = this.players.filter(function (playerID) {
            return playerID !== username
        })
        //TODO SOCKET.IO REMOVE SOCKET AND ROOM
    }

    //
    // function
    //
    informPlayers() {
        this.socket.at(this.id).emit(/* ... */);
    }
    //
    // function
    //
    startRace() {
        this.started = true
    }
    //
    // function
    //
    // stopRace() {
    //
    // }
    get getID() {
        return this.id;
    }
}

module.exports = Race
