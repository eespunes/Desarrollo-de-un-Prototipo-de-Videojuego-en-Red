class Race {
    id = ""
    levelId = ""
    started = false
    players = []
    time = 10

    constructor(id, levelId, io) {
        let idString = "R"
        if (id < 10)
            idString += "00"
        else if (id < 100)
            idString += "0"

        this.id = idString + id
        this.levelId = levelId
        this.io = io
        this.racing()
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

    racing() {
        this.searchingPlayersTimer = setInterval(this.searchingPlayers.bind(this), 1000)
        this.informPlayers()
        this.stopRace()
    }

    //
    // function
    //
    // stopRace() {
    //

    // a()
    // {
    //     if (smthCompleted)
    //         dosmth();
    //     else return Promise.delay(1000).then(() => a());
    // }
    //
    // function
    //
    informPlayers() {
        // this.socket.at(this.id).emit(/* ... */);
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
    stopRace() {

    }

    get getID() {
        return this.id;
    }

    get getPlayers() {
        return this.players;
    }

    searchingPlayers() {
        if (this.players.length < 2) {
            console.log("Buscando jugadores...")
            this.io.emit(this.id, "Buscando jugadores...")
        } else {
            clearInterval(this.searchingPlayersTimer);
            this.waitingPlayersTimer = setInterval(this.waitingPlayers.bind(this), 1000)
        }
    }

    waitingPlayers() {
        if (this.time <= 0) {
            console.log("Empieza la carrera")
            this.io.emit(this.id, "Empieza la carrera")
            clearInterval(this.waitingPlayersTimer);
        } else {
            console.log("Time: " + this.time)
            this.io.emit(this.id, "Tiempo restante para empezar:"+this.time+"")
            this.time--
        }
    }
}

module
    .exports = Race
