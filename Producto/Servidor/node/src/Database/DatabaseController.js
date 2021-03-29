const {Pool} = require('pg')
const config = require('./DatabaseConfigurator')
const encrypt = require("../Security/Encryption");

const pool = new Pool({
    user: config.user,
    host: config.host,
    database: config.database,
    password: config.password,
    port: config.port,
    ssl: {
        rejectUnauthorized: false
    }
})

let playersLoggedIn = []

exports.login = function (req, res) {
    const {username, password} = req.body;
    try {
        pool.query('SELECT pla_password FROM Players WHERE pla_username=$1', [username], (error, results) => {
            if (error) {
                res.sendStatus(404)
            } else {
                if (results.rows[0]['pla_password'] === password) {
                    playersLoggedIn.push(username)
                    console.log(playersLoggedIn)
                    res.sendStatus(202)
                } else
                    res.sendStatus(403)
            }
        })
    } catch (e) {
        res.status(500).send('ERROR: Failed to login')
    }
};

exports.logout = function (req, res) {
    const {username} = req.body;

    console.log(playersLoggedIn)
    if (playerIsLoggedIn(username)) {
        playersLoggedIn = playersLoggedIn.filter(function (id) {
            return id != username
        })
        console.log(playersLoggedIn)
        res.status(200).send()
    } else
        res.status(400).send('ERROR: The User was not Logged In')
};
exports.register = function (req, res) {
    const {username, password, email} = req.body;

    let dataToInsert = [username, password, email, new Date().toISOString(), 1, 0, 0, 0, 0, 50];

    pool.query('INSERT INTO Players VALUES($1,$2,$3,$4,$5,$6,$7,$8,$9,$10)', dataToInsert, (error, results) => {
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


exports.registerRace = function (req, res) {
    let {levelId} = req.body;

        pool.query('INSERT INTO Races VALUES($1,$2,$3)', [new Date().toISOString(),serverID, levelID], (error, results) => {
            if (error) {
                res.sendStatus(400)
            } else {
                res.sendStatus(201)
            }
        })
};
exports.addPlayerToRace = function (req, res) {
    let {username, raceID} = req.body;

    try {
        pool.query('INSERT INTO Competitors VALUES($1,$2,$3)', [username, raceID, -1], (error, results) => {
            if (error) {
                res.sendStatus(400)
            } else {
                res.sendStatus(201)
            }
        })
    } catch (e) {
        res.status(500).send('ERROR: Failed to create Competitor')
    }
};
exports.setPosition = function (req, res) {
    let {username, raceID, position} = req.body;

    pool.query('UPDATE Competitors SET position = $1 WHERE playerusername = $2 and raceid=$3', [position, username, raceID], (error, results) => {
        if (error) {
            res.sendStatus(404).send('ERROR: Failed to update Competitor')
        } else {
            res.sendStatus(200)
        }
    })
};
exports.getAllRacesByPlayer = function (req, res) {
    let {username} = req.params

    pool.query('SELECT * FROM Competitors WHERE playerusername=$1', username, (err, results) => {
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
            value += results.rows[0]['pla_goldenSteeringWheel']
        }
    })

    pool.query('UPDATE Players SET pla_goldenSteeringWheel = $1 WHERE pla_username = $2', [value, username], (err, results) => {
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
            value += results.rows[0]['pla_silverSteeringWheel']
        }
    })

    pool.query('UPDATE Players SET pla_silverSteeringWheel = $1 WHERE pla_username = $2', [value, username], (err, results) => {
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
            value += results.rows[0]['pla_bronzeSteeringWheel']
        }
    })

    pool.query('UPDATE Players SET pla_bronzeSteeringWheel = $1 WHERE pla_username = $2', [value, username], (err, results) => {
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
            value += results.rows[0]['pla_woodenSteeringWheel']
        }
    })

    pool.query('UPDATE Players SET pla_woodenSteeringWheel = $1 WHERE pla_username = $2', [value, username], (err, results) => {
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
    let equals = false;
    playersLoggedIn.forEach(function (entry) {
        if (entry === username) {
            equals = true;
        }
    });
    return equals;
}

