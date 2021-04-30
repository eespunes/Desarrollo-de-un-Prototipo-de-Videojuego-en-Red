// const path = require('path')
// const http = require('http')
// const express = require('express')
// const socketio = require('socket.io')
// // const Filter = require('bad-words')
// // const { generateMessage, generateLocationMessage } = require('./utils/messages')
// // const { addUser, removeUser, getUser, getUsersInRoom } = require('./utils/users')
//
// const app = express()
// const server = http.createServer(app)
// const io = socketio(server)
//
// const port = 3000
// const publicDirectoryPath = path.join(__dirname, '../public')
//
// app.use(express.static(publicDirectoryPath))
//
// io.on('connection', (socket) => {
//     console.log('New WebSocket connection')
//
//     // socket.on('join', (options, callback) => {
//     //     const { error, user } = addUser({ id: socket.id, ...options })
//     //
//     //     if (error) {
//     //         return callback(error)
//     //     }
//     //
//     //     socket.join(user.room)
//     //
//     //     socket.emit('message', generateMessage('Admin', 'Welcome!'))
//     //     socket.broadcast.to(user.room).emit('message', generateMessage('Admin', `${user.username} has joined!`))
//     //     io.to(user.room).emit('roomData', {
//     //         room: user.room,
//     //         users: getUsersInRoom(user.room)
//     //     })
//     //
//     //     callback()
//     // })
//     //
//     // socket.on('sendMessage', (message, callback) => {
//     //     const user = getUser(socket.id)
//     //     const filter = new Filter()
//     //
//     //     if (filter.isProfane(message)) {
//     //         return callback('Profanity is not allowed!')
//     //     }
//     //
//     //     io.to(user.room).emit('message', generateMessage(user.username, message))
//     //     callback()
//     // })
//     //
//     // socket.on('sendLocation', (coords, callback) => {
//     //     const user = getUser(socket.id)
//     //     io.to(user.room).emit('locationMessage', generateLocationMessage(user.username, `https://google.com/maps?q=${coords.latitude},${coords.longitude}`))
//     //     callback()
//     // })
//     //
//     // socket.on('disconnect', () => {
//     //     const user = removeUser(socket.id)
//     //
//     //     if (user) {
//     //         io.to(user.room).emit('message', generateMessage('Admin', `${user.username} has left!`))
//     //         io.to(user.room).emit('roomData', {
//     //             room: user.room,
//     //             users: getUsersInRoom(user.room)
//     //         })
//     //     }
//     // })
// })
//
// server.listen(port, () => {
//     console.log(`Server is up on port ${port}!`)
// })

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
    console.log('A user connected');

    //Whenever someone disconnects this piece of code executed
    client.on('disconnect', function () {
        console.log('A user disconnected');
    });
});


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

server.listen(port, () => {
    playerDB.createServer()
    console.log(`Example app listening at http://localhost:${port}`)
})

