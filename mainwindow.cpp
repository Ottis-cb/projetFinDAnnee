
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
#include <QStackedWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QDate>
#include <QFormLayout>
#include <QLineEdit>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


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
    content->setObjectName("contentArea");

    //Layout occupant toute la zone principale
    QVBoxLayout *outerLayout = new QVBoxLayout(content);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    //gestionnaire de pages
    QStackedWidget *pages = new QStackedWidget(content);
    outerLayout->addWidget(pages);

    //Page d'acceuil
    QWidget *dashboardPage = new QWidget;
    dashboardPage->setObjectName("content");

    //Layout de l'acceuil
    QVBoxLayout *contentLayout = new QVBoxLayout(dashboardPage);
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
            "; font-size: 23px; font-weight: bold;"
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

    QPushButton *newStudent = new QPushButton("Preparer une inscription");
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

    //ajouter la page d'acceuil
    pages->addWidget(dashboardPage);




    //PAGE INSCRIPTION
    QWidget *inscriptionPage = new QWidget;
    inscriptionPage->setObjectName("content");

    QVBoxLayout *inscriptionLayout = new QVBoxLayout(inscriptionPage);
    inscriptionLayout->setContentsMargins(30, 24, 30, 24);
    inscriptionLayout->setSpacing(20);

    QLabel *formTitle = new QLabel("Inscription d'un eleve");
    formTitle->setObjectName("heading");

    QLabel *formSubtitle = new QLabel("Remplissez les informations de l'eleve.");
    formSubtitle->setObjectName("subtitle");

    inscriptionLayout->addWidget(formTitle);
    inscriptionLayout->addWidget(formSubtitle);

    //cadre contenant le formulaire
    QFrame *formCard = new QFrame;
    formCard->setObjectName("welcomeCard");

    QFormLayout *formLayout = new QFormLayout(formCard);
    formLayout->setContentsMargins(24, 24, 24, 24);
    formLayout->setHorizontalSpacing(20);
    formLayout->setVerticalSpacing(16);

    //champ nom
    QLineEdit *nomEdit = new QLineEdit;
    nomEdit->setPlaceholderText("Nom de l'eleve");

    //champ prenom
    QLineEdit *prenomEdit = new QLineEdit;
    prenomEdit->setPlaceholderText("Prenom de l'eleve");

    //Date de naissance
    QDateEdit *dateEdit = new QDateEdit;
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setDate(QDate::currentDate().addYears(-10));

    //sexe
    QComboBox *sexeCombo = new QComboBox;
    sexeCombo->addItems({"Selectionner",
                        "Masculin",
                        "Feminin"
                        });

    //Niveau
    QComboBox *niveauCombo = new QComboBox;
    niveauCombo->addItems({
    "Selectionner",
        "L1","L2","L3","M1","M2"
    });

    //Matricule
    QLineEdit *matriculeEdit = new QLineEdit;
    matriculeEdit->setPlaceholderText("Numero matricule");
    //Telephone
    QLineEdit *teleponeEdit = new QLineEdit;
    teleponeEdit->setPlaceholderText("Numero de telephone");

    //Adresse
    QLineEdit *adresseEdit = new QLineEdit;
    adresseEdit->setPlaceholderText("Adresse de l'eleve");

    //Placer les champs dans le formulaire
    formLayout->addRow("Nom *", nomEdit);
    formLayout->addRow("Prenom *", prenomEdit);
    formLayout->addRow("Matricule *", matriculeEdit);
    formLayout->addRow("Date de naissance", dateEdit);
    formLayout->addRow("sexe *", sexeCombo);
    formLayout->addRow("Niveau *", niveauCombo);
    formLayout->addRow("Telephone", teleponeEdit);
    formLayout->addRow("Adresse", adresseEdit);

    //ajouter le cadre a la page
    inscriptionLayout->addWidget(formCard);
    inscriptionLayout->addStretch();


    //Conteneur horizontal pour les boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout;




    //creation des trois boutons
    QPushButton *saveButton = new QPushButton("Enregistrer");
    QPushButton *resetButton = new QPushButton("Réinitialiser");
    QPushButton *cancelButton = new QPushButton("Annuler");

    //Attribution des noms pour le style QSS
    saveButton->setObjectName("saveButton");
    resetButton->setObjectName("resetButton");
    cancelButton->setObjectName("cancelButton");

    //Ajouter les boutons au layout
    buttonLayout->addStretch();
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);

    //Ajouter le layout des boutons à la page
    inscriptionLayout->addLayout(buttonLayout);

    //Faire fonctionner les boutons
    //Réinitialiser
    connect(resetButton, &QPushButton::clicked, this, [=](){
        nomEdit->clear();
        prenomEdit->clear();
        adresseEdit->clear();
        matriculeEdit->clear();
        dateEdit->setDate(QDate::currentDate().addYears(-10));

        sexeCombo->setCurrentIndex(0);
        niveauCombo->setCurrentIndex(0);
    });

    //Annuler
    connect(cancelButton, &QPushButton::clicked, this, [=](){
        pages->setCurrentWidget(dashboardPage);
    });

    //Enregistrer
    connect(saveButton, &QPushButton::clicked, this, [=](){
        //Récuperer les infos
        QString nom = nomEdit->text().trimmed();
        QString prenom = prenomEdit->text().trimmed();
        QString matricule = matriculeEdit->text().trimmed();
        QString dateNaissance = dateEdit->date().toString("dd/MM/yyyy");
        QString sexe = sexeCombo->currentText();
        QString niveau = niveauCombo->currentText();
        QString telephone = teleponeEdit->text().trimmed();
        QString adresse = adresseEdit->text().trimmed();

        //Verifier les champs obligatoire
        if (nom.isEmpty() || prenom.isEmpty() || matricule.isEmpty() || telephone.isEmpty() || sexeCombo->currentIndex() == 0 || niveauCombo->currentIndex() ==0 ){
            QMessageBox::warning(this, "formulaire incomplet","Veuillez remplir tous les champs obligatoires.");
            return;
        }

        //Ouvrir le fichier en mode ajout
        QFile fichier("eleves.txt");

        if(!fichier.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier.");
            return;
        }

        //Ecrire les infos dans le fichier
        QTextStream sortie(&fichier);

        sortie << nom << ";"
               << prenom << ";"
               << matricule << ";"
               << dateNaissance << ";"
               << sexe << ";"
               << telephone << ";"
               << adresse << "\n";

        //fermer le fichier
        fichier.close();

        //confirmer l'enregistrement
        QMessageBox::information(this, "succès", "L'élève a été enregistré avec succès.");
     });



    //Enregistrer la page d'inscription
    pages->addWidget(inscriptionPage);



    //Navigation entre les pages
    //cliquer sur Accueil
    connect(accueil, &QPushButton::clicked, this, [pages, dashboardPage](){
        pages->setCurrentWidget(dashboardPage);
    });

    //Cliquer sur inscription
    connect(inscription, &QPushButton::clicked, this, [pages, inscriptionPage](){
        pages->setCurrentWidget(inscriptionPage);
    });

    //Cliquer sur le bouton de la page d'acceuil
    connect(newStudent, &QPushButton::clicked, this, [pages, inscriptionPage](){
        pages->setCurrentWidget(inscriptionPage);
    });


    //Asseblage final
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(content, 1);







    //Style global QSS
    setStyleSheet(R"(

    QMainWindow {
        background-color: #17171a;
    }

    QWidget#content {
        background-color: #17171a;
    }

    QFrame#sidebar {
        background-color: #202025;
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
        letter-spacing: 1px;
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
        font-weight: bold;
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
    QFrame#WelcomeCard {
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

    QWidget#contentArea,
    QWidget#content {
        background-color: #17171a;
    }

    QFrame#statCard,
    QFrame#welcomeCard {
        background-color: #232329;
        border: 1px solid #○34343d;
        border-radius: 12px;
    }

    QLineEdit, QDateEdit, QComboBox {
        color: #f4f4f5;
        background-color: #1d1d23;
        border: 1px solid #41414b;
        border-radius: 8px;
        padding: 10px 12px;
        min-height: 22px;
        selection-background-color: #7665f5;
    }

    QLineEdit:focus, QDateEdit:focus, QComboBox:focus {
        border: 1px solid #897bff;
        background-color: #22222a;
    }

    QLineEdit:hover, QDateEdit:hover, QComboBox:hover{
        border-color: #6252d8;
    ;

    QComboBox QAbstractItemView {
        color: #f4f4f5;
        background-color: #232329;
        border: 1px solid #41414b;
        outline: none;
    }

    QFormLayout QLabel {
        color: #d4d4dc;
        font-size: 13px;
    }

    QPushButton {
        border-radius: 8px;
        padding: 10px 16px;
        font-size: 13px;
    }

    QPushButton#primaryButton{
        color: white;
        background-color: #7665f5;
        border: none;
        font-weight: bold;
    }
    QPushButton#primaryButton:hover{
        background-color: #897bff;
    }
    QPushButton#primaryButton:pressed{
        background-color: #6252d8;
    }

    QCalendarWidget QWidget{
        color: #f4f4f5;
        background-color: #232329;
    }
    QCalendarWidget QToolButton{
        color: white;
        background-color: #303039;
    }

    QPushButton#saveButton {
        color: white;
        background-color: #3978e8;
        border: none;
        border-radius: 9px;
        padding: 12px 24px;
        font-size: 14px;
        font-weight: 600;
        min-width: 120px;
    }
    QPushButton#saveButton:hover {
        background-color: #2865d4;
    }
    QPushButton#saveButton:pressed {
        background-color: #1e50b0;
    }


    QPushButton#resetButton {
        color: #e5e7eb;
        background-color: #30313a;
        border: 1px solid #41424d;
        border-radius: 9px;
        padding: 12px 20px;
        font-size: 14px;
        font-weight: 500;
        min-width: 120px;
    }
    QPushButton#resetButton:hover {
        background-color: #3c3d48;
        border-color: #626474;
    }
    QPushButton#resetButton:pressed {
        background-color: #272831;
    }

    )");

}

MainWindow::~MainWindow()
{
    delete ui;
}