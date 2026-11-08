#include "listeelevespage.h"

#include <QAbstractItemView>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

ListeElevesPage::ListeElevesPage(QWidget *parent)
    : QWidget(parent)
{
    //PAGE LISTE DES ELEVES
    setObjectName("content");
    setAttribute(Qt::WA_StyledBackground, true);

    //layout principal de la page
    QVBoxLayout *elevesLayout = new QVBoxLayout(this);
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
    m_table = new QTableWidget;

    m_searchEdit = new QLineEdit;
    m_searchEdit->setPlaceholderText("Rechercherun élève (nom, prénom, matricule...)");
    m_searchEdit->setObjectName("searchEdit");
    m_searchEdit->setMinimumHeight(42);
    m_searchEdit->setClearButtonEnabled(true);

    //Nombre de colonnes
    m_table->setColumnCount(EleveRepository::NombreChamps);

    //Noms des colonnes
    m_table->setHorizontalHeaderLabels({
        "Matricule",
        "Nom",
        "Prenom",
        "Date de naissance",
        "Sexe",
        "Niveau",
        "Telephone",
        "Adresse"
    });


    //Autoriser le retour automatique à la ligne
    m_table->setWordWrap(true);

    //Ne pas remplacer le texte par des points de suspension
    m_table->setTextElideMode(Qt::ElideNone);

    //Adapter automatiquement la hauteur des lignes
    m_table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //ajuster le hauteur du ligne
    m_table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);



    //Adapter les colonnes a la largeur disponnible
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //Hauteur des lignes
    m_table->verticalHeader()->setDefaultSectionSize(42);

    //Masquer les numeros des lignes
    m_table->verticalHeader()->setVisible(false);

    //Selectionner une ligne entière
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);

    //Une seule ligne selectionnable a la fois
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);

    //Interdire la modification directe des cellules
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_table->setWordWrap(true);
    m_table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


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


    //Ajouter la barre de recherche
    tableLayout->addWidget(m_searchEdit);

    //Ajouter le tableau au cadre
    tableLayout->addWidget(m_table);

    //Ajouter les boutons au-dessus du tableau
    tableLayout->addLayout(actionsLayout);

    //Ajouter le cadre a la page
    elevesLayout->addWidget(tableCard);

    //Recherche dynamique des élèves
    connect(m_searchEdit, &QLineEdit::textChanged, this, &ListeElevesPage::rechercher);

    //Faire fonctionner les buttons du tableau
    connect(refreshButton, &QPushButton::clicked, this, &ListeElevesPage::chargerEleves);
    connect(deleteButton, &QPushButton::clicked, this, &ListeElevesPage::supprimerSelection);
}

//Recherche dynamique des élèves
void ListeElevesPage::rechercher(const QString &recherche)
{
    QString texte = recherche.trimmed();

    for (int ligne = 0; ligne < m_table->rowCount(); ligne++)
    {
        bool correspondance = false;

        //parcourir les 8 colonnes
        for (int colonne = 0; colonne < EleveRepository::NombreChamps; colonne++)
        {
            QTableWidgetItem *item = m_table->item(ligne, colonne);

            if(item && item->text().contains(texte, Qt::CaseInsensitive))
            {
                correspondance = true;
                break;
            }
        }

        // Afficher ou masquer la ligne

        m_table->setRowHidden(ligne, ! correspondance);
    }
}

//Fonction pour charger les eleves depuis le fichier
void ListeElevesPage::chargerEleves()
{
    //Vider le tableau avant de le remplir
    m_table->setRowCount(0);

    //Lire les eleves depuis le fichier
    QList<QStringList> eleves;
    if (!m_repo.lireTous(eleves)) {
        return;
    }

    for (const QStringList &informations : eleves) {

        //Ajouter une nouvelle ligne au tableau
        int nouvelleLigne = m_table->rowCount();
        m_table->insertRow(nouvelleLigne);

        //Remplir les 8 colonnes
        for (int colonne = 0; colonne < EleveRepository::NombreChamps; colonne++){
            QTableWidgetItem *cellule = new QTableWidgetItem(informations.at(colonne).trimmed());
            m_table->setItem(nouvelleLigne, colonne, cellule);
        }
    }

    m_table->resizeRowsToContents();

    emit donneesChangees();
}

//deleteButton
void ListeElevesPage::supprimerSelection()
{
    //Verifier si une ligne est séléctionnée
    int ligne = m_table->currentRow();

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
    QString matricule = m_table->item(ligne, EleveRepository::Matricule)->text();

    //Supprimer l'élève du fichier
    switch (m_repo.supprimer(matricule)) {
    case EleveRepository::ResultatSuppression::ErreurLecture:
        QMessageBox::critical(this, "Erreur", "Impossible de lire le fichier.");
        return;
    case EleveRepository::ResultatSuppression::ErreurEcriture:
        QMessageBox::critical(this, "Erreur", "Impossible de modifier le fichier.");
        return;
    case EleveRepository::ResultatSuppression::Ok:
        break;
    }

    //Actualiser le tableau
    chargerEleves();

    QMessageBox::information(this, "Suppression réussi", "L'élève a été supprimé.");
    emit donneesChangees();
}
