#ifndef INSCRIPTIONPAGE_H
#define INSCRIPTIONPAGE_H

#include <QWidget>

#include "data/eleverepository.h"

class QComboBox;
class QDateEdit;
class QLineEdit;

// Page d'inscription d'un élève (formulaire + validation + enregistrement)
class InscriptionPage : public QWidget
{
    Q_OBJECT

public:
    explicit InscriptionPage(QWidget *parent = nullptr);

signals:
    //Le fichier des élèves a (peut-être) changé
    void donneesChangees();

    //Clic sur "Annuler"
    void annulee();

private slots:
    void reinitialiser();
    void enregistrer();

private:
    //Verifier les champs (obligatoires, matricule, nom, date, telephone)
    bool formulaireValide(const QString &nom, const QString &prenom,
                          const QString &matricule, const QString &telephone);

    //Retourne true si l'élève existe déjà (ou si la vérification est impossible)
    bool dejaInscrit(const QString &matricule, const QString &nom,
                     const QString &prenom, const QString &dateNaissance);

    EleveRepository m_repo;

    QLineEdit *m_nomEdit;
    QLineEdit *m_prenomEdit;
    QDateEdit *m_dateEdit;
    QComboBox *m_sexeCombo;
    QComboBox *m_niveauCombo;
    QLineEdit *m_matriculeEdit;
    QLineEdit *m_telephoneEdit;
    QLineEdit *m_adresseEdit;
};

#endif // INSCRIPTIONPAGE_H
