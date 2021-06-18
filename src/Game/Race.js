class Race {
    id = ""
    levelId = ""
    sessionStarted = false
    players = []
    time = 30
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

    get hasCapacity() {
        if (this.sessionStarted)
            return false;
        if (this.players.length === 12)
            return false
        return true
    }

    get getID() {
        return this.id
    }

    get getPlayers() {
        return this.players
    }

    addPlayer(username) {
        this.players.push(username)
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
                if (this.playersMessages.get(player) !== undefined && this.playersMessages.get(player).length > 0) {
                    if (this.playersMessages.get(player)[0].finished) {
                        this.players.unshift(player)
                        return
                    }
                }
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
            clearInterval(this.racingTimer)
        }
    }

    startRace() {
        this.sessionStarted = true
        this.racingTimer = setInterval(this.racing.bind(this), 250)

        for (let i = 0; i < this.players.length; i++) {
            let player = this.players[i]
            if (player === undefined)
                continue
            if (this.playersMessages.get(player) === undefined) {
                this.playersMessages.set(player, [])
                continue;
            }
            if (this.playersMessages.get(player).length === 0)
                continue
            let message = this.playersMessages.get(player).shift()
            this.playersMessages.get(player).unshift(message)
            const {finished} = message
            if (!finished) {
                this.playersMessages.set(player, [])
            }
        }
    }

    addMessageToThePlayer(username, message) {
        if (this.raceStarted) {
            this.io.emit(this.id, message);
            this.changePlayerMessage(username, message);
            this.io.emit(this.id + '-classification', this.getClassification());
        } else
            this.playersMessages.get(username).push(message)
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
            if (this.playersMessages.get(player).length === 0) {
                return false
            }
        }
        return true
    }

    getClassification() {
        let aux;
        for (let i = 0; i < this.players.length; i++) {
            let player = this.players[i]
            if (player === undefined)
                continue

            let message = this.playersMessages.get(player).shift()
            this.playersMessages.get(player).push(message)

            for (let j = this.players.length - 1; j >= i; j--) {
                let otherPlayer = this.players[j]
                if (otherPlayer === undefined || player == otherPlayer)
                    continue
                let otherMessage = this.playersMessages.get(otherPlayer).shift()
                this.playersMessages.get(otherPlayer).push(otherMessage)

                if (!this.InFrontOfOpponent(message, otherMessage, player, otherPlayer)) {
                    aux = this.players[j];
                    this.players[j] = this.players[i];
                    this.players[i] = aux;
                }
            }
        }

        return this.players;
    }

    InFrontOfOpponent(message, message2, player, otherPlayer) {
        const json1 = message;
        const json2 = message2;

        if (message === undefined || message2 === undefined)
            return false

        console.log(this.id + " - " + player + ":" + json1.lap + "/" + json1.checkpoint + "/" + json1.distance + " - " + otherPlayer + ":" + json2.lap + "/" + json2.checkpoint + "/" + json2.distance)
        if (json1.finished && json2.finished) {
            return json1.time < json2.time
        }
        if (json1.finished) {
            return true
        }
        if (json2.finished) {
            return false
        }
        return json1.lap > json2.lap ||
            json1.lap == json2.lap && (
                json1.checkpoint > json2.checkpoint ||
                json1.checkpoint == json2.checkpoint && json1.distance < json2.distance
            )
    }

    changePlayerMessage(username, message) {
        const currentMessage = this.playersMessages.get(username).pop();
        console.log("CHANGED " + username+": "+message.id+"=="+currentMessage.id)
        if (message.id >= currentMessage.id) {
            this.playersMessages.get(username).push(message);
        } else {
            this.playersMessages.get(username).push(currentMessage);
        }
    }

    addGameObject(json) {
        if (json.ragnarok) {
            this.raceStarted = false;
            this.startRace();
        }
        this.io.emit(this.id + "-object", json)
    }
}

module
    .exports = Race
