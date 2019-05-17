drop database assodb;
drop user asso@localhost;
drop user asso;

CREATE DATABASE assodb;

CREATE USER 'asso'@'localhost' IDENTIFIED WITH mysql_native_password BY 'password';
CREATE USER 'asso'@'%' IDENTIFIED WITH mysql_native_password BY 'password';

GRANT ALL ON assodb.* TO 'asso'@'localhost';
GRANT ALL ON assodb.* TO 'asso'@'%';

USE assodb;

CREATE TABLE Intervenant (
  idIntervenant INT UNSIGNED NOT NULL AUTO_INCREMENT,
  titre TINYTEXT,
  nom TINYTEXT,
  prenom TINYTEXT,
  adresse TINYTEXT,
  codePostal TINYTEXT,
  ville TINYTEXT,
  telephone TINYTEXT,
  email TINYTEXT,
  remarque LONGTEXT,
  PRIMARY KEY(idIntervenant)
);

CREATE TABLE Compte (
  idCompte INT UNSIGNED NOT NULL AUTO_INCREMENT,
  date DATE NOT NULL,
  montant DOUBLE NOT NULL,
  montantDepart DOUBLE NOT NULL,
  remarque LONGTEXT,
  PRIMARY KEY(idCompte)
);

CREATE TABLE Animal (
  idAnimal INT UNSIGNED NOT NULL AUTO_INCREMENT,
  type ENUM('chat', 'chien', 'autre') NOT NULL,
  identification TINYTEXT,
  nom TINYTEXT NOT NULL,
  description TINYTEXT NOT NULL,
  remarque LONGTEXT,
  PRIMARY KEY(idAnimal)
);

CREATE TABLE Objet (
  idObjet INT UNSIGNED NOT NULL AUTO_INCREMENT,
  description TINYTEXT NOT NULL,
  remarque LONGTEXT,
  PRIMARY KEY(idObjet)
);

CREATE TABLE Action (
  idAction INT UNSIGNED NOT NULL AUTO_INCREMENT,
  type ENUM('manifestation', 'donation', 'adhesion', 'subvention', 'vente', 'achat', 'adoption', 'sterilisation', 'soin', 'placement', 'entreeAnimal', 'sortieAnimal') NOT NULL,
  date DATE NOT NULL,
  depense DOUBLE NOT NULL DEFAULT 0,
  recette DOUBLE NOT NULL DEFAULT 0,
  remarque LONGTEXT,
  idIntervenant INT UNSIGNED REFERENCES Intervenant(idIntervenant),
  idAnimal INT UNSIGNED REFERENCES Animal(idAnimal),
  idObjet INT UNSIGNED REFERENCES Objet(idObjet),
  idCompte INT UNSIGNED NOT NULL REFERENCES Compte(idCompte),
  PRIMARY KEY(idAction)
);

delimiter |
CREATE TRIGGER update_compte_after_action_insert AFTER INSERT ON Action
FOR EACH ROW
BEGIN
DECLARE m DOUBLE;
SET m = (SELECT Compte.montant FROM Compte WHERE Compte.idCompte = NEW.idCompte);
IF NEW.idCompte <> 0 THEN
SET m = m + (NEW.recette - NEW.depense);
UPDATE Compte SET Compte.montant = m WHERE Compte.idCompte = NEW.idCompte;
END IF;
END|
delimiter ;

delimiter |
CREATE TRIGGER update_compte_after_action_update AFTER UPDATE ON Action
FOR EACH ROW
BEGIN
DECLARE m DOUBLE;
SET m = (SELECT Compte.montant FROM Compte WHERE Compte.idCompte = NEW.idCompte);
IF NEW.idCompte <> 0 THEN
SET m = m - (OLD.recette - OLD.depense);
SET m = m + (NEW.recette - NEW.depense);
UPDATE Compte SET Compte.montant = m WHERE Compte.idCompte = NEW.idCompte;
END IF;
END|
delimiter ;

delimiter |
CREATE TRIGGER update_compte_after_action_delete AFTER DELETE ON Action
FOR EACH ROW
BEGIN
DECLARE m DOUBLE;
SET m = (SELECT Compte.montant FROM Compte WHERE Compte.idCompte = OLD.idCompte);
IF OLD.idCompte <> 0 THEN
SET m = m - (OLD.recette - OLD.depense);
UPDATE Compte SET Compte.montant = m WHERE Compte.idCompte = OLD.idCompte;
END IF;
END|
delimiter ;
