const {Pool} = require('pg')
const databaseConfig = require('../Configuration/DatabaseConfigurator')
const serverConfig = require('../Configuration/ServerConfiguration')

const pool = new Pool({
    user: databaseConfig.user,
    host: databaseConfig.host,
    database: databaseConfig.database,
    password: databaseConfig.password,
    port: databaseConfig.port,
    ssl: {
        rejectUnauthorized: false
    }
})

let playersLoggedIn = []

exports.createServer = function () {
    pool.query('SELECT ser_id FROM Servers WHERE ser_id=$1', [serverConfig.serverID], (error, results) => {
        try {
            results.rows[0]['ser_id']
        } catch (e) {
            pool.query('INSERT INTO Servers VALUES($1,$2,$3)', [serverConfig.serverID, serverConfig.serverName, serverConfig.serverLocation], (error) => {
                if (!error)
                    console.log("Server Created in the Database")
            })
        }
    })
};

exports.login = function (req, res) {
    const {username, password} = req.body;
    pool.query('SELECT pla_password FROM Players WHERE pla_username=$1', [username], (error, results) => {
        if (error) {
            res.sendStatus(404)
        } else {
            try {
                if (results.rows[0]['pla_password'] === password) {
                    playersLoggedIn.push(username)
                    console.log(playersLoggedIn)
                    res.sendStatus(202)
                } else
                    res.sendStatus(403)
            } catch (e) {
                res.sendStatus(503)
            }
        }
    })
};

exports.logout = function (req, res) {
    const {username} = req.body;

    console.log(playersLoggedIn)
    if (playerIsLoggedIn(username)) {
        playersLoggedIn = playersLoggedIn.filter(function (id) {
            return id !== username
        })
        console.log(playersLoggedIn)
        res.status(200).send()
    } else
        res.status(400).send('ERROR: The User was not Logged In')
};
exports.register = function (req, res) {
    const {username, password, email} = req.body;

    let dataToInsert = [username, password, email, new Date().toISOString(), 1, 0, 0, 0, 0, 50];

    pool.query('INSERT INTO Players VALUES($1,$2,$3,$4,$5,$6,$7,$8,$9,$10)', dataToInsert, (error) => {
        if (error) {
            res.sendStatus(401);
            return
        }
        res.sendStatus(201);
    })
};
exports.getPlayer = function (req, res) {
    const {username} = req.params;

    pool.query('SELECT * FROM Players WHERE pla_username=$1', [username], (error, results) => {
        if (error) {
            res.status(404).send('Player with' + username + ', not found');
            return
        }
        res.status(200).json(results.rows);
    })
};


exports.registerRace = async function (levelID) {
    await pool.query('INSERT INTO Races (rac_raceDate, rac_serverID, rac_levelID) VALUES($1,$2,$3)', [new Date(Date.now()).toISOString().replace('T', ' ').replace('Z', ''), serverConfig.serverID, levelID], (error) => {
        return !error;
    })
};
exports.addPlayerToRace = async function (username, raceID) {
    await pool.query('INSERT INTO Competitors VALUES($1,$2,$3)', [username, raceID, -1], (error) => {
        if (error)
            console.log(raceID)
        return !error;
    })
};
exports.setPosition = function (req, res) {
    let {username, raceID, position} = req.body;

    pool.query('UPDATE Competitors SET com_position = $1 WHERE com_playerusername = $2 and com_raceid=$3', [position, username, raceID], (error) => {
        if (error) {
            res.sendStatus(404).send('ERROR: Failed to update Competitor')
        } else {
            res.sendStatus(200)
        }
    })
};
exports.getAllRacesByPlayer = function (req, res) {
    let {username} = req.params

    pool.query('SELECT * FROM Competitors WHERE com_playerusername=$1', username, (err, results) => {
        if (err) {
            res.sendStatus(404)
        } else {
            res.status(200).send(results.rows)
        }
    })
}


exports.addGolden = function (req, res) {
    let {username, value} = req.body;

    pool.query('SELECT pla_goldenSteeringWheel FROM Players WHERE pla_username = $1', [username], (error, results) => {
        if (error) {
            res.sendStatus(404)
        } else {
            try {
                value += results.rows[0]['pla_goldenSteeringWheel']
            } catch (e) {
                res.sendStatus(503)
            }
        }
    })

    pool.query('UPDATE Players SET pla_goldenSteeringWheel = $1 WHERE pla_username = $2', [value, username], (err) => {
        if (err) {
            res.sendStatus(406)
        } else {
            res.status(202).json(value)
        }
    })
};
exports.addSilver = function (req, res) {
    let {username, value} = req.body;

    pool.query('SELECT pla_silverSteeringWheel FROM Players WHERE pla_username = $1', [username], (error, results) => {
        if (error) {
            res.sendStatus(404)
        } else {
            try {
                value += results.rows[0]['pla_silverSteeringWheel']
            } catch (e) {
                res.sendStatus(503)
            }
        }
    })

    pool.query('UPDATE Players SET pla_silverSteeringWheel = $1 WHERE pla_username = $2', [value, username], (err) => {
        if (err) {
            res.sendStatus(406)
        } else {
            res.status(202).json(value)
        }
    })
};
exports.addBronze = function (req, res) {
    let {username, value} = req.body;

    pool.query('SELECT pla_bronzeSteeringWheel FROM Players WHERE pla_username = $1', [username], (error, results) => {
        if (error) {
            res.sendStatus(404)
        } else {
            try {
                value += results.rows[0]['pla_bronzeSteeringWheel']
            } catch (e) {
                res.sendStatus(503)
            }
        }
    })

    pool.query('UPDATE Players SET pla_bronzeSteeringWheel = $1 WHERE pla_username = $2', [value, username], (err) => {
        if (err) {
            res.sendStatus(406)
        } else {
            res.status(202).json(value)
        }
    })
};
exports.addWooden = function (req, res) {
    let {username, value} = req.body;

    pool.query('SELECT pla_woodenSteeringWheel FROM Players WHERE pla_username = $1', [username], (error, results) => {
        if (error) {
            res.sendStatus(404)
        } else {
            try {
                value += results.rows[0]['pla_woodenSteeringWheel']
            } catch (e) {
                res.sendStatus(503)
            }
        }
    })

    pool.query('UPDATE Players SET pla_woodenSteeringWheel = $1 WHERE pla_username = $2', [value, username], (err) => {
        if (err) {
            res.sendStatus(406)
        } else {
            res.status(202).json(value)
        }
    })
};


exports.getCar = function (req, res) {
    const {id} = req.params;

    pool.query('SELECT * FROM Cars WHERE car_id=$1', [id], (error, results) => {
        if (error) {
            res.sendStatus(404);
            return
        }
        res.status(200).json(results.rows);
    })
}
exports.getAllCars = function (req, res) {
    pool.query('SELECT * FROM Cars', (error, results) => {
        if (error) {
            res.sendStatus(404);
            return
        }
        res.status(200).json(results.rows);
    })
}

exports.getAI = function (req, res) {
    const {id} = req.params;

    pool.query('SELECT * FROM AIs WHERE ai_id=$1', [id], (error, results) => {
        if (error) {
            res.sendStatus(404);
            return
        }
        res.status(200).json(results.rows);
    })
}
exports.getAllAIs = function (req, res) {
    pool.query('SELECT * FROM AIs', (error, results) => {
        if (error) {
            res.sendStatus(404);
            return
        }
        res.status(200).json(results.rows);
    })
}

exports.getObject = function (req, res) {
    const {id} = req.params;

    pool.query('SELECT * FROM Objects WHERE obj_id=$1', [id], (error, results) => {
        if (error) {
            res.sendStatus(404);
            return
        }
        res.status(200).json(results.rows);
    })
}
exports.getAllObjects = function (req, res) {
    pool.query('SELECT * FROM Objects', (error, results) => {
        if (error) {
            res.sendStatus(404);
            return
        }
        res.status(200).json(results.rows);
    })
}

exports.getLevel = function (req, res) {
    const {id} = req.params;

    pool.query('SELECT * FROM Levels WHERE lvl_id=$1', [id], (error, results) => {
        if (error) {
            res.sendStatus(404);
            return
        }
        res.status(200).json(results.rows);
    })
}
exports.getAllLevels = function (req, res) {
    pool.query('SELECT * FROM Levels', (error, results) => {
        if (error) {
            res.sendStatus(404);
            return
        }
        res.status(200).json(results.rows);
    })
}

function playerIsLoggedIn(username) {
    let usernameIsPlaying = false;
    playersLoggedIn.forEach(function (entry) {
        if (entry === username) {
            usernameIsPlaying = true;
        }
    });
    return usernameIsPlaying;
}

