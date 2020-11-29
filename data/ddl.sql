--=======================================================================
-- File:        ddl.sql
-- Description: ddl for our museen project
-- Authors: Aëlien MOUBECHE, Adel ABBAS
-- Created:     2020-11-27
--========================================================================

CREATE TABLE pricing
(
    pricing_id          CHAR(4),
    pricing_price       FLOAT NOT NULL,
    pricing_name        VARCHAR(20),
    pricing_description VARCHAR(50),
    CONSTRAINT pricing_pk PRIMARY KEY (pricing_id)
);

CREATE TABLE subscription
(
    subscription_id       CHAR(4)
        CONSTRAINT subscription_id_format CHECK (subscription_id LIKE 'S%'),
    subscription_duration INT NOT NULL,
    CONSTRAINT subscription_pk PRIMARY KEY (subscription_id),
    CONSTRAINT subscription_fk FOREIGN KEY (subscription_id) REFERENCES pricing (pricing_id)
);

CREATE TABLE guided_tour
(
    gt_id         CHAR(4)
        CONSTRAINT gt_id_format CHECK (gt_id LIKE 'G%'),
    gt_guide_name VARCHAR(15) NOT NULL,
    gt_day        CHAR(2)     NOT NULL,
    gt_hour       TIME        NOT NULL,
    CONSTRAINT gt_pk PRIMARY KEY (gt_id),
    CONSTRAINT gt_id_fk FOREIGN KEY (gt_id) REFERENCES pricing (pricing_id)
);

CREATE TABLE visitor
(
    visitor_id            VARCHAR(20),
    visitor_first_name    VARCHAR(20)         NOT NULL,
    visitor_last_name     VARCHAR(20)         NOT NULL,
    visitor_birthdate     DATE                NOT NULL,
    visitor_password_hash VARCHAR(255)        NOT NULL,
    visitor_authkey_hash  VARCHAR(255) UNIQUE NOT NULL,
    CONSTRAINT visitor_pk PRIMARY KEY (visitor_id),
    CONSTRAINT visitor_id_min_length CHECK (length(visitor_id) >= 4)
);

CREATE TABLE is_paying
(
    ip_visitor_id       VARCHAR(20),
    ip_pricing_id       CHAR(4),
    ip_reservation_date DATE,
    ip_payment_date     DATE    NOT NULL,
    ip_auto_renewed     BOOLEAN NOT NULL DEFAULT FALSE,
    CONSTRAINT ip_pk PRIMARY KEY (ip_visitor_id, ip_pricing_id, ip_reservation_date),
    CONSTRAINT ip_fk_visitor FOREIGN KEY (ip_visitor_id) REFERENCES visitor (visitor_id),
    CONSTRAINT ip_fk_pricing FOREIGN KEY (ip_pricing_id) REFERENCES pricing (pricing_id)
);

CREATE TABLE location
(
    loc_id        SERIAL,
    loc_room_name VARCHAR(30) NOT NULL,
    CONSTRAINT location_pk PRIMARY KEY (loc_id)
);

CREATE TABLE location_history
(
    location_visitor_id VARCHAR(20),
    location_id         INTEGER,
    location_time_in    TIMESTAMP,
    location_time_out   TIMESTAMP,
    CONSTRAINT location_history_pk PRIMARY KEY (location_visitor_id, location_id, location_time_in),
    CONSTRAINT location_history_fk_visitor FOREIGN KEY (location_visitor_id) REFERENCES visitor (visitor_id),
    CONSTRAINT location_history_fk_location FOREIGN KEY (location_id) REFERENCES location (loc_id)
);

CREATE TABLE artist
(
    artist_id                SERIAL,
    artist_name              VARCHAR(20) NOT NULL,
    artist_birth             DATE,
    artist_death             DATE,
    artist_artistic_movement INTEGER,
    artist_bio               VARCHAR(150),
    CONSTRAINT artist_pk PRIMARY KEY (artist_id)
);

CREATE TABLE artistic_movement
(
    artistic_movement_id          SERIAL,
    artistic_movement_name        VARCHAR(30) NOT NULL,
    artistic_movement_lead_artist VARCHAR(30) NOT NULL,
    CONSTRAINT artistic_movement_pk PRIMARY KEY (artistic_movement_id)
);

CREATE TYPE art_type AS ENUM ('Sculpture', 'Tableau');

CREATE TABLE artwork
(
    artwork_id          SERIAL,
    artwork_title       VARCHAR(30) NOT NULL,
    artwork_type        art_type    NOT NULL,
    artwork_artist      INTEGER     NOT NULL,
    artwork_date        VARCHAR(10) NOT NULL,
    artwork_movement_id INTEGER     NOT NULL,
    artwork_popularity  INTEGER,
    artwork_location    INTEGER     NOT NULL,
    CONSTRAINT artwork_pk PRIMARY KEY (artwork_id),
    CONSTRAINT artwork_fk_artist FOREIGN KEY (artwork_artist) REFERENCES artist (artist_id),
    CONSTRAINT artwork_fk_movement FOREIGN KEY (artwork_movement_id) REFERENCES artistic_movement (artistic_movement_id)
);

CREATE TABLE artist_preference
(
    ap_visitor_id VARCHAR(20),
    ap_artist_id  INTEGER,
    CONSTRAINT artist_preference_pk PRIMARY KEY (ap_visitor_id, ap_artist_id),
    CONSTRAINT artist_preference_fk_visitor FOREIGN KEY (ap_visitor_id) REFERENCES visitor (visitor_id),
    CONSTRAINT artist_preference_fk_artist FOREIGN KEY (ap_artist_id) REFERENCES artist (artist_id)
);

CREATE TABLE artistic_movement_preference
(
    amp_visitor_id           VARCHAR(20),
    amp_artistic_movement_id INTEGER,
    amp_score                INT NOT NULL,
    CONSTRAINT artist_movement_preference_pk PRIMARY KEY (amp_visitor_id, amp_artistic_movement_id),
    CONSTRAINT artist_movement_preference_fk_visitor FOREIGN KEY (amp_visitor_id) REFERENCES visitor (visitor_id),
    CONSTRAINT artist_movement_preference_fk_artist FOREIGN KEY (amp_artistic_movement_id) REFERENCES artistic_movement (artistic_movement_id)
);

INSERT INTO pricing (pricing_id, pricing_price, pricing_name, pricing_description)
VALUES ('POO1', 10, 'Entrée simple', 'Entrée simple'),
       ('POO2', 5, 'Entrée tarif réduit', 'Tarif réduit (-50%)'),
       ('S001', 100, 'Abonnement annuel', 'Accès illimité au musée pendant 1 an'),
       ('G001', 15, 'Visite guidée', 'Visite guidée');

INSERT INTO subscription (subscription_id, subscription_duration)
VALUES ('S001', 365);

INSERT INTO guided_tour(gt_id, gt_guide_name, gt_day, gt_hour, gt_place)
VALUES ('G001', 'Gilles', 'Lu', '09:00', 'Hall');

INSERT INTO visitor(visitor_id, visitor_first_name, visitor_last_name, visitor_birthdate, visitor_password_hash,
                    visitor_authkey_hash)
VALUES ('jtest', 'Jean', 'Test', '2020-01-01', 'museen', '8af763ee3a7549af94f472543bff710e09db9b217ab8e35a6c542db8e6330b0d');

INSERT INTO artistic_movement(artistic_movement_name, artistic_movement_lead_artist) VALUES
('Surréalisme', 'André Breton');

INSERT INTO artist(artist_name, artist_birth, artist_death, artist_artistic_movement, artist_bio)
VALUES ('René Magritte', '1898-11-21', '1967-08-15', 1,
        'René Magritte, né le 21 novembre 1898 à Lessines dans le Hainaut (Belgique) et mort à Bruxelles le 15 août 1967, est un peintre surréaliste belge.');
INSERT INTO artwork(artwork_title, artwork_type, artwork_artist, artwork_date, artwork_movement_id,
                    artwork_location)
VALUES ('La Trahison des images', 'Tableau', 1, '1928–1929', 1, 1);
INSERT INTO location (loc_room_name)
VALUES ('Grande salle'),
       ('Amphithéâtre'),
       ('Hall Napoléon'),
       ('Pyramide'),
       ('Galerie Médicis');