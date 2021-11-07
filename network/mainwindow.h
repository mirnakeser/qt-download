#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QNetworkAccessManager;
class QNetworkReply;
class QUrl;
class QAuthenticator;
class QProgressBar;
class QTemporaryFile;
class QFile;
class QString;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onFinished();
    void onReadyRead();
    void onDownloadClicked();
    void onAuthenticationRequired(QNetworkReply *, QAuthenticator *);
    void onDownloadProgress(qint64, qint64);
    void clearFields();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager * nam;
    QNetworkReply * reply;
    QProgressBar * progress;
    //bool flagError = false;
    //bool flagMime = false;
    //QTemporaryFile * tempfile;
    QFile * file;
   // QString fileName = "";

    void makeRequest(QUrl const &);
    void setupSignalsAndSlots();
    void setupCoreWidgets();

};

#endif // MAINWINDOW_H
