#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog.h"
#include "ui_dialog.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QAuthenticator>
#include <QProgressBar>
#include <QDialog>
#include <QFileDialog>
#include <QTemporaryFile>
#include <QDir>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    nam(new QNetworkAccessManager(this)),
    progress(new QProgressBar(this))
{
    ui->setupUi(this);

    //auto list = nam->supportedSchemes(); //QStringList
    //foreach(QString str, list)
    //    qDebug() << str;

    setWindowTitle("File download");
    setupCoreWidgets();
    setupSignalsAndSlots();
}

void MainWindow::setupCoreWidgets()
{
    ui->urlLineEdit->setClearButtonEnabled(true);
    ui->plainTextEdit->setReadOnly(true);
    ui->headersPlainTextEdit->setReadOnly(true);
    ui->clearPushButton->setShortcut(Qt::CTRL + Qt::Key_C);

    progress->setMinimum(0);
    progress->setMaximum(100);
    progress->setValue(0);
    ui->statusBar->addWidget(progress);
    progress->setVisible(false);
}

void MainWindow::setupSignalsAndSlots()
{
    connect(ui->downloadPushButton, &QPushButton::clicked, this, &MainWindow::onDownloadClicked);
    connect(nam, &QNetworkAccessManager::authenticationRequired, this, &MainWindow::onAuthenticationRequired);
    connect(ui->exitPushButton, &QPushButton::clicked, &QApplication::quit);
    connect(ui->clearPushButton, &QPushButton::clicked, this, &MainWindow::clearFields);
}

void MainWindow::clearFields()
{
    ui->plainTextEdit->clear();
    ui->headersPlainTextEdit->clear();
}

void MainWindow::onDownloadClicked()
{
    auto url_text = ui->urlLineEdit->text().trimmed();

    if( url_text.isEmpty() )
        return;

    QTemporaryFile tf;
    if(tf.open())
     {
        QString tfname = tf.fileName();
        tf.close();
        file = new QFile(tfname);
    }


    QUrl url(url_text);
    makeRequest(url);
}

void MainWindow::makeRequest(const QUrl &url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "myApplication");
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    //if ()
    //    saveFile();

    reply = nam->get(request);

    connect(reply, &QNetworkReply::finished, this, &MainWindow::onFinished);
    connect(reply, &QNetworkReply::readyRead, this, &MainWindow::onReadyRead);
    connect(reply, &QNetworkReply::downloadProgress, this, &MainWindow::onDownloadProgress);

    progress->setValue(0);
    progress->setVisible(true);
}

void MainWindow::onFinished()
{
    QVariant target = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    reply->deleteLater();

    if(target.isValid()) {
        QUrl new_url = target.toUrl();
        auto button = QMessageBox::question(this, "Redirection", tr("Do you want to redirect to %1").arg(
                                  new_url.toString()));
        if(button == QMessageBox::No)
            return;
        else
           makeRequest(new_url);
    }

    if( reply->error() != QNetworkReply::NoError )
    {
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 0)
            ui->statusBar->showMessage(tr("HTTP status = %1").arg(
                                       reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()));
        ui->headersPlainTextEdit->clear();
        ui->headersPlainTextEdit->textCursor().insertText(reply->errorString());
    }
    else
    {
        ui->statusBar->clearMessage();
        auto list = reply->rawHeaderPairs();
        for (auto l : list)
            ui->headersPlainTextEdit->textCursor().insertText(tr(l.first + " : " + l.second + "\n"));
    }

    QVariant mime = reply->header(QNetworkRequest::ContentTypeHeader);
    if(mime.isValid())
    {
        if(!file->open(QIODevice::ReadOnly))
            qDebug() << "Error";
        else
            qDebug() << "Open";

        file->flush();
        file->seek(0);

        if(mime.toString().contains("text"))
            ui->plainTextEdit->textCursor().insertText( file->readAll() );

        else
        {
            QString fn = QFileDialog::getSaveFileName(this, "Save file");
            if(!fn.isNull())
            {
                QDir().rename(file->fileName(), fn);
                ui->plainTextEdit->textCursor().insertText( "Saved to " + fn + "." );
            }
        }
    }
    file->close();

}

void MainWindow::onReadyRead()
{
    if(!file->open(QIODevice::WriteOnly | QIODevice::Append))
        qDebug() << "Error";
    else
        qDebug() << "Open";

    file->write(reply->readAll());
    file->close();
}

void MainWindow::onDownloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qreal progress_ = (bytesTotal < 1) ? 1.0 : static_cast<qreal>(bytesSent)/bytesTotal;
    progress->setValue(progress_ * progress->maximum());
}

void MainWindow::onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    Dialog dialog;

    if(dialog.exec() == QDialog::Accepted)
    {
        authenticator->setUser(dialog.username);
        authenticator->setPassword(dialog.password);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
