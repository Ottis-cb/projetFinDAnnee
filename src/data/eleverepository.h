#ifndef ELEVEREPOSITORY_H
#define ELEVEREPOSITORY_H

#include <QList>
#include <QString>
#include <QStringList>

// Accès au fichier "eleves.txt".
// Une ligne = un élève = 8 champs séparés par des points-virgules.
// C'est le SEUL endroit du projet qui ouvre ce fichier.
class EleveRepository
{
public:
    // Position de chaque champ dans une ligne
    enum Champ : int {
        Matricule = 0,
        Nom,
        Prenom,
        DateNaissance,
        Sexe,
        Niveau,
        Telephone,
        Adresse,
        NombreChamps
    };

    enum class ResultatSuppression {
        Ok,
        ErreurLecture,
        ErreurEcriture
    };

    // Nom du fichier de données
    static QString chemin();

    bool existe() const;

    // Lit tous les élèves valides (lignes de 8 champs).
    // Retourne false si le fichier ne peut pas être ouvert.
    bool lireTous(QList<QStringList> &eleves) const;

    // Ajoute un élève (8 champs) à la fin du fichier.
    bool ajouter(const QStringList &champs) const;

    // Supprime l'élève ayant ce matricule.
    ResultatSuppression supprimer(const QString &matricule) const;
};

#endif // ELEVEREPOSITORY_H
