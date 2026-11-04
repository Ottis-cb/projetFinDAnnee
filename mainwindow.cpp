
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
#include <QTableWidget>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QStringList>


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

    //afficheurs de nombres

    QLabel *totalNumber = nullptr;
    QLabel *garçonsNumber = nullptr;
    QLabel *fillesNumber = nullptr;

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
    statsLayout->addWidget(createCard("TOTAL", "0", "Total des élèves", "#a78bfa", totalNumber), 0, 0);

    statsLayout->addWidget(createCard(QString(QChar(0x2642)), "0", "Garçons", "#60a5fa", garçonsNumber), 0, 1);

    statsLayout->addWidget(createCard(QString(QChar(0x2640)), "0", "Filles", "#f472b6", fillesNumber), 0, 2);

    contentLayout->addLayout(statsLayout);


    //Fonction pour calculer les statistiques

    auto actualiserStatistiques = [=]()
    {
        int total = 0;
        int garcons = 0;
        int filles = 0;

        //ouvrir le fichier des élèves
        QFile fichier("eleves.txt");

        if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream entree(&fichier);

            //lire chaque élève
            while (!entree.atEnd())
            {
                QString ligne = entree.readLine();
                QStringList informations = ligne.split(";");

                //verifier que la ligne contient 8 informations
                if (informations.size() != 8){
                    continue;
                }
                total++;

                //Le sexe se trouve à la position 4
                QString sexe = informations.at(4).trimmed();
                if (sexe == "Masculin"){
                    garcons++;
                }
                else if (sexe == "Feminin" || sexe == "Féminin") {
                    filles++;
                }

            }
            fichier.close();
        }
        //Actualiser les trois nombres affichés

        totalNumber->setText(QString::number(total));
        garçonsNumber->setText(QString::number(garcons));
        fillesNumber->setText(QString::number(filles));
    };

    //actualiser les statistiques au démarrage
    actualiserStatistiques();



    //6. panneau de bienvenu
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
    inscriptionLayout->addStretch();
    inscriptionLayout->addLayout(buttonLayout);

    //Faire fonctionner les boutons
    //Réinitialiser
    connect(resetButton, &QPushButton::clicked, this, [=](){
        nomEdit->clear();
        prenomEdit->clear();
        adresseEdit->clear();
        matriculeEdit->clear();
        teleponeEdit->clear();
        dateEdit->setDate(QDate::currentDate().addYears(-10));

        sexeCombo->setCurrentIndex(0);
        niveauCombo->setCurrentIndex(0);
        actualiserStatistiques();
    });

    //Annuler
    connect(cancelButton, &QPushButton::clicked, this, [=](){
        pages->setCurrentWidget(dashboardPage);
        actualiserStatistiques();
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

        sortie << matricule << ";"
                << nom << ";"
               << prenom << ";"
               << dateNaissance << ";"
               << sexe << ";"
                << niveau << ";"
               << telephone << ";"
               << adresse << "\n";

        //fermer le fichier
        fichier.close();

        //confirmer l'enregistrement
        QMessageBox::information(this, "succès", "L'élève a été enregistré avec succès.");
        actualiserStatistiques();
     });



    //Enregistrer la page d'inscription
    pages->addWidget(inscriptionPage);


    //PAGE LISTE DES ELEVES


    QWidget *elevesPage = new QWidget;
    elevesPage->setObjectName("content");


    //layout principal de la page
    QVBoxLayout *elevesLayout = new QVBoxLayout(elevesPage);
    elevesLayout->setContentsMargins(30, 24, 30, 24);
    elevesLayout->setSpacing(20);

    //Titre de la page
    QLabel *elevesTitle = new QLabel("Liste des eleves");
    elevesTitle->setObjectName("heading");
    QLabel *elevesSubtitle = new QLabel("Consultez et gerez les informations des eleves.");
    elevesSubtitle->setObjectName("subtitle");

    elevesLayout->addWidget(elevesTitle);
    elevesLayout->addWidget(elevesSubtitle);

    //cadre contenant le tableau
    QFrame *tableCard = new QFrame;
    tableCard->setObjectName("welcomeCard");

    QVBoxLayout *tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(10, 20, 10, 20);

    //Creation du tableau
    QTableWidget *tableEleves = new QTableWidget;

    QLineEdit *searchEdit = new QLineEdit;
    searchEdit->setPlaceholderText("Rechercherun élève (nom, prénom, matricule...)");
    searchEdit->setObjectName("searchEdit");
    searchEdit->setMinimumHeight(42);
    searchEdit->setClearButtonEnabled(true);

    //Nombre de colonnes
    tableEleves->setColumnCount(8);

    //Noms des colonnes
    tableEleves->setHorizontalHeaderLabels({
        "Matricule",
        "Nom",
        "Prenom",
        "Date de naissance",
        "Sexe",
        "Niveau",
        "Telephone",
        "Adresse"
    });

    //Adapter les colonnes a la largeur disponnible
    tableEleves->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //Hauteur des lignes
    tableEleves->verticalHeader()->setDefaultSectionSize(42);

    //Masquer les numeros des lignes
    tableEleves->verticalHeader()->setVisible(false);

    //Selectionner une ligne entière
    tableEleves->setSelectionBehavior(QAbstractItemView::SelectRows);

    //Une seule ligne selectionnable a la fois
    tableEleves->setSelectionMode(QAbstractItemView::SingleSelection);

    //Interdire la modification directe des cellules
    tableEleves->setEditTriggers(QAbstractItemView::NoEditTriggers);


    tableEleves->setWordWrap(true);
    tableEleves->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    //Conteneur des boutons d'action
    QHBoxLayout *actionsLayout = new QHBoxLayout;

    //Bouton actualiser
    QPushButton *refreshButton = new QPushButton("Actualiser");
    refreshButton->setObjectName("resetButton");

    //Bouton supprimer
    QPushButton *deleteButton = new QPushButton("Supprimer");
    deleteButton->setObjectName("deleteButton");

    //curseur
    refreshButton->setCursor(Qt::PointingHandCursor);
    deleteButton->setCursor(Qt::PointingHandCursor);

    //Placer les boutons à droite
    actionsLayout->addStretch();
    actionsLayout->addWidget(refreshButton);
    actionsLayout->addWidget(deleteButton);

    //Ajouter les boutons au-dessus du tableau
    tableLayout->addLayout(actionsLayout);

    //Ajouter la barre de recherche
    tableLayout->addWidget(searchEdit);

    //Recherche dynamique des élèves

    connect(searchEdit, &QLineEdit::textChanged, this, [=](const QString &recherche){
        QString texte = recherche.trimmed();

        for (int ligne=0 ; ligne < tableEleves->rowCount(); ligne++)
        {
            bool correspondance = false;

            //parcourir les 8 colonnes
            for (int colonne = 0; colonne < 8; colonne++)
            {
                QTableWidgetItem *item = tableEleves->item(ligne, colonne);

                if(item && item->text().contains(texte, Qt::CaseInsensitive))
                {
                    correspondance = true;
                    break;
                }
            }

            // Afficher ou masquer la ligne

            tableEleves->setRowHidden(ligne, ! correspondance);
        }
    });






    //Ajouter le tableau au cadre
    tableLayout->addWidget(tableEleves);


    //Fonction pour charger les eleves depuis le fichier
    auto chargerEleves = [tableEleves, actualiserStatistiques]()
    {
        //Vider le tableau avant de le remplir
        tableEleves->setRowCount(0);

        //Ouvrir le fichier en lecture
        QFile fichier("eleves.txt");
        if(!fichier.open(QIODevice::ReadOnly | QIODevice::Text)){
            return;
        }

        //Creer un flux de lecture
        QTextStream entree(&fichier);

        //Lire les fichiers ligne par ligne
        while(!entree.atEnd()){
            QString ligne = entree.readLine();

            //Decouper la ligne avec le point-virgule
            QStringList informations = ligne.split(';');

            //Verifier que la ligne contient 8 informations
            if (informations.size() != 8){
                continue;
            }

            //Ajouter une nouvelle ligne au tableau
            int nouvelleLigne = tableEleves->rowCount();
            tableEleves->insertRow(nouvelleLigne);

            //Remplir les 8 colonnes
            for (int colonne =0; colonne<8; colonne++){
                QTableWidgetItem*cellule= new QTableWidgetItem(informations.at(colonne).trimmed());
                tableEleves->setItem(nouvelleLigne, colonne, cellule);
            }
        }

        //Fermer le fichier
        fichier.close();

        actualiserStatistiques();
    };



    //Faire fonctionner les buttons du tableau
    //refreshButton
    connect(refreshButton, &QPushButton::clicked, this, [chargerEleves](){
        chargerEleves();
    });
    //deleteButton
    connect(deleteButton, &QPushButton::clicked, this, [=](){
        //Verifier si une ligne est séléctionnée
        int ligne = tableEleves->currentRow();

        if (ligne < 0){
            QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un éléve.");
            return;
        }

        //Demander confirmation
        QMessageBox::StandardButton reponse = QMessageBox::question(this, "Confirmation", "Voulez vous vraiment supprimer cet élève.",
        QMessageBox::Yes | QMessageBox::No);

        if (reponse != QMessageBox::Yes){
            return;
        }

        //Recuperer le matricule de l'élève
        QString matricule = tableEleves->item(ligne, 0)->text();

        //Lire tous les élèves sauf celui à supprimer
        QFile fichier("eleves.txt");

        if (!fichier.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::critical(this, "Erreur", "Impossible de lire le fichier.");
            return;
        }

        QStringList lignes ;
        QTextStream entree(&fichier);

        while (!entree.atEnd()){
            QString ligneTexte = entree.readLine();
            QStringList infos = ligneTexte.split(';');

            if(infos.size() == 8 && infos.at(0).trimmed() != matricule){
                lignes.append(ligneTexte);
            }
        }

        fichier.close();

        //Réecrire le fichier sans l'élève supprimé
        if(!fichier.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
            QMessageBox::critical(this, "Erreur", "Impossible de modifier le fichier.");
            return;
        }

        QTextStream sortie(&fichier);
        for (const QString &ligneTexte : lignes){
            sortie << ligneTexte << "\n";
        }
        fichier.close();

        //Actualiser le tableau
        chargerEleves();

        QMessageBox::information(this, "Suppression réussi", "L'élève a été supprimé.");
        actualiserStatistiques();
    });




    //Ajouter le cadre a la page
    elevesLayout->addWidget(tableCard);

    //Ajouter la page au gestionnaire de pages
    pages->addWidget(elevesPage);







    //Navigation entre les pages
    //cliquer sur Accueil
    connect(accueil, &QPushButton::clicked, this, [pages, dashboardPage](){
        pages->setCurrentWidget(dashboardPage);
    });

    //Cliquer sur inscription
    connect(inscription, &QPushButton::clicked, this, [pages, inscriptionPage](){
        pages->setCurrentWidget(inscriptionPage);
    });
    //Cliquer sur liste des eleves
    connect(eleves, &QPushButton::clicked, this, [pages, elevesPage, chargerEleves](){
        chargerEleves();
        pages->setCurrentWidget(elevesPage);
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

    QFrame#dashboardStatCard {
        background-color: #23232b;
        border: 1px solid #343440;
        border-radius: 14px;
    }
    QFrame#dashboardStatCard:hover {
        background-color: #292936;
        border: 1px solid #8060d8;
    }

    QLabel#dashboardStatNumber {
        color: #ffffff;
        font-size: 30px;
        font-weight: bold;
    }

    QLabel#dashboardStatDescription {
        color: #a1a1aa;
        font-size: 13px;
    }
    QLabel#dashboardStatIcon {
        font-size: 24px;
    }

    QFrame#dashboardWelcomeCard {
        background-color: #24242f;
        border: 1px solid #403951;
        border-radius: 16px;
    }

    QLabel#dashboardBadge {
        color: #a78bfa;
        font-size: 10px;
        font-weight: bold;
        letter-spacing: 2px;
    }

    QLabel#dashboardWelcomeTitle {
        color: #ffffff;
        font-size: 23px;
        font-weight: bold;
    }

    QLabel#dashboardWelcomeText {
        color: #b7b7c2;
        font-size: 13px;
    }

    QPushButton#primaryButton {
        color: #ffffff;
        background-color: #7655cf;
        border: none;
        border-radius: 9px;
        padding: 12px 20px;
        font-size: 13px;
        font-weight: bold;
    }

    QPushButton#primaryButton:hover {
        background-color: #896be0;
    }

    QPushButton#primaryButton:pressed {
        background-color: #6243b7;
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








    QLabel {
        color: #d1d5db;
        font-size: 13px;
        font-weight: 600;
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
    }

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

    QPushButton#cancelButton {
        color: #a1a1aa;
        background-color: transparent;
        border: 1px solid #41424d;
        border-radius: 9px;
        padding: 12px 20px;
        font-size: 14px;
        font-weight: 500;
        min-width: 100px;
    }
    QPushButton#cancelButton:hover {
        color: white;
        background-color: #30313a;
        border-color: #626474;
    }
    QPushButton#resetButton:pressed {
        background-color: #272831;
    }

    QLineEdit#searchEdit {
        color: #ffffff;
        background-color: #202025;
        border: 1px solid #41414b;
        border-radius: 10px;
        padding: 10px 14px;
        font-size: 13px;
    }
    QLineEdit#searchEdit:focus {
        background-color: #262630;
        border: 1px solid #a78bfa;
    }
    QLineEdit#searchEdit::placeholder {
        color: #858591;
    }






    QTableWidget {
        color: #e5e7eb;
        background-color: #202025;
        alternate-background-color: #262630;
        selection-background-color: #514078;
        selection-color: #ffffff;
        border: 1px solid #34343b;
        border-radius: 10px;
        gridline-color: #383842;
        font-size: 12px;

        outline: none;
    }
    QHeaderView::section {
        color: #a78bfa;
        background-color: #292832;
        padding: 13px 8px;

        border: none;
        border-bottom: 2px solid #a78bfa;

        font-size: 12px;
        font-weight: bold;
    }
    QTableWidget::item {
        padding: 10px;
        border-bottom: 1px solid #34343b;
    }
    QTableWidget::item:hover {
        background-color: #302b3d;
    }
    QTableWidget::item:selected {
        background-color: #514078;
        color: #ffffff;
    }
    QHeaderView::section:vertical {
        background-color: #202025;
        border: none;
    }

    QPushButton#deleteButton {
        color: white;
        background-color: #dc4545;
        border: none;
        border-radius: 9px;
        padding: 12px 20px;
        font-size: 14px;
        font-weight: 600;
        min-width: 120px;
    }
    QPushButton#deleteButton:hover {
        background-color: #ef5555;
    }
    QPushButton#deleteButton:pressed {
        background-color: #b83232;
    }
    )");

}

MainWindow::~MainWindow()
{
    delete ui;
}