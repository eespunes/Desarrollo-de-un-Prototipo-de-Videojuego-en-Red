class Race {
    id = ""
    levelId = ""
    started = false
    players = []

    constructor(id, levelId) {
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

    hasPlayer(username) {
        for (let i = 0; i < this.players.length; i++) {
            let player = this.players.pop()
            this.players.unshift(player)
            if (player === username)
                return true
        }
        return false
    }

    removePlayer(username) {
        for (let i = 0; i < this.players.length; i++) {
            let player = this.players.pop()
            if (player === username)
                return
            this.players.unshift(player)
        }
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

    get getPlayers() {
        return this.players;
    }
}

module.exports = Race
