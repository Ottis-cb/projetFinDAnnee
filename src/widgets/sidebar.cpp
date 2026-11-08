#include "sidebar.h"

#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>

SideBar::SideBar(QWidget *parent)
    : QFrame(parent)
{
    //Barre laterale
    setObjectName("sidebar");
    setFixedWidth(220);

    QVBoxLayout *sideLayout = new QVBoxLayout(this);
    sideLayout->setContentsMargins(18, 24, 18, 20);
    sideLayout->setSpacing(12);

    QLabel *menuTitle = new QLabel("MENU PRINCIPAL");
    menuTitle->setObjectName("menuTitle");
    sideLayout->addWidget(menuTitle);
    sideLayout->addSpacing(8);

    m_accueil = new QPushButton(" Accueil");
    m_inscription = new QPushButton(" Inscription");
    m_eleves = new QPushButton(" Liste des eleves");

    m_accueil->setObjectName("activeMenu");
    m_inscription->setObjectName("menuButton");
    m_eleves->setObjectName("menuButton");

    for (QPushButton *button : {m_accueil, m_inscription, m_eleves})
    {
        button->setMinimumHeight(44);
        button->setCursor(Qt::PointingHandCursor);
        sideLayout->addWidget(button);
    }
    sideLayout->addStretch();

    QLabel *version = new QLabel("Gestion scolaire\nVersion 1.0");
    version->setObjectName("version");
    sideLayout->addWidget(version);

    //Les clics sont transmis à la fenêtre principale
    connect(m_accueil, &QPushButton::clicked, this, &SideBar::accueilClicked);
    connect(m_inscription, &QPushButton::clicked, this, &SideBar::inscriptionClicked);
    connect(m_eleves, &QPushButton::clicked, this, &SideBar::elevesClicked);
}

void SideBar::setMenuActif(Page page)
{
    QPushButton *boutonActif = m_accueil;
    if (page == Inscription) {
        boutonActif = m_inscription;
    } else if (page == Eleves) {
        boutonActif = m_eleves;
    }

    //Réinitialiser les trois boutons
    m_accueil->setObjectName("menuButton");
    m_inscription->setObjectName("menuButton");
    m_eleves->setObjectName("menuButton");

    boutonActif->setObjectName("activeMenu");

    //Actualiser le style QSS
    m_accueil->style()->unpolish(m_accueil);
    m_accueil->style()->polish(m_accueil);

    m_inscription->style()->unpolish(m_inscription);
    m_inscription->style()->polish(m_inscription);

    m_eleves->style()->unpolish(m_eleves);
    m_eleves->style()->polish(m_eleves);
}
