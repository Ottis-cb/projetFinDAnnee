#include "eleverepository.h"

#include <QFile>
#include <QTextStream>

QString EleveRepository::chemin()
{
    return "eleves.txt";
}

bool EleveRepository::existe() const
{
    return QFile::exists(chemin());
}

bool EleveRepository::lireTous(QList<QStringList> &eleves) const
{
    eleves.clear();

    //Ouvrir le fichier en lecture
    QFile fichier(chemin());
    if (!fichier.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    //Creer un flux de lecture
    QTextStream entree(&fichier);

    //Lire les fichiers ligne par ligne
    while (!entree.atEnd()) {
        QString ligne = entree.readLine();

        //Decouper la ligne avec le point-virgule
        QStringList informations = ligne.split(';');

        //Verifier que la ligne contient 8 informations
        if (informations.size() != NombreChamps) {
            continue;
        }

        eleves.append(informations);
    }

    fichier.close();
    return true;
}

bool EleveRepository::ajouter(const QStringList &champs) const
{
    //Ouvrir le fichier en mode ajout
    QFile fichier(chemin());

    if (!fichier.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        return false;
    }

    //Ecrire les infos dans le fichier
    QTextStream sortie(&fichier);
    sortie << champs.join(";") << "\n";

    //fermer le fichier
    fichier.close();
    return true;
}

EleveRepository::ResultatSuppression EleveRepository::supprimer(const QString &matricule) const
{
    //Lire tous les élèves sauf celui à supprimer
    QFile fichier(chemin());

    if (!fichier.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return ResultatSuppression::ErreurLecture;
    }

    QStringList lignes;
    QTextStream entree(&fichier);

    while (!entree.atEnd()) {
        QString ligneTexte = entree.readLine();
        QStringList infos = ligneTexte.split(';');

        if (infos.size() == NombreChamps && infos.at(Matricule).trimmed() != matricule) {
            lignes.append(ligneTexte);
        }
    }

    fichier.close();

    //Réecrire le fichier sans l'élève supprimé
    if (!fichier.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return ResultatSuppression::ErreurEcriture;
    }

    QTextStream sortie(&fichier);
    for (const QString &ligneTexte : lignes) {
        sortie << ligneTexte << "\n";
    }
    fichier.close();

    return ResultatSuppression::Ok;
}
