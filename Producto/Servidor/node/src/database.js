const { Pool, Client } = require('pg')
const connectionString = 'postgresql://postgres:7508@localhost:5433/postgres'
const pool = new Pool({
    connectionString,
})


pool.query('SELECT * FROM Players', (err, res) => {
    console.log(err, res)
    pool.end()
})
