const path = require('path')
const http = require('http')
const express = require('express')
const socketio = require('socket.io')

const playerDB = require('./Database/DatabaseController')
const gameManager = require('./Game/GameManager')
const bodyParser = require("body-parser")
const cors = require("cors")

const app = express()
const server = http.createServer(app)
const io = socketio(server)

const port = process.env.PORT || 3000
const publicDirectoryPath = path.join(__dirname, '../public')

app.use(express.static(publicDirectoryPath))
app.use(cors());
app.use(bodyParser.json());


io.on('connection', client => {
    client.on("FindRace", (json) => {
        const {username, levelId} = JSON.parse(json);
        const returnJSON = JSON.parse(gameManager.addPlayerToRace(username, levelId, io))
        io.to(client.id).emit("FindRace", returnJSON);

        const {race} = returnJSON
        client.join(race, function () {
            setTimeout(() => {
                io.emit(race + "-players", gameManager.getPlayersFromRace(race))
            }, 500);

        });
    });

    client.on("Racing", (json) => {
        const {race, username, data} = JSON.parse(json);
        gameManager.addMessageToThePlayer(race, username, JSON.parse(data))
    });

    client.on("Object", (json) => {
        const {race} = JSON.parse(json);
        gameManager.addObjectToRace(race, JSON.parse(json))
    });

    client.on("Disconnect", (json) => {
        const {username, levelId} = JSON.parse(json);
        client.leave(levelId, function () {
            gameManager.removePlayerFromRace(username, levelId)
            io.emit(levelId + "-players", gameManager.getPlayersFromRace(levelId))
        });
    });
});


app.get('/', (req, res) => {
    res.send('Bienvenido al servidor')
})

app.post('/api/login/', (req, res) => {
    playerDB.login(req, res)
})
app.post('/api/logout/', (req, res) => {
    playerDB.logout(req, res)
})
app.post('/api/player/', (req, res) => {
    playerDB.register(req, res)
})
app.get('/api/player/:username', (req, res) => {
    playerDB.getPlayer(req, res)
})

app.post('/api/golden/', (req, res) => {
    playerDB.addGolden(req, res)
})
app.post('/api/silver/', (req, res) => {
    playerDB.addSilver(req, res)
})
app.post('/api/bronze/', (req, res) => {
    playerDB.addBronze(req, res)
})
app.post('/api/wood/', (req, res) => {
    playerDB.addWooden(req, res)
})

app.get('/api/race/all/:username', (req, res) => {
    playerDB.getAllRacesByPlayer(req, res)
})

app.get('/api/car/:id', (req, res) => {
    playerDB.getCar(req, res)
})
app.get('/api/car/all', (req, res) => {
    playerDB.getAllCars(req, res)
})
app.get('/api/ai/:id', (req, res) => {
    playerDB.getAI(req, res)
})
app.get('/api/ai/all', (req, res) => {
    playerDB.getAllAIs(req, res)
})
app.get('/api/object/:id', (req, res) => {
    playerDB.getObject(req, res)
})
app.get('/api/object/all', (req, res) => {
    playerDB.getAllObjects(req, res)
})
app.get('/api/level/:id', (req, res) => {
    playerDB.getLevel(req, res)
})
app.get('/api/level/all', (req, res) => {
    playerDB.getAllLevels(req, res)
})

server.listen(port, () => {
    playerDB.createServer()
    console.log(`Example app listening at http://localhost:${port}`)
})

module.exports = io

