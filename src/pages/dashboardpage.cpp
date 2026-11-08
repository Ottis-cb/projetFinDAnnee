#include "dashboardpage.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
{
    //Page d'acceuil
    setObjectName("content");
    setAttribute(Qt::WA_StyledBackground, true);

    //Layout de l'acceuil
    QVBoxLayout *contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(30, 24, 30, 24);
    contentLayout->setSpacing(22);

    //en-tete
    QHBoxLayout *headerLayout = new QHBoxLayout;
    QVBoxLayout *headingLayout = new QVBoxLayout;

    QLabel *heading = new QLabel("Tableau de bord");
    heading->setObjectName("heading");

    QLabel *subtitle = new QLabel("Bienvenu dans votre espace de gestion des eleves.");
    subtitle->setObjectName("subtitle");

    headingLayout->addWidget(heading);
    headingLayout->addWidget(subtitle);

    QPushButton *profileButton = new QPushButton("Administration");
    profileButton->setObjectName("profileButton");
    profileButton->setMinimumHeight(40);

    headerLayout->addLayout(headingLayout);
    headerLayout->addWidget(profileButton);
    contentLayout->addLayout(headerLayout);

    //Carte statistique
    QGridLayout *statsLayout = new QGridLayout;
    statsLayout->setSpacing(16);

    //Fonction qui créer une carte statistique

    auto createCard = [](const QString &symbol, const QString &value, const QString &description, const QString &accent, QLabel *&numberLabel)-> QFrame *{
        QFrame *card = new QFrame;

        card->setObjectName("dashboardStatCard");
        card->setMinimumHeight(145);

        QVBoxLayout *layout = new QVBoxLayout(card);
        layout->setContentsMargins(20, 18, 20, 18);
        layout->setSpacing(9);

        QLabel *icon = new QLabel(symbol);
        icon->setObjectName("dashboardStatIcon");
        icon->setStyleSheet("color: " + accent + "; font-size: 25px; font-weight: bold;");


        //Création du labels qui afficherons les nombres
        numberLabel = new QLabel(value);
        numberLabel->setObjectName("dashboardStatNumber");

        QLabel *text = new QLabel(description);
        text->setObjectName("dashboardStatDescription");

        layout->addWidget(icon);
        layout->addWidget(numberLabel);
        layout->addWidget(text);
        layout->addStretch();

        return card;
    };

    //Création des trois cartes
    statsLayout->addWidget(createCard("TOTAL", "0", "Total des élèves", "#38bdf8", m_totalNumber), 0, 0);

    statsLayout->addWidget(createCard(QString(QChar(0x2642)), "0", "Garçons", "#60a5fa", m_garconsNumber), 0, 1);

    statsLayout->addWidget(createCard(QString(QChar(0x2640)), "0", "Filles", "#f472b6", m_fillesNumber), 0, 2);

    contentLayout->addLayout(statsLayout);

    //actualiser les statistiques au démarrage
    actualiserStatistiques();


    // panneau de bienvenu
    QFrame *welcomeCard = new QFrame;

    welcomeCard->setObjectName("dashboardWelcomeCard");

    QVBoxLayout *welcomeLayout = new QVBoxLayout(welcomeCard);
    welcomeLayout->setContentsMargins(28, 26, 28, 26);
    welcomeLayout->setSpacing(14);

    //Petit texte introductif
    QLabel *welcomeBadge = new QLabel("ESPACE D'ADMINISTRATION");
    welcomeBadge->setObjectName("dashboardBadge");

    //Grand titre
    QLabel *welcomeTitle = new QLabel("Bienvenu dans GestionEleves !");
    welcomeTitle->setObjectName("dashboardWelcomeTitle");

    //description
    QLabel *welcomeText = new QLabel("Centraliser les informations des élèves, "
                                     "organiser les inscriptions et consultez"
                                     "vos données scolaires depuis un seul endroit.");
    welcomeText->setObjectName("dashboardWelcomeText");
    welcomeText->setWordWrap(true);

    //Bouton d'inscription
    QPushButton *newStudent = new QPushButton(" Preparer une inscription");
    newStudent->setObjectName("primaryButton");
    newStudent->setMinimumHeight(46);
    newStudent->setCursor(Qt::PointingHandCursor);

    //Organisation du panneau
    welcomeLayout->addWidget(welcomeBadge);
    welcomeLayout->addWidget(welcomeTitle);
    welcomeLayout->addWidget(welcomeText);
    welcomeLayout->addSpacing(8);
    welcomeLayout->addWidget(newStudent, 0, Qt::AlignLeft);

    contentLayout->addWidget(welcomeCard);


    //Espace disponible pour les prochains modules
    contentLayout->addStretch();
    QLabel *footer = new QLabel("GestionEleves | Application de gestion scolaire");
    footer->setObjectName("footer");
    contentLayout->addWidget(footer);

    //Cliquer sur le bouton de la page d'acceuil
    connect(newStudent, &QPushButton::clicked, this, &DashboardPage::inscriptionDemandee);
}

void DashboardPage::actualiserStatistiques()
{
    int total = 0;
    int garcons = 0;
    int filles = 0;

    //lire le fichier des élèves
    QList<QStringList> eleves;

    if (m_repo.lireTous(eleves))
    {
        //lire chaque élève
        for (const QStringList &informations : eleves)
        {
            total++;

            //Le sexe se trouve à la position 4
            QString sexe = informations.at(EleveRepository::Sexe).trimmed();
            if (sexe == "Masculin"){
                garcons++;
            }
            else if (sexe == "Feminin" || sexe == "Féminin") {
                filles++;
            }
        }
    }

    //Actualiser les trois nombres affichés

    m_totalNumber->setText(QString::number(total));
    m_garconsNumber->setText(QString::number(garcons));
    m_fillesNumber->setText(QString::number(filles));
}
