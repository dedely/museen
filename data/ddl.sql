--=======================================================================
-- File:        ddl.sql
-- Description: ddl for our museen project
-- Authors: Aëlien MOUBECHE, Adel ABBAS
-- Created:     2020-11-03
--========================================================================

CREATE TABLE pricing
(
    pricing_id          CHAR(3),
    pricing_price       FLOAT NOT NULL,
    pricing_name        VARCHAR(10),
    pricing_description VARCHAR(50),
    CONSTRAINT pricing_pk PRIMARY KEY (pricing_id)
);

CREATE TABLE subscription
(
    subscription_id       CHAR(3),
    subscription_duration INT     NOT NULL,
    subscription_renewed  BOOLEAN NOT NULL,
    CONSTRAINT subscription_pk PRIMARY KEY (subscription_id),
    CONSTRAINT subscription_fk FOREIGN KEY (subscription_id) REFERENCES pricing (pricing_id)
);

CREATE TABLE guided_tour
(
    gt_id         CHAR(3),
    gt_guide_name VARCHAR(15) NOT NULL,
    gt_date       CHAR(2)        NOT NULL,
    gt_hour       TIME        NOT NULL,
    gt_place      VARCHAR(15) NOT NULL,
    CONSTRAINT gt_pk PRIMARY KEY (gt_id),
    CONSTRAINT gt_id_fk FOREIGN KEY (gt_id) REFERENCES pricing (pricing_id)
);

CREATE TABLE visitor
(
    visitor_id         CHAR(10),
    visitor_first_name VARCHAR(20) NOT NULL,
    visitor_last_name  VARCHAR(20) NOT NULL,
    visitor_birthdate  DATE        NOT NULL,
    visitor_pricing    CHAR(3),
    CONSTRAINT visitor_pk PRIMARY KEY (visitor_id),
    CONSTRAINT visitor_fk FOREIGN KEY (visitor_pricing) REFERENCES pricing (pricing_id)
);

CREATE TABLE is_paying
(
    ip_visitor_id       CHAR(10),
    ip_pricing_id       CHAR(3),
    ip_reservation_date DATE NOT NULL,
    ip_payment_date     DATE NOT NULL,
    CONSTRAINT ip_pk PRIMARY KEY (ip_visitor_id, ip_pricing_id),
    CONSTRAINT ip_fk_visitor FOREIGN KEY (ip_visitor_id) REFERENCES visitor (visitor_id),
    CONSTRAINT ip_fk_pricing FOREIGN KEY (ip_pricing_id) REFERENCES pricing (pricing_id)
);

CREATE TABLE location
(
    loc_id        CHAR(8),
    loc_room_name VARCHAR(30) NOT NULL,
    CONSTRAINT location_pk PRIMARY KEY (loc_id)
);

CREATE TABLE location_history
(
    location_visitor_id CHAR(10),
    location_id         CHAR(8),
    location_time_in    TIMESTAMP,
    location_time_out   TIMESTAMP,
    CONSTRAINT location_history_pk PRIMARY KEY (location_visitor_id, location_id, location_time_in),
    CONSTRAINT location_history_fk_visitor FOREIGN KEY (location_visitor_id) REFERENCES visitor (visitor_id),
    CONSTRAINT location_history_fk_location FOREIGN KEY (location_id) REFERENCES location (loc_id)
);

CREATE TABLE artist
(
    artist_id                CHAR(8),
    artist_name              VARCHAR(20) DEFAULT 'Artiste Inconnu' NOT NULL,
    artist_birth             DATE,
    artist_death             DATE,
    artist_artistic_movement CHAR(3),
    artist_bio               VARCHAR(150),
    CONSTRAINT artist_pk PRIMARY KEY (artist_id)
);

CREATE TABLE artistic_movement
(
    artistic_movement_id          CHAR(3),
    artistic_movement_name        VARCHAR(30) NOT NULL,
    artistic_movement_lead_artist VARCHAR(30) NOT NULL,
    CONSTRAINT artistic_movement_pk PRIMARY KEY (artistic_movement_id)
);

CREATE TYPE art_type AS ENUM ('Sculpture', 'Tableau');

CREATE TABLE artwork
(
    artwork_id          CHAR(11),
    artwork_type        art_type    NOT NULL,
    artwork_artist      CHAR(8)     NOT NULL,
    artwork_date        VARCHAR(10) NOT NULL,
    artwork_movement_id CHAR(3)     NOT NULL,
    artwork_popularity  INT,
    artwork_location    CHAR(8)     NOT NULL,
    CONSTRAINT artwork_pk PRIMARY KEY (artwork_id),
    CONSTRAINT artwork_fk_artist FOREIGN KEY (artwork_artist) REFERENCES artist (artist_id),
    CONSTRAINT artwork_fk_movement FOREIGN KEY (artwork_movement_id) REFERENCES artistic_movement (artistic_movement_id)
);

CREATE TABLE artist_preference
(
    ap_visitor_id CHAR(10),
    ap_artist_id  CHAR(8),
    CONSTRAINT artist_preference_pk PRIMARY KEY (ap_visitor_id, ap_artist_id),
    CONSTRAINT artist_preference_fk_visitor FOREIGN KEY (ap_visitor_id) REFERENCES visitor (visitor_id),
    CONSTRAINT artist_preference_fk_artist FOREIGN KEY (ap_artist_id) REFERENCES artist (artist_id)
);

CREATE TABLE artistic_movement_preference
(
    amp_visitor_id           CHAR(10),
    amp_artistic_movement_id CHAR(4),
    amp_score                INT NOT NULL,
    CONSTRAINT artist_movement_preference_pk PRIMARY KEY (amp_visitor_id, amp_artistic_movement_id),
    CONSTRAINT artist_movement_preference_fk_visitor FOREIGN KEY (amp_visitor_id) REFERENCES visitor (visitor_id),
    CONSTRAINT artist_movement_preference_fk_artist FOREIGN KEY (amp_artistic_movement_id) REFERENCES artistic_movement (artistic_movement_id)
);