#ifndef LISTEELEVESPAGE_H
#define LISTEELEVESPAGE_H

#include <QWidget>

#include "data/eleverepository.h"

class QLineEdit;
class QTableWidget;

// Page "Liste des élèves" : tableau, recherche, actualisation, suppression
class ListeElevesPage : public QWidget
{
    Q_OBJECT

public:
    explicit ListeElevesPage(QWidget *parent = nullptr);

public slots:
    //Fonction pour charger les eleves depuis le fichier
    void chargerEleves();

signals:
    //Le fichier des élèves a (peut-être) changé
    void donneesChangees();

private slots:
    void rechercher(const QString &recherche);
    void supprimerSelection();

private:
    EleveRepository m_repo;

    QTableWidget *m_table;
    QLineEdit *m_searchEdit;
};

#endif // LISTEELEVESPAGE_H
