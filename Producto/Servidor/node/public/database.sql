DROP TABLE IF EXISTS Continents CASCADE;
CREATE TABLE Continents
(
    con_id   VARCHAR(2),
    con_name VARCHAR(16),
    PRIMARY KEY (con_id)
);
DROP TABLE IF EXISTS Countries CASCADE;
CREATE TABLE Countries
(
    cou_id          VARCHAR(3),
    cou_name        VARCHAR(256),
    cou_continentID VARCHAR(2) NOT NULL,
    PRIMARY KEY (cou_id),
    FOREIGN KEY (cou_continentID) REFERENCES Continents (con_id)
);
DROP TABLE IF EXISTS Servers CASCADE;
CREATE TABLE Servers
(
    ser_id        SERIAL,
    ser_name      VARCHAR(64),
    ser_countryID VARCHAR(3) NOT NULL,
    PRIMARY KEY (ser_id),
    FOREIGN KEY (ser_countryID) REFERENCES Countries (cou_id)
);
DROP TABLE IF EXISTS Levels CASCADE;
CREATE TABLE Levels
(
    lvl_id   SERIAL,
    lvl_name VARCHAR(64),
    PRIMARY KEY (lvl_id)
);
DROP TABLE IF EXISTS Races CASCADE;
CREATE TABLE Races
(
    rac_id       SERIAL UNIQUE ,
    rac_raceDate DATE,
    rac_serverID INTEGER,
    rac_levelID  INTEGER,
    PRIMARY KEY (rac_id, rac_raceDate, rac_serverID),
    FOREIGN KEY (rac_serverID) REFERENCES Servers (ser_id),
    FOREIGN KEY (rac_levelID) REFERENCES Levels (lvl_id)
);
DROP TABLE IF EXISTS Players CASCADE;
CREATE TABLE Players
(
    pla_username            VARCHAR(64),
    pla_password            VARCHAR(32)  NOT NULL,
    pla_email               VARCHAR(256) NOT NULL,
    pla_registrationDate    DATE,
    pla_level               INTEGER,
    pla_experiencePoints    INTEGER,
    pla_goldenSteeringWheel INTEGER,
    pla_silverSteeringWheel INTEGER,
    pla_bronzeSteeringWheel INTEGER,
    pla_woodenSteeringWheel INTEGER,
    pla_aiID                INTEGER,
    PRIMARY KEY (pla_username),
    FOREIGN KEY (pla_aiID) REFERENCES AIs (ai_id)
);
DROP TABLE IF EXISTS AIs CASCADE;
CREATE TABLE AIs
(
    ai_id SERIAL,
    PRIMARY KEY (ai_id)
);
DROP TABLE IF EXISTS Objects CASCADE;
CREATE TABLE Objects
(
    obj_id SERIAL,
    PRIMARY KEY (obj_id)
);
DROP TABLE IF EXISTS Cars CASCADE;
CREATE TABLE Cars
(
    car_id SERIAL,
    PRIMARY KEY (car_id)
);

DROP TABLE IF EXISTS CarSetups CASCADE;
CREATE TABLE CarSetups
(
    cse_playerUsername VARCHAR(64),
    cse_raceID         INTEGER,
    cse_carID          INTEGER,
    PRIMARY KEY (cse_playerUsername, cse_raceID),
    FOREIGN KEY (cse_playerUsername) REFERENCES Players (pla_username),
    FOREIGN KEY (cse_raceID) REFERENCES Races (rac_id),
    FOREIGN KEY (cse_carID) REFERENCES Cars (car_id)
);

DROP TABLE IF EXISTS ObjectsThrew CASCADE;
CREATE TABLE ObjectsThrew
(
    oth_id             SERIAL,
    oth_playerUsername VARCHAR(64),
    oth_raceID         INTEGER,
    oth_objectID       INTEGER,
    PRIMARY KEY (oth_id, oth_playerUsername, oth_raceID),
    FOREIGN KEY (oth_playerUsername) REFERENCES Players (pla_username),
    FOREIGN KEY (oth_raceID) REFERENCES Races (rac_id),
    FOREIGN KEY (oth_objectID) REFERENCES Objects (obj_id)
);

DROP TABLE IF EXISTS Competitors CASCADE;
CREATE TABLE Competitors
(
    playerUsername VARCHAR(64),
    raceID         INTEGER,
    position       INTEGER,
    PRIMARY KEY (playerUsername, raceID),
    FOREIGN KEY (playerUsername) REFERENCES Players (pla_username),
    FOREIGN KEY (raceID) REFERENCES Races (rac_id)
);


INSERT INTO Continents
VALUES ('AF', 'Africa');
INSERT INTO Continents
VALUES ('AN', 'Antarctica');
INSERT INTO Continents
VALUES ('AS', 'Asia');
INSERT INTO Continents
VALUES ('EU', 'Europe');
INSERT INTO Continents
VALUES ('NA', 'North America');
INSERT INTO Continents
VALUES ('OC', 'Oceania');
INSERT INTO Continents
VALUES ('SA', 'South America');

INSERT INTO Countries
VALUES ('AND', 'Principality of Andorra', 'EU');
INSERT INTO Countries
VALUES ('ARE', 'United Arab Emirates', 'AS');
INSERT INTO Countries
VALUES ('AFG', 'Islamic Republic of Afghanistan', 'AS');
INSERT INTO Countries
VALUES ('ATG', 'Antigua and Barbuda', 'NA');
INSERT INTO Countries
VALUES ('AIA', 'Anguilla', 'NA');
INSERT INTO Countries
VALUES ('ALB', 'Republic of Albania', 'EU');
INSERT INTO Countries
VALUES ('ARM', 'Republic of Armenia', 'AS');
INSERT INTO Countries
VALUES ('ANT', 'Netherlands Antilles', 'NA');
INSERT INTO Countries
VALUES ('AGO', 'Republic of Angola', 'AF');
INSERT INTO Countries
VALUES ('ATA', 'Antarctica', 'AN');
INSERT INTO Countries
VALUES ('ARG', 'Argentine Republic', 'SA');
INSERT INTO Countries
VALUES ('ASM', 'American Samoa', 'OC');
INSERT INTO Countries
VALUES ('AUT', 'Republic of Austria', 'EU');
INSERT INTO Countries
VALUES ('AUS', 'Commonwealth of Australia', 'OC');
INSERT INTO Countries
VALUES ('ABW', 'Aruba', 'NA');
INSERT INTO Countries
VALUES ('ALA', 'Åland Islands', 'EU');
INSERT INTO Countries
VALUES ('AZE', 'Republic of Azerbaijan', 'AS');
INSERT INTO Countries
VALUES ('BIH', 'Bosnia and Herzegovina', 'EU');
INSERT INTO Countries
VALUES ('BRB', 'Barbados', 'NA');
INSERT INTO Countries
VALUES ('BGD', 'Republic of Bangladesh', 'AS');
INSERT INTO Countries
VALUES ('BEL', 'Kingdom of Belgium', 'EU');
INSERT INTO Countries
VALUES ('BFA', 'Burkina Faso', 'AF');
INSERT INTO Countries
VALUES ('BGR', 'Republic of Bulgaria', 'EU');
INSERT INTO Countries
VALUES ('BHR', 'Kingdom of Bahrain', 'AS');
INSERT INTO Countries
VALUES ('BDI', 'Republic of Burundi', 'AF');
INSERT INTO Countries
VALUES ('BEN', 'Republic of Benin', 'AF');
INSERT INTO Countries
VALUES ('BLM', 'Saint Barthelemy', 'NA');
INSERT INTO Countries
VALUES ('BMU', 'Bermuda', 'NA');
INSERT INTO Countries
VALUES ('BRN', 'Brunei Darussalam', 'AS');
INSERT INTO Countries
VALUES ('BOL', 'Republic of Bolivia', 'SA');
INSERT INTO Countries
VALUES ('BRA', 'Federative Republic of Brazil', 'SA');
INSERT INTO Countries
VALUES ('BHS', 'Commonwealth of the Bahamas', 'NA');
INSERT INTO Countries
VALUES ('BTN', 'Kingdom of Bhutan', 'AS');
INSERT INTO Countries
VALUES ('BVT', 'Bouvet Island', 'AN');
INSERT INTO Countries
VALUES ('BWA', 'Republic of Botswana', 'AF');
INSERT INTO Countries
VALUES ('BLR', 'Republic of Belarus', 'EU');
INSERT INTO Countries
VALUES ('BLZ', 'Belize', 'NA');
INSERT INTO Countries
VALUES ('CAN', 'Canada', 'NA');
INSERT INTO Countries
VALUES ('CCK', 'Cocos(Keeling) Islands', 'AS');
INSERT INTO Countries
VALUES ('COD', 'Democratic Republic of the Congo', 'AF');
INSERT INTO Countries
VALUES ('CAF', 'Central African Republic', 'AF');
INSERT INTO Countries
VALUES ('COG', 'Republic of the Congo', 'AF');
INSERT INTO Countries
VALUES ('CHE', 'Swiss Confederation', 'EU');
INSERT INTO Countries
VALUES ('CIV', 'Republic of Cote dIvoire', 'AF');
INSERT INTO Countries
VALUES ('COK', 'Cook Islands', 'OC');
INSERT INTO Countries
VALUES ('CHL', 'Republic of Chile', 'SA');
INSERT INTO Countries
VALUES ('CMR', 'Republic of Cameroon', 'AF');
INSERT INTO Countries
VALUES ('CHN', 'Republic of China', 'AS');
INSERT INTO Countries
VALUES ('COL', 'Republic of Colombia', 'SA');
INSERT INTO Countries
VALUES ('CRI', 'Republic of Costa Rica', 'NA');
INSERT INTO Countries
VALUES ('CUB', 'Republic of Cuba', 'NA');
INSERT INTO Countries
VALUES ('CPV', 'Republic of Cape Verde', 'AF');
INSERT INTO Countries
VALUES ('CXR', 'Christmas Island', 'AS');
INSERT INTO Countries
VALUES ('CYP', 'Republic of Cyprus', 'AS');
INSERT INTO Countries
VALUES ('CZE', 'Czech Republic', 'EU');
INSERT INTO Countries
VALUES ('DEU', 'Federal Republic of Germany', 'EU');
INSERT INTO Countries
VALUES ('DJI', 'Republic of Djibouti', 'AF');
INSERT INTO Countries
VALUES ('DNK', 'Kingdom of Denmark', 'EU');
INSERT INTO Countries
VALUES ('DMA', 'Commonwealth of Dominica', 'NA');
INSERT INTO Countries
VALUES ('DOM', 'Dominican Republic', 'NA');
INSERT INTO Countries
VALUES ('DZA', 'Democratic Republic of Algeria', 'AF');
INSERT INTO Countries
VALUES ('ECU', 'Republic of Ecuador', 'SA');
INSERT INTO Countries
VALUES ('EST', 'Republic of Estonia', 'EU');
INSERT INTO Countries
VALUES ('EGY', 'Arab Republic of Egypt', 'AF');
INSERT INTO Countries
VALUES ('ESH', 'Western Sahara', 'AF');
INSERT INTO Countries
VALUES ('ERI', 'State of Eritrea', 'AF');
INSERT INTO Countries
VALUES ('ESP', 'Kingdom of Spain', 'EU');
INSERT INTO Countries
VALUES ('ETH', 'Federal Democratic Republic of Ethiopia', 'AF');
INSERT INTO Countries
VALUES ('FIN', 'Republic of Finland', 'EU');
INSERT INTO Countries
VALUES ('FJI', 'Republic of the Fiji Islands', 'OC');
INSERT INTO Countries
VALUES ('FLK', 'Falkland Islands (Malvinas)', 'SA');
INSERT INTO Countries
VALUES ('FSM', 'Federated States of Micronesia', 'OC');
INSERT INTO Countries
VALUES ('FRO', 'Faroe Islands', 'EU');
INSERT INTO Countries
VALUES ('FRA', 'French Republic', 'EU');
INSERT INTO Countries
VALUES ('GAB', 'Gabonese Republic', 'AF');
INSERT INTO Countries
VALUES ('GBR', 'United Kingdom of Great Britain & Northern Ireland', 'EU');
INSERT INTO Countries
VALUES ('GRD', 'Grenada', 'NA');
INSERT INTO Countries
VALUES ('GEO', 'Georgia', 'AS');
INSERT INTO Countries
VALUES ('GUF', 'French Guiana', 'SA');
INSERT INTO Countries
VALUES ('GGY', 'Bailiwick of Guernsey', 'EU');
INSERT INTO Countries
VALUES ('GHA', 'Republic of Ghana', 'AF');
INSERT INTO Countries
VALUES ('GIB', 'Gibraltar', 'EU');
INSERT INTO Countries
VALUES ('GRL', 'Greenland', 'NA');
INSERT INTO Countries
VALUES ('GMB', 'Republic of the Gambia', 'AF');
INSERT INTO Countries
VALUES ('GIN', 'Republic of Guinea', 'AF');
INSERT INTO Countries
VALUES ('GLP', 'Guadeloupe', 'NA');
INSERT INTO Countries
VALUES ('GNQ', 'Republic of Equatorial Guinea', 'AF');
INSERT INTO Countries
VALUES ('GRC', 'Hellenic Republic Greece', 'EU');
INSERT INTO Countries
VALUES ('SGS', 'South Georgia and the South Sandwich Islands', 'AN');
INSERT INTO Countries
VALUES ('GTM', 'Republic of Guatemala', 'NA');
INSERT INTO Countries
VALUES ('GUM', 'Guam', 'OC');
INSERT INTO Countries
VALUES ('GNB', 'Republic of Guinea-Bissau', 'AF');
INSERT INTO Countries
VALUES ('GUY', 'Co-operative Republic of Guyana', 'SA');
INSERT INTO Countries
VALUES ('HKG', 'Hong Kong Special Administrative Region of China', 'AS');
INSERT INTO Countries
VALUES ('HMD', 'Heard Island and McDonald Islands', 'AN');
INSERT INTO Countries
VALUES ('HND', 'Republic of Honduras', 'NA');
INSERT INTO Countries
VALUES ('HRV', 'Republic of Croatia', 'EU');
INSERT INTO Countries
VALUES ('HTI', 'Republic of Haiti', 'NA');
INSERT INTO Countries
VALUES ('HUN', 'Republic of Hungary', 'EU');
INSERT INTO Countries
VALUES ('IDN', 'Republic of Indonesia', 'AS');
INSERT INTO Countries
VALUES ('IRL', 'Ireland', 'EU');
INSERT INTO Countries
VALUES ('ISR', 'State of Israel', 'AS');
INSERT INTO Countries
VALUES ('IMN', 'Isle of Man', 'EU');
INSERT INTO Countries
VALUES ('IND', 'Republic of India', 'AS');
INSERT INTO Countries
VALUES ('IOT', 'British Indian Ocean Territory (Chagos Archipelago)', 'AS');
INSERT INTO Countries
VALUES ('IRQ', 'Republic of Iraq', 'AS');
INSERT INTO Countries
VALUES ('IRN', 'Islamic Republic of Iran', 'AS');
INSERT INTO Countries
VALUES ('ISL', 'Republic of Iceland', 'EU');
INSERT INTO Countries
VALUES ('ITA', 'Italian Republic', 'EU');
INSERT INTO Countries
VALUES ('JEY', 'Bailiwick of Jersey', 'EU');
INSERT INTO Countries
VALUES ('JAM', 'Jamaica', 'NA');
INSERT INTO Countries
VALUES ('JOR', 'Hashemite Kingdom of Jordan', 'AS');
INSERT INTO Countries
VALUES ('JPN', 'Japan', 'AS');
INSERT INTO Countries
VALUES ('KEN', 'Republic of Kenya', 'AF');
INSERT INTO Countries
VALUES ('KGZ', 'Kyrgyz Republic', 'AS');
INSERT INTO Countries
VALUES ('KHM', 'Kingdom of Cambodia', 'AS');
INSERT INTO Countries
VALUES ('KIR', 'Republic of Kiribati', 'OC');
INSERT INTO Countries
VALUES ('COM', 'Union of the Comoros', 'AF');
INSERT INTO Countries
VALUES ('KNA', 'Federation of Saint Kitts and Nevis', 'NA');
INSERT INTO Countries
VALUES ('PRK', 'Democratic Republic of Korea', 'AS');
INSERT INTO Countries
VALUES ('KOR', 'Republic of Korea', 'AS');
INSERT INTO Countries
VALUES ('KWT', 'State of Kuwait', 'AS');
INSERT INTO Countries
VALUES ('CYM', 'Cayman Islands', 'NA');
INSERT INTO Countries
VALUES ('KAZ', 'Republic of Kazakhstan', 'AS');
INSERT INTO Countries
VALUES ('LAO', 'Lao Democratic Republic', 'AS');
INSERT INTO Countries
VALUES ('LBN', 'Lebanese Republic', 'AS');
INSERT INTO Countries
VALUES ('LCA', 'Saint Lucia', 'NA');
INSERT INTO Countries
VALUES ('LIE', 'Principality of Liechtenstein', 'EU');
INSERT INTO Countries
VALUES ('LKA', 'Democratic Socialist Republic of Sri Lanka', 'AS');
INSERT INTO Countries
VALUES ('LBR', 'Republic of Liberia', 'AF');
INSERT INTO Countries
VALUES ('LSO', 'Kingdom of Lesotho', 'AF');
INSERT INTO Countries
VALUES ('LTU', 'Republic of Lithuania', 'EU');
INSERT INTO Countries
VALUES ('LUX', 'Grand Duchy of Luxembourg', 'EU');
INSERT INTO Countries
VALUES ('LVA', 'Republic of Latvia', 'EU');
INSERT INTO Countries
VALUES ('LBY', 'Libyan Arab Jamahiriya', 'AF');
INSERT INTO Countries
VALUES ('MAR', 'Kingdom of Morocco', 'AF');
INSERT INTO Countries
VALUES ('MCO', 'Principality of Monaco', 'EU');
INSERT INTO Countries
VALUES ('MDA', 'Republic of Moldova', 'EU');
INSERT INTO Countries
VALUES ('MNE', 'Republic of Montenegro', 'EU');
INSERT INTO Countries
VALUES ('MAF', 'Saint Martin', 'NA');
INSERT INTO Countries
VALUES ('MDG', 'Republic of Madagascar', 'AF');
INSERT INTO Countries
VALUES ('MHL', 'Republic of the Marshall Islands', 'OC');
INSERT INTO Countries
VALUES ('MKD', 'Republic of Macedonia', 'EU');
INSERT INTO Countries
VALUES ('MLI', 'Republic of Mali', 'AF');
INSERT INTO Countries
VALUES ('MMR', 'Union of Myanmar', 'AS');
INSERT INTO Countries
VALUES ('MNG', 'Mongolia', 'AS');
INSERT INTO Countries
VALUES ('MAC', 'Macao Special Administrative Region of China', 'AS');
INSERT INTO Countries
VALUES ('MNP', 'Commonwealth of the Northern Mariana Islands', 'OC');
INSERT INTO Countries
VALUES ('MTQ', 'Martinique', 'NA');
INSERT INTO Countries
VALUES ('MRT', 'Islamic Republic of Mauritania', 'AF');
INSERT INTO Countries
VALUES ('MSR', 'Montserrat', 'NA');
INSERT INTO Countries
VALUES ('MLT', 'Republic of Malta', 'EU');
INSERT INTO Countries
VALUES ('MUS', 'Republic of Mauritius', 'AF');
INSERT INTO Countries
VALUES ('MDV', 'Republic of Maldives', 'AS');
INSERT INTO Countries
VALUES ('MWI', 'Republic of Malawi', 'AF');
INSERT INTO Countries
VALUES ('MEX', 'United Mexican States', 'NA');
INSERT INTO Countries
VALUES ('MYS', 'Malaysia', 'AS');
INSERT INTO Countries
VALUES ('MOZ', 'Republic of Mozambique', 'AF');
INSERT INTO Countries
VALUES ('NAM', 'Republic of Namibia', 'AF');
INSERT INTO Countries
VALUES ('NCL', 'New Caledonia', 'OC');
INSERT INTO Countries
VALUES ('NER', 'Republic of Niger', 'AF');
INSERT INTO Countries
VALUES ('NFK', 'Norfolk Island', 'OC');
INSERT INTO Countries
VALUES ('NGA', 'Federal Republic of Nigeria', 'AF');
INSERT INTO Countries
VALUES ('NIC', 'Republic of Nicaragua', 'NA');
INSERT INTO Countries
VALUES ('NLD', 'Kingdom of the Netherlands', 'EU');
INSERT INTO Countries
VALUES ('NOR', 'Kingdom of Norway', 'EU');
INSERT INTO Countries
VALUES ('NPL', 'State of Nepal', 'AS');
INSERT INTO Countries
VALUES ('NRU', 'Republic of Nauru', 'OC');
INSERT INTO Countries
VALUES ('NIU', 'Niue', 'OC');
INSERT INTO Countries
VALUES ('NZL', 'New Zealand', 'OC');
INSERT INTO Countries
VALUES ('OMN', 'Sultanate of Oman', 'AS');
INSERT INTO Countries
VALUES ('PAN', 'Republic of Panama', 'NA');
INSERT INTO Countries
VALUES ('PER', 'Republic of Peru', 'SA');
INSERT INTO Countries
VALUES ('PYF', 'French Polynesia', 'OC');
INSERT INTO Countries
VALUES ('PNG', 'Independent State of Papua New Guinea', 'OC');
INSERT INTO Countries
VALUES ('PHL', 'Republic of the Philippines', 'AS');
INSERT INTO Countries
VALUES ('PAK', 'Islamic Republic of Pakistan', 'AS');
INSERT INTO Countries
VALUES ('POL', 'Republic of Poland', 'EU');
INSERT INTO Countries
VALUES ('SPM', 'Saint Pierre and Miquelon', 'NA');
INSERT INTO Countries
VALUES ('PCN', 'Pitcairn Islands', 'OC');
INSERT INTO Countries
VALUES ('PRI', 'Commonwealth of Puerto Rico', 'NA');
INSERT INTO Countries
VALUES ('PSE', 'Occupied Palestinian Territory', 'AS');
INSERT INTO Countries
VALUES ('PRT', 'Portuguese Republic', 'EU');
INSERT INTO Countries
VALUES ('PLW', 'Republic of Palau', 'OC');
INSERT INTO Countries
VALUES ('PRY', 'Republic of Paraguay', 'SA');
INSERT INTO Countries
VALUES ('QAT', 'State of Qatar', 'AS');
INSERT INTO Countries
VALUES ('REU', 'Reunion', 'AF');
INSERT INTO Countries
VALUES ('ROU', 'Romania', 'EU');
INSERT INTO Countries
VALUES ('SRB', 'Republic of Serbia', 'EU');
INSERT INTO Countries
VALUES ('RUS', 'Russian Federation', 'EU');
INSERT INTO Countries
VALUES ('RWA', 'Republic of Rwanda', 'AF');
INSERT INTO Countries
VALUES ('SAU', 'Kingdom of Saudi Arabia', 'AS');
INSERT INTO Countries
VALUES ('SLB', 'Solomon Islands', 'OC');
INSERT INTO Countries
VALUES ('SYC', 'Republic of Seychelles', 'AF');
INSERT INTO Countries
VALUES ('SDN', 'Republic of Sudan', 'AF');
INSERT INTO Countries
VALUES ('SWE', 'Kingdom of Sweden', 'EU');
INSERT INTO Countries
VALUES ('SGP', 'Republic of Singapore', 'AS');
INSERT INTO Countries
VALUES ('SHN', 'Saint Helena', 'AF');
INSERT INTO Countries
VALUES ('SVN', 'Republic of Slovenia', 'EU');
INSERT INTO Countries
VALUES ('SJM', 'Svalbard & Jan Mayen Islands', 'EU');
INSERT INTO Countries
VALUES ('SVK', 'Slovakia (Slovak Republic)', 'EU');
INSERT INTO Countries
VALUES ('SLE', 'Republic of Sierra Leone', 'AF');
INSERT INTO Countries
VALUES ('SMR', 'Republic of San Marino', 'EU');
INSERT INTO Countries
VALUES ('SEN', 'Republic of Senegal', 'AF');
INSERT INTO Countries
VALUES ('SOM', 'Somali Republic', 'AF');
INSERT INTO Countries
VALUES ('SUR', 'Republic of Suriname', 'SA');
INSERT INTO Countries
VALUES ('STP', 'Democratic Republic of Sao Tome and Principe', 'AF');
INSERT INTO Countries
VALUES ('SLV', 'Republic of El Salvador', 'NA');
INSERT INTO Countries
VALUES ('SYR', 'Syrian Arab Republic', 'AS');
INSERT INTO Countries
VALUES ('SWZ', 'Kingdom of Swaziland', 'AF');
INSERT INTO Countries
VALUES ('TCA', 'Turks and Caicos Islands', 'NA');
INSERT INTO Countries
VALUES ('TCD', 'Republic of Chad', 'AF');
INSERT INTO Countries
VALUES ('ATF', 'French Southern Territories', 'AN');
INSERT INTO Countries
VALUES ('TGO', 'Togolese Republic', 'AF');
INSERT INTO Countries
VALUES ('THA', 'Kingdom of Thailand', 'AS');
INSERT INTO Countries
VALUES ('TJK', 'Republic of Tajikistan', 'AS');
INSERT INTO Countries
VALUES ('TKL', 'Tokelau', 'OC');
INSERT INTO Countries
VALUES ('TLS', 'Democratic Republic of Timor-Leste', 'AS');
INSERT INTO Countries
VALUES ('TKM', 'Turkmenistan', 'AS');
INSERT INTO Countries
VALUES ('TUN', 'Tunisian Republic', 'AF');
INSERT INTO Countries
VALUES ('TON', 'Kingdom of Tonga', 'OC');
INSERT INTO Countries
VALUES ('TUR', 'Republic of Turkey', 'AS');
INSERT INTO Countries
VALUES ('TTO', 'Republic of Trinidad and Tobago', 'NA');
INSERT INTO Countries
VALUES ('TUV', 'Tuvalu', 'OC');
INSERT INTO Countries
VALUES ('TWN', 'Taiwan', 'AS');
INSERT INTO Countries
VALUES ('TZA', 'United Republic of Tanzania', 'AF');
INSERT INTO Countries
VALUES ('UKR', 'Ukraine', 'EU');
INSERT INTO Countries
VALUES ('UGA', 'Republic of Uganda', 'AF');
INSERT INTO Countries
VALUES ('UMI', 'United States Minor Outlying Islands', 'OC');
INSERT INTO Countries
VALUES ('USA', 'United States of America', 'NA');
INSERT INTO Countries
VALUES ('URY', 'Eastern Republic of Uruguay', 'SA');
INSERT INTO Countries
VALUES ('UZB', 'Republic of Uzbekistan', 'AS');
INSERT INTO Countries
VALUES ('VAT', 'Holy See (Vatican City State)', 'EU');
INSERT INTO Countries
VALUES ('VCT', 'Saint Vincent and the Grenadines', 'NA');
INSERT INTO Countries
VALUES ('VEN', 'Bolivarian Republic of Venezuela', 'SA');
INSERT INTO Countries
VALUES ('VGB', 'British Virgin Islands', 'NA');
INSERT INTO Countries
VALUES ('VIR', 'United States Virgin Islands', 'NA');
INSERT INTO Countries
VALUES ('VNM', 'Socialist Republic of Vietnam', 'AS');
INSERT INTO Countries
VALUES ('VUT', 'Republic of Vanuatu', 'OC');
INSERT INTO Countries
VALUES ('WLF', 'Wallis and Futuna', 'OC');
INSERT INTO Countries
VALUES ('WSM', 'Independent State of Samoa', 'OC');
INSERT INTO Countries
VALUES ('YEM', 'Yemen', 'AS');
INSERT INTO Countries
VALUES ('MYT', 'Mayotte', 'AF');
INSERT INTO Countries
VALUES ('ZAF', 'Republic of South Africa', 'AF');
INSERT INTO Countries
VALUES ('ZMB', 'Republic of Zambia', 'AF');
INSERT INTO Countries
VALUES ('ZWE', 'Republic of Zimbabwe', 'AF');

INSERT INTO Servers
VALUES ('Main Spanish Server', 'ESP');



