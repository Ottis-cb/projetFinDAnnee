#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFont>
#include <QStyle>
#include <QStatusBar>


MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("GestionEleves");
    setMinimumSize(900, 600);
    resize(1200, 760);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);


    //fenetre principale
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);



    //Barre laterale
    QFrame *sidebar = new QFrame;
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(220);

    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(18, 24, 18, 20);
    sideLayout->setSpacing(12);

    QLabel *menuTitle = new QLabel("MENU PRINCIPAL");
    menuTitle->setObjectName("menuTitle");
    sideLayout->addWidget(menuTitle);
    sideLayout->addSpacing(8);

    QPushButton *accueil = new QPushButton(" Accueil");
    QPushButton *inscription = new QPushButton(" Inscription");
    QPushButton *eleves = new QPushButton(" Liste des eleves");

    accueil->setObjectName("activeMenu");
    inscription->setObjectName("menuButton");
    eleves->setObjectName("menuButton");

    for (QPushButton *button: {accueil, inscription, eleves})
    {
        button->setMinimumHeight(44);
        button->setCursor(Qt::PointingHandCursor);
        sideLayout->addWidget(button);
    }
    sideLayout->addStretch();

    QLabel *version = new QLabel("Gestion scolaire\nVersion 1.0");
    version->setObjectName("version");
    sideLayout->addWidget(version);


    //zone principale
    QWidget *content = new QWidget;
    content->setObjectName("content");

    QVBoxLayout *contentLayout = new QVBoxLayout;
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

    auto createCard = [](const QString&symbol, const QString&value, const QString&description, const QString&accent)->QFrame *
    {
        QFrame *card = new QFrame;
        card->setObjectName("statCard");
        card->setMinimumHeight(135);

        QVBoxLayout *layout= new QVBoxLayout(card);
        layout->setContentsMargins(18, 16, 18, 16);
        layout->setSpacing(8);

        QLabel *icon = new QLabel(symbol);
        icon->setStyleSheet(
            "color: " + accent +
            "; font-size: 23px; fontWeight: bold;"
            );

        QLabel *number = new QLabel(value);
        number->setObjectName("statNumber");
        QLabel *text = new QLabel(description);

        text->setObjectName("statDescription");

        layout->addWidget(icon);
        layout->addWidget(number);
        layout->addWidget(text);

        return card;
    };

    statsLayout->addWidget(createCard("02", "0", "Fiches enregistree", "#6EE7B7"), 0, 1);

    contentLayout->addLayout(statsLayout);



    //6. panneau de bienvenu
    QFrame *welcomeCard = new QFrame;

    welcomeCard->setObjectName("WelcomeCard");

    QVBoxLayout *welcomeLayout = new QVBoxLayout(welcomeCard);
    welcomeLayout->setContentsMargins(24, 22, 24, 22);
    welcomeLayout->setSpacing(12);

    QLabel *welcomeTitle = new QLabel("Bienvenu dans GestionEleves");
    welcomeTitle->setObjectName("sectionTitle");

    QLabel *welcomeText = new QLabel("Centraliser les informations des eleves, "
                                     "organiser les inscriptions et consultez"
                                     "les donnees en un seul endroit.");
    welcomeText->setObjectName("subtitle");
    welcomeText->setWordWrap(true);

    QPushButton *newStudent = new QPushButton("+ Preparer une inscription");
    newStudent->setObjectName("primaryButton");
    newStudent->setMinimumHeight(44);
    newStudent->setCursor(Qt::PointingHandCursor);

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

    //Asseblage final
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(content, 1);

    //Action temporaire: confirmation visuelle
    connect(newStudent, &QPushButton::clicked , this, [this](){
        statusBar()->showMessage("Le formulaire d'inscription sera ahoute"
                                 "a la prochaine etape.", 4000);
    });


    //Style global QSS
    setStyleSheet(R"(

    QMainWindow {
        background-color: #17171a;
    }

    QWidget#content {
        background-color: #17171a;
    }

    QFrame#sidebar {
        backgroung-color: #202025;
        border-right: 1px solid #34343b;
    }

    QLabel#logo {
        color: #ffffff;
        font-size: 20px;
        font-weight: bold;
        padding-bottom: 22px;
    }

    QLabel#menuTitle {
        color: #858591;
        font-size: 10px;
        font-weight: bold;
        letter-spacing: 1px
    }

    QPushButton#menuButton,
    QPushButton#activeMenu {
        text-align: left;
        padding-left: 12px;
        border: none;
        border-radius: 8px;
        font-size: 13px;
    }

    QPushButton#menuButton {
        color: #b8b8c2;
        background-color: transparent;
    }

    QPushButton#menuButton:hover {
        color: #ffffff;
        background-color: #2d2d35;
    }

    QPushButton#activeMenu {
        color: #ffffff;
        background-color: #39324f;
        font-weight: bold;
    }

    QLabel#version {
        color: #777783;
        font-size: 11px;
        padding-top: 12px
    }

    QLabel#heading {
        color: #ffffff;
        font-size: 27px;
        font-weight: bold
    }

    QLabel#subtitle {
        color: #a1a1aa;
        font-size: 13px;
    }

    QPushButton#profileButton {
        color: #e4e4e7;
        background-color: #25252c;
        border: 1px solid #3a3a43;
        border-radius: 8px;
        padding: 0 14px;
    }

    QFrame#statCard,
    QFrame#welcomeCard {
        background-color: #232329;
        border: 1px solid #34343d;
        border-radius: 12px;
    }

    QLabel#statNumber {
    color: #ffffff;
    font-size: 27px;
    font-weight: bold;
    }

    QLabel#statDescription {
    color: #a1a1aa;
    font-size: 12px;
    }

    QLabel#sectionTitle {
    color: #ffffff;
    font-size: 19px;
    font-weight: bold;
    }

    QPushButton#primaryButton {
        color: #ffffff;
        background-color: #7665f5;
        border: none;
        border-radius: 8px;
        padding: 0 18px;
        font-size: 13px;
        font-weight: bold;
    }

    QPushButton#primaryButton:hover {
        background-color: #897bff;
    }

    QPushButton#primaryButton:pressed {
        background-color: #6252d8;
    }

    QLabel#footer {
        color: #777783;
        font-size: 11px;
        padding-top: 8px;
    }

    QStatusBar {
        color: #c4c4cc;
        background-color: #202025;
    }
    )");

}

MainWindow::~MainWindow()
{
    delete ui;
}