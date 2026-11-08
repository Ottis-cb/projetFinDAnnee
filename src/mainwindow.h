#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DashboardPage;
class InscriptionPage;
class ListeElevesPage;
class QStackedWidget;
class SideBar;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    //Navigation entre les pages
    void afficherAccueil();
    void afficherInscription();
    void afficherListeEleves();

private:
    void creerInterface();
    void connecterSignaux();
    void appliquerStyle();

    Ui::MainWindow *ui;

    SideBar *m_sidebar = nullptr;
    QStackedWidget *m_pages = nullptr;
    DashboardPage *m_dashboardPage = nullptr;
    InscriptionPage *m_inscriptionPage = nullptr;
    ListeElevesPage *m_listePage = nullptr;
};
#endif // MAINWINDOW_H
