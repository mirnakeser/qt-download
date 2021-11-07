#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QNetworkAccessManager;
class QNetworkReply;
class QAuthenticator;

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
    void clearFields();
    void authentication(QNetworkReply* r, QAuthenticator* a);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager * nam;
    QNetworkReply * reply;

    void setupSignalsAndSlots();

};

#endif // MAINWINDOW_H
