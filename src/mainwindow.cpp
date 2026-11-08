#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "pages/dashboardpage.h"
#include "pages/inscriptionpage.h"
#include "pages/listeelevespage.h"
#include "widgets/sidebar.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("GestionEleves");
    setMinimumSize(900, 600);
    resize(1200, 760);

    creerInterface();
    connecterSignaux();
    appliquerStyle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Construction de la fenêtre : barre latérale + zone des pages
void MainWindow::creerInterface()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    //fenetre principale
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    //Barre laterale
    m_sidebar = new SideBar;

    //zone principale
    QWidget *content = new QWidget;
    content->setObjectName("contentArea");

    //Layout occupant toute la zone principale
    QVBoxLayout *outerLayout = new QVBoxLayout(content);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    //gestionnaire de pages
    m_pages = new QStackedWidget(content);
    outerLayout->addWidget(m_pages);

    //Les trois pages
    m_dashboardPage = new DashboardPage;
    m_inscriptionPage = new InscriptionPage;
    m_listePage = new ListeElevesPage;

    m_pages->addWidget(m_dashboardPage);
    m_pages->addWidget(m_inscriptionPage);
    m_pages->addWidget(m_listePage);

    //Asseblage final
    mainLayout->addWidget(m_sidebar);
    mainLayout->addWidget(content, 1);
}

//Connexion entre la barre latérale, les pages et les statistiques
void MainWindow::connecterSignaux()
{
    //Navigation entre les pages
    connect(m_sidebar, &SideBar::accueilClicked, this, &MainWindow::afficherAccueil);
    connect(m_sidebar, &SideBar::inscriptionClicked, this, &MainWindow::afficherInscription);
    connect(m_sidebar, &SideBar::elevesClicked, this, &MainWindow::afficherListeEleves);

    //Cliquer sur le bouton de la page d'acceuil
    connect(m_dashboardPage, &DashboardPage::inscriptionDemandee, this, &MainWindow::afficherInscription);

    //Annuler l'inscription : retour à l'accueil
    connect(m_inscriptionPage, &InscriptionPage::annulee, this, [this](){
        m_dashboardPage->actualiserStatistiques();
        afficherAccueil();
    });

    //Actualiser les statistiques quand les données changent
    connect(m_inscriptionPage, &InscriptionPage::donneesChangees,
            m_dashboardPage, &DashboardPage::actualiserStatistiques);
    connect(m_listePage, &ListeElevesPage::donneesChangees,
            m_dashboardPage, &DashboardPage::actualiserStatistiques);
}

void MainWindow::afficherAccueil()
{
    m_pages->setCurrentWidget(m_dashboardPage);
    m_sidebar->setMenuActif(SideBar::Accueil);
}

void MainWindow::afficherInscription()
{
    m_pages->setCurrentWidget(m_inscriptionPage);
    m_sidebar->setMenuActif(SideBar::Inscription);
}

void MainWindow::afficherListeEleves()
{
    m_listePage->chargerEleves();
    m_pages->setCurrentWidget(m_listePage);
    m_sidebar->setMenuActif(SideBar::Eleves);
}

//Style global QSS (fichier resources/style.qss)
void MainWindow::appliquerStyle()
{
    QFile fichierStyle(":/style.qss");

    if (fichierStyle.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(fichierStyle.readAll()));
    }
}
