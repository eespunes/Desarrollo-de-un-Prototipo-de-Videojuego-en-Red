class Race {
    id = ""
    levelId = ""
    started = false
    players = []

    constructor(id) {
        let idString = "L"
        if (id < 10)
            idString += "00"
        else if (id < 100)
            idString += "0"

        this.id = idString + id
    }

    addPlayer(username) {
        this.players.push(username)
        //TODO SOCKET.IO CREATE SOCKET AND ROOM
    }

    hasCapacity() {
        if (this.started)
            return false;
        if (this.players.length === 12)
            return false
        return true
    }

    removePlayer(username) {
        this.players = this.players.filter(function (playerID) {
            return playerID != username
        })
        //TODO SOCKET.IO REMOVE SOCKET AND ROOM
    }

    //
    // function
    //
    // informPlayers() {
    //
    // }
    //
    // function
    //
    // startRace() {
    //     started = true
    // }
    //
    // function
    //
    // stopRace() {
    //
    // }
    getID() {
        return this.id;
    }
}
