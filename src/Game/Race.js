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
            if (player === username) {
                return
            }
            this.players.unshift(player)
        }
    }

    racing() {
        if (!this.sessionStarted) return;
        if (!this.raceStarted) {
            if (this.allPlayersAreReady()) {
                this.raceStarted = true;
                this.io.emit(this.id + "-start", "Lights out")
            }
        } else {
            // if (this.players.length === 0)
            //     clearInterval(this.racingTimer)
            // this.io.emit(this.id, this.getMessages())
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
        if (this.raceStarted) {
            // console.log(username + ": " + JSON.stringify(message))
            this.io.emit(this.id, message);
        } else
            this.playersMessages.get(username).push(message)
    }

    //
    // function
    //
    stopRace() {

    }

    get getID() {
        return this.id
    }

    get getPlayers() {
        return this.players
    }

    searchingPlayers() {
        // this.startTime=this.maxStartTime
        if (this.players.length < this.minPLayersToStart) {
            this.io.emit(this.id + "-timer", "Buscando " + (this.minPLayersToStart - this.players.length) + " jugadores...")
        } else {
            clearInterval(this.searchingPlayersTimer);
            this.waitingPlayersTimer = setInterval(this.waitingPlayers.bind(this), 1000)
        }
    }

    waitingPlayers() {
        if (this.players.length < this.minPLayersToStart) {
            this.searchingPlayersTimer = setInterval(this.searchingPlayers.bind(this), 100)
            clearInterval(this.waitingPlayersTimer)
            return
        }

        if (this.time <= 0) {
            this.startRace()
            this.io.emit(this.id + "-timer", "Empieza la carrera")
            clearInterval(this.waitingPlayersTimer)
        } else {
            this.io.emit(this.id + "-timer", "Tiempo restante para empezar:" + this.time + "")
            this.time--
        }
    }

    allPlayersAreReady() {
        for (let i = 0; i < this.players.length; i++) {
            let player = this.players.pop()
            this.players.unshift(player)
            if (this.playersMessages.get(player).length === 0)
                return false
        }
        return true
    }

    getMessages() {
        let messages = []
        for (let i = 0; i < this.players.length; i++) {
            let player = this.players[i]
            if (player === undefined)
                continue
            //check ids
            let message = this.playersMessages.get(player).shift()

            if (message === undefined) {
                message = "{username: \"" + player + "\", id:\"NOT RECIEVED\"}"
            }
            console.log(i + " - " + player + ": " + message)

            messages.push(message)
        }

        let biggerID = 0
        for (let i = 0; i < messages.length; i++) {
            let message = messages[i]
            const {id, username} = message
            // console.log(i + " - " + id + " - " + username)
            if (id > biggerID)
                biggerID = id
        }

        let aux;
        for (let i = 1; i < messages.length; i++) {
            for (let j = messages.length - 1; j >= i; j--) {
                if (this.InFrontOfOpponent(messages[j], messages[j - 1])) {
                    aux = messages[j];
                    messages[j] = messages[j - 1];
                    messages[j - 1] = aux;
                }
            }
        }

        return messages;
    }

    InFrontOfOpponent(message, message2) {
        const json1 = message;
        const json2 = message2;
        if (message === undefined || message2 === undefined)
            return false
        return json1.lap > json2.lap ||
            json1.lap == json2.lap && (
                json1.checkpoint > json2.checkpoint ||
                json1.checkpoint == json2.checkpoint && json1.distance < json2.distance
            )
    }
}

module
    .exports = Race
