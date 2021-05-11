class Race {
    id = ""
    levelId = ""
    sessionStarted = false
    players = []
    time = 5
    raceStarted = false

    playersMessages = new Map()
    minPLayersToStart = 2;

    constructor(id, levelId, io) {
        let idString = "R"
        if (id < 10)
            idString += "00"
        else if (id < 100)
            idString += "0"

        this.id = idString + id
        this.levelId = levelId
        this.io = io;

        this.searchingPlayersTimer = setInterval(this.searchingPlayers.bind(this), 100)
    }


    addPlayer(username) {
        this.players.push(username)
    }

    get hasCapacity() {
        if (this.sessionStarted)
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
    racing() {
        if (!this.sessionStarted) return;
        if (!this.raceStarted) {
            if (this.allPlayersAreReady()) {
                this.raceStarted = true;
                this.io.emit(this.id + "-start", "Lights out")
                console.log(this.id + ": It's lights out and away we go!!")
            }
        } else {
        }
    }

    //
    // function
    //
    startRace() {
        this.sessionStarted = true
        this.racingTimer = setInterval(this.racing.bind(this), 250)

        for (let i = 0; i < this.players.length; i++) {
            let player = this.players.pop()
            this.players.unshift(player)
            this.playersMessages.set(player, [])
        }
    }

    addMessageToThePlayer(username, message) {
        this.playersMessages.get(username).push(message);
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
        if (this.players.length < this.minPLayersToStart) {
            this.io.emit(this.id + "-timer", "Buscando " + (this.minPLayersToStart - this.players.length) + " jugadores...")
        } else {
            clearInterval(this.searchingPlayersTimer);
            this.waitingPlayersTimer = setInterval(this.waitingPlayers.bind(this), 1000)
        }
    }

    waitingPlayers() {
        if (this.time <= 0) {
            this.startRace()
            this.io.emit(this.id + "-timer", "Empieza la carrera")
            clearInterval(this.waitingPlayersTimer);
        } else {
            this.io.emit(this.id + "-timer", "Tiempo restante para empezar:" + this.time + "")
            this.time--
        }
    }

    allPlayersAreReady() {
        for (let i = 0; i < this.players.length; i++) {
            let player = this.players.pop()
            this.players.unshift(player)
            // console.log(player + " -" + this.playersMessages.get(player).length)
            if (this.playersMessages.get(player).length === 0)
                return false
        }
        return true
    }
}

module
    .exports = Race
