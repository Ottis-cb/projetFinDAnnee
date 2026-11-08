#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QFrame>

class QPushButton;

// Barre latérale de navigation (menu principal)
class SideBar : public QFrame
{
    Q_OBJECT

public:
    enum Page {
        Accueil,
        Inscription,
        Eleves
    };

    explicit SideBar(QWidget *parent = nullptr);

    //Fonction pour gérer le bouton actif
    void setMenuActif(Page page);

signals:
    void accueilClicked();
    void inscriptionClicked();
    void elevesClicked();

private:
    QPushButton *m_accueil;
    QPushButton *m_inscription;
    QPushButton *m_eleves;
};

#endif // SIDEBAR_H
