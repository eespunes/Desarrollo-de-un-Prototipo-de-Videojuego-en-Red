const express = require('express'),
    app = express(),
    port = process.env.PORT || 3000,
    playerDB = require('./Database/DatabaseController'),
    gameManager = require('./Game/GameManager'),
    bodyParser = require("body-parser"),
    cors = require("cors");

app.use(cors());
app.use(bodyParser.json());

app.get('/', (req, res) => {
    res.send('Welcome to our Game Server')
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


app.post('/api/race/add/', (req, res) => {
    const {username, levelID} = req.body;
    gameManager.addPlayerToRace(username, levelID, res)
})
app.get('/api/race/all/:username', (req, res) => {
    playerDB.getAllRacesByPlayer(req, res)
})
app.get('/api/race/remove/:username', (req, res) => {
    const {username} = req.params;
    gameManager.removePlayerFromRace(username, res)
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

app.listen(port, () => {
    playerDB.createServer()
    console.log(`Example app listening at http://localhost:${port}`)
})

