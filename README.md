# GestionEleves

Application de bureau (Qt / C++) de gestion des élèves : inscription, liste, recherche, suppression et statistiques.

## Structure du projet

```
Projet_Fin_dAnnee/
├── CMakeLists.txt
├── resources/
│   ├── resources.qrc        Ressources Qt
│   └── style.qss            Feuille de style de toute l'application
└── src/
    ├── main.cpp             Point d'entrée
    ├── mainwindow.{h,cpp}   Fenêtre principale : assemble la barre latérale et les pages
    ├── data/
    │   └── eleverepository  Seul endroit qui lit / écrit eleves.txt
    ├── widgets/
    │   └── sidebar          Menu de navigation
    └── pages/
        ├── dashboardpage    Accueil et statistiques
        ├── inscriptionpage  Formulaire, validation, enregistrement
        └── listeelevespage  Tableau, recherche, suppression
```

## Format des données

Fichier `eleves.txt` : une ligne par élève, 8 champs séparés par `;`

```
matricule;nom;prenom;date_naissance;sexe;niveau;telephone;adresse
```

## Compilation

Qt 6 (Widgets) et CMake ≥ 3.16. Ouvrir `CMakeLists.txt` dans Qt Creator, puis Compiler et Exécuter.
