#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    nam(new QNetworkAccessManager(this))

{
    ui->setupUi(this);
    ui->urlLineEdit->setClearButtonEnabled(true);
    ui->textPlainTextEdit->setReadOnly(true);
    ui->headersPlainTextEdit->setReadOnly(true);
    //ui->clearPushButton->setShortcut(Qt::CTRL + Qt::Key_C);

    setWindowTitle("File download");
    setupSignalsAndSlots();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSignalsAndSlots()
{
    connect(ui->exitPushButton, &QPushButton::clicked, &QApplication::quit);
    connect(ui->clearPushButton, &QPushButton::clicked, this, &MainWindow::clearFields);
    connect(nam, SIGNAL(&QNetworkAccessManager::authenticationRequired(QNetworkReply*, QAuthenticator*)), this, SLOT(&MainWindow::authentication(QNetworkReply*, QAuthenticator*))); //--autentifikacija

}

void MainWindow::clearFields()
{
    ui->textPlainTextEdit->clear();
    ui->headersPlainTextEdit->clear();
}


