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



    //Fonction pour gérer le bouton actif
    auto changerMenuActif = [=](QPushButton *boutonActif)
    {
        //Réinitialiser les trois boutons
        accueil->setObjectName("menuButton");
        inscription->setObjectName("menuButton");
        eleves->setObjectName("menuButton");

        boutonActif->setObjectName("activeMenu");

        //Actualiser le style QSS
        accueil->style()->unpolish(accueil);
        accueil->style()->polish(accueil);

        inscription->style()->unpolish(inscription);
        inscription->style()->polish(inscription);

        eleves->style()->unpolish(eleves);
        eleves->style()->polish(eleves);
    };


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
    statsLayout->addWidget(createCard("TOTAL", "0", "Total des élèves", "#38bdf8", totalNumber), 0, 0);

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
        changerMenuActif(accueil);
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



// VALIDATION DU MATRICULE

// Refuser les caractères qui pourraient casser le fichier
if (matricule.contains(';') ||
    matricule.contains('\n') ||
    matricule.contains('\r')) {

    QMessageBox::warning(this, "Matricule invalide",
                         "Le matricule contient des caractères interdits.");
    return;
}


// VALIDATION DU NOM ET PRENOM

// Accepter les lettres, accents, espaces,
// apostrophes et tirets.
QRegularExpression expressionNom(
    QStringLiteral("^[\\p{L}][\\p{L} '\\-]*$")
);

if (!expressionNom.match(nom).hasMatch()) {

    QMessageBox::warning(this, "Nom invalide",
                         "Le nom contient des caractères invalides.");
    return;
}

if (!expressionNom.match(prenom).hasMatch()) {

    QMessageBox::warning(this, "Prénom invalide",
                         "Le prénom contient des caractères invalides.");
    return;
}


// VALIDATION DE LA DATE DE NAISSANCE


if (dateEdit->date() > QDate::currentDate()) {

    QMessageBox::warning(this, "Date invalide",
                         "La date de naissance ne peut pas être dans le futur.");
    return;
}


// VALIDATION DU TELEPHONE MALAGASY

// Supprimer les espaces, tirets et parenthèses
QString telephoneNettoye = telephone;
telephoneNettoye.remove(' ');
telephoneNettoye.remove('-');
telephoneNettoye.remove('(');
telephoneNettoye.remove(')');

// Vérifier le format national ou international
QRegularExpression expressionTelephone(
    "^(03[2348][0-9]{7}|\\+2613[2348][0-9]{7})$"
);

if (!expressionTelephone.match(telephoneNettoye).hasMatch()) {

    QMessageBox::warning(this, "Téléphone invalide",
                         "Veuillez saisir un numéro malgache valide.\n"
                         "Exemple : 0380615517 ou +261380615517.");
    return;
}


// VERIFICATION DES DOUBLONS
QFile fichierLecture("eleves.txt");

if (fichierLecture.exists()) {

    if (!fichierLecture.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QMessageBox::critical(this, "Erreur",
                              "Impossible de vérifier les inscriptions existantes.");
        return;
    }

    QTextStream entree(&fichierLecture);

    while (!entree.atEnd()) {

        QString ligne = entree.readLine();
        QStringList informations = ligne.split(';');

        if (informations.size() != 8)
            continue;

        QString ancienMatricule = informations.at(0).trimmed();
        QString ancienNom = informations.at(1).trimmed();
        QString ancienPrenom = informations.at(2).trimmed();
        QString ancienneDate = informations.at(3).trimmed();

        // Comparaison du matricule sans tenir compte
        // des majuscules et minuscules.
        if (ancienMatricule.compare(
                matricule, Qt::CaseInsensitive) == 0) {

            fichierLecture.close();

            QMessageBox::warning(this, "Matricule déjà utilisé",
                                 "Ce numéro matricule existe déjà.");

            return;
        }

        // Comparaison de l'identité
        if (ancienNom.simplified().toCaseFolded()
                == nom.simplified().toCaseFolded() &&
            ancienPrenom.simplified().toCaseFolded()
                == prenom.simplified().toCaseFolded() &&
            ancienneDate == dateNaissance) {

            fichierLecture.close();

            QMessageBox::warning(this, "Élève déjà inscrit",
                                 "Un élève avec le même nom, prénom "
                                 "et date de naissance existe déjà.");

            return;
        }
    }

    fichierLecture.close();
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
    connect(accueil, &QPushButton::clicked, this, [=](){
        pages->setCurrentWidget(dashboardPage);
        changerMenuActif(accueil);
    });

    //Cliquer sur inscription
    connect(inscription, &QPushButton::clicked, this, [=](){
        pages->setCurrentWidget(inscriptionPage);
        changerMenuActif(inscription);
    });
    //Cliquer sur liste des eleves
    connect(eleves, &QPushButton::clicked, this, [=](){
        chargerEleves();
        pages->setCurrentWidget(elevesPage);
        changerMenuActif(eleves);
    });

    //Cliquer sur le bouton de la page d'acceuil
    connect(newStudent, &QPushButton::clicked, this, [=](){
        pages->setCurrentWidget(inscriptionPage);
        changerMenuActif(inscription);
    });


    //Asseblage final
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(content, 1);




    //Style global QSS
    setStyleSheet(R"(

/* FENETRE PRINCIPALE */

QMainWindow {
    background-color: #171B22;
}

QWidget#content,
QWidget#contentArea {
    background-color: #171B22;
}




/*  BARRE LATERALE  */

QFrame#sidebar {
    background-color: #202833;
    border-right: 1px solid #344256;
}

QLabel#logo {
    color: #F8FAFC;
    font-size: 21px;
    font-weight: bold;
    padding-bottom: 22px;
}

QLabel#menuTitle {
    color: #64748B;
    font-size: 10px;
    font-weight: bold;
    letter-spacing: 1px;
}

QLabel#version {
    color: #64748B;
    font-size: 11px;
    padding-top: 10px;
}




/* NAVIGATION */

QPushButton#menuButton {
    background-color: transparent;
    color: #94A3B8;
    border: none;
    border-radius: 9px;
    padding: 12px 15px;
    text-align: left;
    font-size: 14px;
    font-weight: 600;
}

QPushButton#menuButton:hover {
    background-color: #283547;
    color: #FFFFFF;
}

QPushButton#activeMenu {
    background-color: #263B56;
    color: #60A5FA;
    border: none;
    border-left: 4px solid #3B82F6;
    border-radius: 9px;
    padding: 12px 15px;
    text-align: left;
    font-size: 14px;
    font-weight: bold;
}




/* TITRES ET TEXTES */

QLabel#heading {
    color: #F1F5F9;
    font-size: 27px;
    font-weight: bold;
}

QLabel#subtitle {
    color: #94A3B8;
    font-size: 13px;
}

QLabel#footer {
    color: #64748B;
    font-size: 11px;
}

QLabel#sectionTitle {
    color: #F1F5F9;
    font-size: 22px;
    font-weight: bold;
}




/* CARTES DU TABLEAU DE BORD */

QFrame#dashboardStatCard {
    background-color: #202833;
    border: 1px solid #344256;
    border-radius: 14px;
}

QFrame#dashboardStatCard:hover {
    border: 1px solid #3B82F6;
    background-color: #243142;
}

QLabel#dashboardStatIcon {
    font-size: 25px;
    font-weight: bold;
}

QLabel#dashboardStatNumber {
    color: #F8FAFC;
    font-size: 29px;
    font-weight: bold;
}

QLabel#dashboardStatDescription {
    color: #94A3B8;
    font-size: 13px;
}




/*  PANNEAU DE BIENVENUE */

QFrame#dashboardWelcomeCard,
QFrame#welcomeCard {
    background-color: #202833;
    border: 1px solid #344256;
    border-radius: 14px;
}

QLabel#dashboardBadge {
    color: #38BDF8;
    font-size: 11px;
    font-weight: bold;
    letter-spacing: 1px;
}

QLabel#dashboardWelcomeTitle {
    color: #F8FAFC;
    font-size: 24px;
    font-weight: bold;
}

QLabel#dashboardWelcomeText {
    color: #94A3B8;
    font-size: 14px;
}




/* BOUTONS PRINCIPAUX */

QPushButton#primaryButton,
QPushButton#saveButton {
    background-color: #3B82F6;
    color: #FFFFFF;
    border: none;
    border-radius: 9px;
    padding: 12px 22px;
    font-size: 14px;
    font-weight: bold;
}

QPushButton#primaryButton:hover,
QPushButton#saveButton:hover {
    background-color: #2563EB;
}

QPushButton#primaryButton:pressed,
QPushButton#saveButton:pressed {
    background-color: #1D4ED8;
}




/* BOUTON ADMINISTRATION */

QPushButton#profileButton {
    background-color: #283547;
    color: #CBD5E1;
    border: 1px solid #3B4B60;
    border-radius: 9px;
    padding: 9px 16px;
    font-size: 13px;
    font-weight: 600;
}

QPushButton#profileButton:hover {
    background-color: #34465D;
    color: #FFFFFF;
    border: 1px solid #3B82F6;
}




/* BOUTONS SECONDAIRES */

QPushButton#resetButton,
QPushButton#cancelButton {
    background-color: #303B4A;
    color: #CBD5E1;
    border: 1px solid #405066;
    border-radius: 9px;
    padding: 11px 19px;
    font-size: 13px;
    font-weight: 600;
}

QPushButton#resetButton:hover,
QPushButton#cancelButton:hover {
    background-color: #3A485A;
    color: #FFFFFF;
}

QPushButton#resetButton:pressed,
QPushButton#cancelButton:pressed {
    background-color: #253143;
}




/* BOUTON SUPPRIMER */

QPushButton#deleteButton {
    background-color: #C83F4B;
    color: #FFFFFF;
    border: none;
    border-radius: 9px;
    padding: 11px 20px;
    font-size: 13px;
    font-weight: bold;
}

QPushButton#deleteButton:hover {
    background-color: #E04B57;
}

QPushButton#deleteButton:pressed {
    background-color: #A92F3A;
}




/* CHAMPS DU FORMULAIRE  */

QLineEdit,
QDateEdit,
QComboBox {
    background-color: #171F2B;
    color: #E2E8F0;
    border: 1px solid #3B485A;
    border-radius: 8px;
    padding: 10px 12px;
    font-size: 13px;
    selection-background-color: #3B82F6;
    selection-color: #FFFFFF;
}

QLineEdit:hover,
QDateEdit:hover,
QComboBox:hover {
    border: 1px solid #64748B;
}

QLineEdit:focus,
QDateEdit:focus,
QComboBox:focus {
    border: 2px solid #3B82F6;
    background-color: #202B3A;
}

QLineEdit:disabled,
QDateEdit:disabled,
QComboBox:disabled {
    color: #64748B;
    background-color: #202833;
}




/*  ETIQUETTES DU FORMULAIRE */

QLabel#formLabel {
    color: #60A5FA;
    font-size: 13px;
    font-weight: bold;
    padding: 5px;
}
QLabel {
    color: #CBD5E1;
    background-color: transparent;
}

QLabel#titleLabel {
    color: #F8FAFC;
    font-weight: bold;
}

QLabel#subtitleLabel {
    color: #94A3B8;
}




/* LISTE DEROULANTE */

QComboBox::drop-down {
    border: none;
    width: 28px;
}

QComboBox QAbstractItemView {
    background-color: #202833;
    color: #E2E8F0;
    border: 1px solid #3B82F6;
    selection-background-color: #263B56;
    selection-color: #60A5FA;
    padding: 5px;
}




/* CALENDRIER */

QCalendarWidget QWidget {
    background-color: #202833;
    color: #E2E8F0;
}

QCalendarWidget QToolButton {
    background-color: #283547;
    color: #FFFFFF;
    border-radius: 5px;
    padding: 5px;
}

QCalendarWidget QToolButton:hover {
    background-color: #3B82F6;
}

QCalendarWidget QAbstractItemView {
    background-color: #171F2B;
    color: #E2E8F0;
    selection-background-color: #3B82F6;
    selection-color: #FFFFFF;
}




/* BARRE DE RECHERCHE */

QLineEdit#searchEdit {
    background-color: #202833;
    color: #F1F5F9;
    border: 1px solid #3B485A;
    border-radius: 11px;
    padding: 12px 15px;
    font-size: 13px;
    font-weight: 500;
    selection-background-color: #3B82F6;
    selection-color: #FFFFFF;
}

QLineEdit#searchEdit:hover {
    border: 1px solid #64748B;
    background-color: #243142;
}

QLineEdit#searchEdit:focus {
    border: 2px solid #3B82F6;
    background-color: #202B3A;
}

QLineEdit#searchEdit QToolButton {
    background-color: transparent;
    color: #60A5FA;
    border: none;
    border-radius: 5px;
    padding: 4px;
}

QLineEdit#searchEdit QToolButton:hover {
    background-color: #34465D;
    color: #FFFFFF;
}




/* TABLEAU DES ELEVES */

QTableWidget {
    background-color: #202833;
    alternate-background-color: #252F3D;
    color: #E2E8F0;

    border: 1px solid #344256;
    border-radius: 10px;

    gridline-color: #344256;
    outline: none;

    font-size: 12px;

    selection-background-color: #294B73;
    selection-color: #FFFFFF;
}

QTableWidget::item {
    padding: 10px;
    border-bottom: 1px solid #344256;
}

QTableWidget::item:hover {
    background-color: #2C3B50;
}

QTableWidget::item:selected {
    background-color: #294B73;
    color: #FFFFFF;
}




/* EN-TETES DU TABLEAU */

QHeaderView::section {
    background-color: #283547;
    color: #60A5FA;

    padding: 13px 8px;
    border: none;
    border-bottom: 2px solid #3B82F6;

    font-size: 12px;
    font-weight: bold;
}

QHeaderView::section:vertical {
    background-color: #202833;
    border: none;
}




/* BARRE DE DEFILEMENT */

QScrollBar:vertical {
    background-color: #171B22;
    width: 9px;
    border: none;
    margin: 0;
}

QScrollBar::handle:vertical {
    background-color: #405066;
    border-radius: 4px;
    min-height: 30px;
}

QScrollBar::handle:vertical:hover {
    background-color: #3B82F6;
}

QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical {
    height: 0;
}

QScrollBar:horizontal {
    background-color: #171B22;
    height: 9px;
    border: none;
}

QScrollBar::handle:horizontal {
    background-color: #405066;
    border-radius: 4px;
    min-width: 30px;
}

QScrollBar::handle:horizontal:hover {
    background-color: #3B82F6;
}

QScrollBar::add-line:horizontal,
QScrollBar::sub-line:horizontal {
    width: 0;
}




/* BOITES DE DIALOGUE */

QMessageBox {
    background-color: #202833;
}

QMessageBox QLabel {
    color: #E2E8F0;
    font-size: 13px;
}

QMessageBox QPushButton {
    background-color: #3B82F6;
    color: #FFFFFF;
    border: none;
    border-radius: 7px;
    padding: 8px 18px;
    min-width: 75px;
    font-weight: bold;
}

QMessageBox QPushButton:hover {
    background-color: #2563EB;
}




/* TOOLTIP */

QToolTip {
    background-color: #283547;
    color: #F8FAFC;
    border: 1px solid #3B82F6;
    padding: 6px;
    border-radius: 5px;
    font-size: 11px;
}
    )");

}

MainWindow::~MainWindow()
{
    delete ui;
}