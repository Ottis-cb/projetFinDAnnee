#include "inscriptionpage.h"

#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QFormLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QVBoxLayout>

InscriptionPage::InscriptionPage(QWidget *parent)
    : QWidget(parent)
{
    //PAGE INSCRIPTION
    setObjectName("content");
    setAttribute(Qt::WA_StyledBackground, true);

    QVBoxLayout *inscriptionLayout = new QVBoxLayout(this);
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
    m_nomEdit = new QLineEdit;
    m_nomEdit->setPlaceholderText("Nom de l'eleve");

    //champ prenom
    m_prenomEdit = new QLineEdit;
    m_prenomEdit->setPlaceholderText("Prenom de l'eleve");

    //Date de naissance
    m_dateEdit = new QDateEdit;
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDisplayFormat("dd/MM/yyyy");
    m_dateEdit->setDate(QDate::currentDate().addYears(-10));

    //sexe
    m_sexeCombo = new QComboBox;
    m_sexeCombo->addItems({"Selectionner",
                           "Masculin",
                           "Feminin"
                          });

    //Niveau
    m_niveauCombo = new QComboBox;
    m_niveauCombo->addItems({
        "Selectionner",
        "L1","L2","L3","M1","M2"
    });

    //Matricule
    m_matriculeEdit = new QLineEdit;
    m_matriculeEdit->setPlaceholderText("Numero matricule");
    //Telephone
    m_telephoneEdit = new QLineEdit;
    m_telephoneEdit->setPlaceholderText("Numero de telephone");

    //Adresse
    m_adresseEdit = new QLineEdit;
    m_adresseEdit->setPlaceholderText("Adresse de l'eleve");

    //Placer les champs dans le formulaire
    formLayout->addRow("Nom *", m_nomEdit);
    formLayout->addRow("Prenom *", m_prenomEdit);
    formLayout->addRow("Matricule *", m_matriculeEdit);
    formLayout->addRow("Date de naissance", m_dateEdit);
    formLayout->addRow("sexe *", m_sexeCombo);
    formLayout->addRow("Niveau *", m_niveauCombo);
    formLayout->addRow("Telephone", m_telephoneEdit);
    formLayout->addRow("Adresse", m_adresseEdit);

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
    connect(resetButton, &QPushButton::clicked, this, &InscriptionPage::reinitialiser);
    connect(cancelButton, &QPushButton::clicked, this, &InscriptionPage::annulee);
    connect(saveButton, &QPushButton::clicked, this, &InscriptionPage::enregistrer);
}

//Réinitialiser
void InscriptionPage::reinitialiser()
{
    m_nomEdit->clear();
    m_prenomEdit->clear();
    m_adresseEdit->clear();
    m_matriculeEdit->clear();
    m_telephoneEdit->clear();
    m_dateEdit->setDate(QDate::currentDate().addYears(-10));

    m_sexeCombo->setCurrentIndex(0);
    m_niveauCombo->setCurrentIndex(0);
    emit donneesChangees();
}

//Enregistrer
void InscriptionPage::enregistrer()
{
    //Récuperer les infos
    QString nom = m_nomEdit->text().trimmed();
    QString prenom = m_prenomEdit->text().trimmed();
    QString matricule = m_matriculeEdit->text().trimmed();
    QString dateNaissance = m_dateEdit->date().toString("dd/MM/yyyy");
    QString sexe = m_sexeCombo->currentText();
    QString niveau = m_niveauCombo->currentText();
    QString telephone = m_telephoneEdit->text().trimmed();
    QString adresse = m_adresseEdit->text().trimmed();

    //Validation du formulaire
    if (!formulaireValide(nom, prenom, matricule, telephone)) {
        return;
    }

    //Verification des doublons
    if (dejaInscrit(matricule, nom, prenom, dateNaissance)) {
        return;
    }

    //Ecrire les infos dans le fichier
    const QStringList champs = {matricule, nom, prenom, dateNaissance,
                                sexe, niveau, telephone, adresse};

    if (!m_repo.ajouter(champs)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier.");
        return;
    }

    //confirmer l'enregistrement
    QMessageBox::information(this, "succès", "L'élève a été enregistré avec succès.");
    emit donneesChangees();
}

bool InscriptionPage::formulaireValide(const QString &nom, const QString &prenom,
                                       const QString &matricule, const QString &telephone)
{
    //Verifier les champs obligatoire
    if (nom.isEmpty() || prenom.isEmpty() || matricule.isEmpty() || telephone.isEmpty() || m_sexeCombo->currentIndex() == 0 || m_niveauCombo->currentIndex() ==0 ){
        QMessageBox::warning(this, "formulaire incomplet","Veuillez remplir tous les champs obligatoires.");
        return false;
    }


    // VALIDATION DU MATRICULE

    // Refuser les caractères qui pourraient casser le fichier
    if (matricule.contains(';') ||
        matricule.contains('\n') ||
        matricule.contains('\r')) {

        QMessageBox::warning(this, "Matricule invalide",
                             "Le matricule contient des caractères interdits.");
        return false;
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
        return false;
    }

    if (!expressionNom.match(prenom).hasMatch()) {

        QMessageBox::warning(this, "Prénom invalide",
                             "Le prénom contient des caractères invalides.");
        return false;
    }


    // VALIDATION DE LA DATE DE NAISSANCE

    if (m_dateEdit->date() > QDate::currentDate()) {

        QMessageBox::warning(this, "Date invalide",
                             "La date de naissance ne peut pas être dans le futur.");
        return false;
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
        return false;
    }

    return true;
}

bool InscriptionPage::dejaInscrit(const QString &matricule, const QString &nom,
                                  const QString &prenom, const QString &dateNaissance)
{
    // VERIFICATION DES DOUBLONS
    if (!m_repo.existe()) {
        return false;
    }

    QList<QStringList> eleves;

    if (!m_repo.lireTous(eleves)) {

        QMessageBox::critical(this, "Erreur",
                              "Impossible de vérifier les inscriptions existantes.");
        return true;
    }

    for (const QStringList &informations : eleves) {

        QString ancienMatricule = informations.at(EleveRepository::Matricule).trimmed();
        QString ancienNom = informations.at(EleveRepository::Nom).trimmed();
        QString ancienPrenom = informations.at(EleveRepository::Prenom).trimmed();
        QString ancienneDate = informations.at(EleveRepository::DateNaissance).trimmed();

        // Comparaison du matricule sans tenir compte
        // des majuscules et minuscules.
        if (ancienMatricule.compare(
                matricule, Qt::CaseInsensitive) == 0) {

            QMessageBox::warning(this, "Matricule déjà utilisé",
                                 "Ce numéro matricule existe déjà.");

            return true;
        }

        // Comparaison de l'identité
        if (ancienNom.simplified().toCaseFolded()
                == nom.simplified().toCaseFolded() &&
            ancienPrenom.simplified().toCaseFolded()
                == prenom.simplified().toCaseFolded() &&
            ancienneDate == dateNaissance) {

            QMessageBox::warning(this, "Élève déjà inscrit",
                                 "Un élève avec le même nom, prénom "
                                 "et date de naissance existe déjà.");

            return true;
        }
    }

    return false;
}
