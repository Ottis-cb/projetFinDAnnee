#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>

#include "data/eleverepository.h"

class QLabel;

// Page d'accueil : tableau de bord avec les statistiques
class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(QWidget *parent = nullptr);

public slots:
    //Fonction pour calculer les statistiques
    void actualiserStatistiques();

signals:
    //Clic sur "Preparer une inscription"
    void inscriptionDemandee();

private:
    EleveRepository m_repo;

    //afficheurs de nombres
    QLabel *m_totalNumber = nullptr;
    QLabel *m_garconsNumber = nullptr;
    QLabel *m_fillesNumber = nullptr;
};

#endif // DASHBOARDPAGE_H
