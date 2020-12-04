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
    visitor_first_name    VARCHAR(20)        NOT NULL,
    visitor_last_name     VARCHAR(20)        NOT NULL,
    visitor_birthdate     DATE               NOT NULL,
    visitor_password_hash VARCHAR(64)        NOT NULL, --sha256 hashes are 64 characters long
    visitor_authkey_hash  VARCHAR(64) UNIQUE NOT NULL, --sha256 hashes are 64 characters long
    CONSTRAINT visitor_pk PRIMARY KEY (visitor_id),
    CONSTRAINT visitor_id_min_length CHECK (length(visitor_id) >= 4),
    CONSTRAINT visitor_id_max_length CHECK (length(visitor_id) <= 20)
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
    artist_id    SERIAL,
    artist_first_name  VARCHAR(20) NOT NULL,
    artist_last_name VARCHAR(20),
    artist_birth DATE,
    artist_death DATE,
    artist_bio   VARCHAR(150),
    CONSTRAINT artist_pk PRIMARY KEY (artist_id)
);

CREATE TABLE artistic_movement
(
    artistic_movement_id   SERIAL,
    artistic_movement_name VARCHAR(30) NOT NULL,
    CONSTRAINT artistic_movement_pk PRIMARY KEY (artistic_movement_id)
);

CREATE TYPE art_type AS ENUM ('Sculpture', 'Tableau');

CREATE TABLE artwork
(
    artwork_id          SERIAL,
    artwork_title       VARCHAR(50) NOT NULL,
    artwork_type        art_type    NOT NULL,
    artwork_artist      INTEGER     NOT NULL,
    artwork_date        VARCHAR(10) NOT NULL,
    artwork_movement_id INTEGER     NOT NULL,
    artwork_popularity  INTEGER     NOT NULL DEFAULT 1,
    artwork_location    INTEGER     NOT NULL,
    CONSTRAINT artwork_pk PRIMARY KEY (artwork_id),
    CONSTRAINT artwork_fk_artist FOREIGN KEY (artwork_artist) REFERENCES artist (artist_id),
    CONSTRAINT artwork_fk_movement FOREIGN KEY (artwork_movement_id) REFERENCES artistic_movement (artistic_movement_id),
    CONSTRAINT artwork_min_popularity CHECK (artwork_popularity >= 1),
    CONSTRAINT artwork_max_popularity CHECK (artwork_popularity <= 10)
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
    CONSTRAINT artist_movement_preference_fk_artist FOREIGN KEY (amp_artistic_movement_id) REFERENCES artistic_movement (artistic_movement_id),
    CONSTRAINT artist_movement_preference_min_score CHECK (amp_score >= 1),
    CONSTRAINT artist_movement_preference_max_score CHECK (amp_score <= 5)
);


INSERT INTO pricing (pricing_id, pricing_price, pricing_name, pricing_description)
VALUES ('P001', 10, 'Entrée simple', 'Entrée simple'),
       ('P002', 5, 'Entrée tarif réduit', 'Tarif réduit (-50%)'),
       ('S001', 100, 'Abonnement annuel', 'Accès illimité au musée pendant 1 an'),('S002', 20, 'Abonnement mensuel', 'Accès illimité au musée pendant 30 jours'),
       ('G001', 15, 'Visite guidée', 'Visite guidée');

INSERT INTO subscription (subscription_id, subscription_duration)
VALUES ('S001', 365),
       ('S002', 30);

INSERT INTO guided_tour(gt_id, gt_guide_name, gt_day, gt_hour)
VALUES ('G001', 'Gilles', 'Lu', '09:00');

INSERT INTO visitor(visitor_id, visitor_first_name, visitor_last_name, visitor_birthdate, visitor_password_hash,
                    visitor_authkey_hash)
VALUES ('jtest', 'Jean', 'Test', '2020-01-01', '7bd5bcac8fb0b44d24ddbacc11c8cca0f48b65a8736212625096506947a39347',
        '8af763ee3a7549af94f472543bff710e09db9b217ab8e35a6c542db8e6330b0d'),
       ('paulognaise', 'Paulo', 'Niaise', '1999-05-30', '5b0bf9dd4e84e66331c1b4c81f51e234a54277cea9031df591a79ed330f5dd83',
        '6ae257767546adc52587faa6ab3fa00b40277e55cc57bc2b2de12d7d68c4e503'),
       ('ML_student', 'Marco', 'Ladjoint', '1960-06-12', 'fa0a212adc1799e50b07edf5251d6e402160397fcc8291197e3225d4202ec5bd', '13ae1f481c28f3760f658bc959e9583a5d08009449b8adaed16d0820bf196222'),
       ('TNDD', 'Désirée-Delphine', 'Tang Nook', '1976-12-24', '182e0187ef4dd6db9569b93196bfc5e5ece23d9fca405d9bfc12f307b2891a8f', 'c37016ea1ba649e00dc59a251aac61f259018dd369db0fdc5869dee7c4cc5e50'),
       ('dedely', 'Adel', 'Abbas', '1999-02-12', '78b6248bb9c04928730b56f9ea27fb6b90d639cb9bd38b06e13c51b9fcd1693e', 'dc3e9c824e7c8f2b4eb1e1b9f7ec6afe7513613e580ec3147065e7b162e669d4'),
       ('HeroAllistair', 'Aelien', 'Moubeche', '2001-04-30', 'bf63b40611e322e9457a7cdb362eb79b5be409b33474806c7b38f579da607a99', 'a67332fadb6e881985876096e4579df6223eb3e07578ffb82f0d5c387219e262');

INSERT INTO is_paying(ip_visitor_id, ip_pricing_id, ip_reservation_date, ip_payment_date, ip_auto_renewed)
VALUES ('jtest', 'P001', '2020-12-01', '2020-11-27', FALSE),
       ('ML_student', 'S001', '2020-01-01', '2019-12-31', FALSE),
       ('TNDD', 'S002', '2020-12-01', '2020-11-27', TRUE),
       ('paulognaise', 'G001', '2020-12-08', '2020-12-02', FALSE),
       ('dedely', 'P002', '2020-12-08', '2020-12-02', FALSE),
       ('HeroAllistair', 'P002', '2020-12-08', '2020-12-01', FALSE);

INSERT INTO location (loc_room_name)
VALUES ('Hall'),
       ('Caisses'),
       ('Toilettes'),
       ('Restaurant'),
       ('Galerie Médicis'),
       ('Galerie Médicis'),
       ('Galerie Médicis'),
       ('Galerie Médicis'),
       ('Galerie Médicis'),
       ('Galerie Médicis'),
       ('Galerie Médicis'),
       ('Galerie Médicis'),
       ('Galerie Médicis'),
       ('Pyramide'),
       ('Pyramide'),
       ('Pyramide'),
       ('Pyramide'),
       ('Pyramide'),
       ('Pyramide'),
       ('Pyramide'),
       ('Pyramide'),
       ('Espace temporaire'),
       ('Espace temporaire'),
       ('Espace temporaire'),
       ('Espace temporaire');


INSERT INTO artistic_movement(artistic_movement_name)
VALUES ('Surréalisme'),
       ('Cubisme'),
       ('Romantisme'),
       ('Impressionnisme'),
       ('Renaissance'),
       ('Expressionisme'),
       ('Baroque'),
       ('Réalisme'),
       ('Symbolisme');

INSERT INTO artist(artist_first_name, artist_last_name, artist_birth, artist_death, artist_bio)
VALUES ('René', 'Magritte', '1898-11-21', '1967-08-15', 'Artiste Belge'),
       ('Camille', 'Pissaro', '1830-07-10', '1903-11-13', 'Artiste Franco-danois'),
       ('Robert', 'Delaunay', '1885-04-12', '1941-10-25', 'Artiste Français'),
       ('Vassily', 'Kandinsky', '1866-12-4', '1944-12-13', 'Artiste Russe'),
       ('Alvar', 'Cawen', '1886-06-08', '1935-10-03', 'Artiste Finlandais'),
       ('Edgar', 'Degas', '1834-07-19', '1917-09-27', 'Artiste Français'),
       ('Caspar David', 'Friedrich', '1774-09-05', '1840-05-07', 'Artiste Prussien'),
       ('Vincent', 'Van Gogh', '1853-03-30', '1890-07-29', 'Artiste Néerlandais ayant vécu en France'),
       ('Claude', 'Monet', '1840-11-14', '1926-12-05', 'Artiste Français'),
       ('Pablo', 'Picasso', '1881-10-25', '1973-04-08', 'Artiste Espagnol ayant vécu en France'),
       ('Gustave', 'Courbet', '1819-06-10', '1877-12-31', 'Artiste Français'),
       ('Paul', 'Cézanne', '1839-01-19', '1906-10-22', 'Artiste Français'),
       ('Edouard', 'Manet', '1832-01-23', '1883-04-30 ', 'Artiste Français'),
       ('Auguste', 'Renoir', '1841-02-25', '1919-12-03', 'Artiste Français'),
       ('Rembrandt', NULL,'1606-07-15', '1669-10-04', 'Artiste Néerlandais'),
       ('Pierre Paul', 'Rubens', '1577-06-28', '1640-05-30', 'Artiste Néerlandais'),
       ('Michel-Ange', NULL, '1475-03-06', '1564-02-18', 'Artiste de Florence');
       

INSERT INTO artwork(artwork_title, artwork_type, artwork_artist, artwork_date, artwork_movement_id, artwork_popularity, artwork_location)
VALUES ('La Trahison des images', 'Tableau', 1, '1928–1929', 1, 7, 5),
       ('Le Boulevard de Montmartre, matin d hiver', 'Tableau', 2, '1897', 4, 3, 6),
       ('La Ville n°2', 'Tableau', 3, '1910', 2, 4, 7),
       ('Fugue', 'Tableau', 4, '1914', 6, 6, 8),
       ('Sokea Soittoniekka', 'Tableau', 5, '1922', 6, 3, 9),
       ('La classe de danse', 'Tableau', 6, '1874', 4, 9, 10),
       ('Olympia', 'Tableau', 13, '1863', 4, 10, 11),
       ('Femme s essuyant', 'Tableau', 6, 'vers 1890', 4, 6, 12),
       ('Le voyageur au-dessus de la mer de nuages', 'Tableau', 7, '1818', 3, 10, 13),
       ('Falaises de craie sur l île de Rügen', 'Tableau', 7, '1818', 3, 5, 14),
       ('La Ronde de nuit', 'Tableau', 15, '1642', 7, 6, 15),
       ('Les Syndics', 'Tableau', 15, '1662', 7, 7, 16),
       ('La Fourrure', 'Tableau', 16, '1638', 7, 3, 17),
       ('Pietà Rondanini', 'Sculpture', 17, 'vers 1564', 5, 4, 18),
       ('Terrasse du café le soir', 'Tableau', 8, '1988', 9, 4, 19),
       ('L entrée du port de Trouville', 'Tableau', 9, '1870', 4, 5, 20),
       ('Las Meninas', 'Tableau', 10, '1957', 2, 8, 21),
       ('L Origine du monde', 'Tableau', 11, '1866', 8, 10, 22),
       ('La Maison du pendu', 'Tableau', 12, '1873', 4, 3, 23),
       ('Un bar aux Folies Bergère', 'Tableau', 13, '1882', 4, 9, 24),
       ('La Loge', 'Tableau', 14,  '1874', 4, 1, 25);
	   
INSERT INTO artistic_movement_preference(amp_visitor_id, amp_artistic_movement_id, amp_score)
VALUES ('HeroAllistair', 4, 5),
	   ('HeroAllistair', 6, 4),
	   ('HeroAllistair', 8, 3),
	   ('dedely',4,5),
	   ('dedely',2,4),
	   ('dedely',3,3);

	   
INSERT INTO artist_preference(ap_visitor_id, ap_artist_id)
VALUES ('HeroAllistair', 4),
	('HeroAllistair', 12),
	('HeroAllistair', 9),
	('dedely',13),
	('dedely',12),
	('dedely',10);