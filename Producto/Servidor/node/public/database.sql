DROP TABLE IF EXISTS Continents CASCADE;
CREATE TABLE Continents
(
    id   VARCHAR(2),
    name VARCHAR(16),
    PRIMARY KEY (id)
);
DROP TABLE IF EXISTS Countries CASCADE;
CREATE TABLE Countries
(
    id          VARCHAR(2),
    name        VARCHAR(256),
    continentID VARCHAR(2) NOT NULL,
    PRIMARY KEY (id),
    FOREIGN KEY (continentID) REFERENCES Continents (id)
);
DROP TABLE IF EXISTS Servers CASCADE;
CREATE TABLE Servers
(
    id        VARCHAR(4),
    name      VARCHAR(64),
    countryID VARCHAR(2) NOT NULL,
    PRIMARY KEY (id),
    FOREIGN KEY (countryID) REFERENCES Countries (id)
);
DROP TABLE IF EXISTS Races CASCADE;
CREATE TABLE Races
(
    id       VARCHAR(4) UNIQUE,
    raceDate DATE,
    serverID VARCHAR(4),
    PRIMARY KEY (id, raceDate, serverID),
    FOREIGN KEY (serverID) REFERENCES Servers (id)
);
DROP TABLE IF EXISTS Players CASCADE;
CREATE TABLE Players
(
    username         VARCHAR(64),
    password         VARCHAR(32)  NOT NULL,
    email            VARCHAR(256) NOT NULL,
    registrationDate DATE,
    level            INTEGER,
    experiencePoints INTEGER,
    PRIMARY KEY (username)
);
DROP TABLE IF EXISTS SteeringWheels CASCADE;
CREATE TABLE SteeringWheels
(
    id             VARCHAR(4),
    playerUsername VARCHAR(64),
    golden         INTEGER,
    silver         INTEGER,
    bronze         INTEGER,
    wooden         INTEGER,
    PRIMARY KEY (id, playerUsername),
    FOREIGN KEY (playerUsername) REFERENCES Players (username)
);
DROP TABLE IF EXISTS Positions CASCADE;
CREATE TABLE Positions
(
    position INTEGER,
    PRIMARY KEY (position)
);
DROP TABLE IF EXISTS Competitors CASCADE;
CREATE TABLE Competitors
(
    playerUsername VARCHAR(64),
    raceID         VARCHAR(4),
    position       INTEGER,
    PRIMARY KEY (playerUsername,raceID),
    FOREIGN KEY (playerUsername) REFERENCES Players (username),
    FOREIGN KEY (raceID) REFERENCES Races (id),
    FOREIGN KEY (position) REFERENCES Positions (position)
);
