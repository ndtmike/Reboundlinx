/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/
/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: mainwindow.cpp
**
** mainwindow.cpp - reboundlinx software
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2015
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "settingsdialog.h"
#include "reboundlinxsplash.h"
#include "parser.h"

#include <QtSerialPort/QSerialPort>
#include <QProcess>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    console = new Console;
    console->setEnabled(false);
    setCentralWidget(console);
    serial = new QSerialPort(this);
    settings = new SettingsDialog;
    serialTimeOut = new QTimer(this);
    ui->actionQuit->setEnabled(true);

    initActionsConnections();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));

    connect(serialTimeOut,SIGNAL(timeout()), this,SLOT(endUpload()));

    QTimer* init_timer = new QTimer(this);
    init_timer->singleShot(100, this, SLOT(showSplash()));
}

MainWindow::~MainWindow()
{
    closeSerialPort();
    delete settings;
    delete ui;
}

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadOnly)) {
            console->setEnabled(true);
            console->setLocalEchoEnabled(p.localEchoEnabled);
            ui->statusBar->showMessage(tr("Connected"));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Open error"));
    }
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    console->setEnabled(false);
    ui->statusBar->showMessage(tr("Disconnected"));
}

void MainWindow::about()
{
    QString s;
    QTextStream toabout(&s);

    toabout << tr("The <b>Reboundlinx Software</b> is used with the <br>") <<
               tr("<b><i>James Instruments Inc.</i></b> Digital Rebound Hammers.<br><br>")<<
               tr("USA: +1773.4636565<br>")<<
               tr("Europe: +31.548.659032<br>")<<
               tr("Email: <a href=\"mailto:info@ndtjames.com?Subject=Reboundlinx\" target=\"_top\">info@ndtjames.com</a><br>")<<
               tr("Web: <a href=\"http://www.ndtjames.com\">http://www.ndtjames.com</a><br>");

    QMessageBox::information(this, tr("About Reboundlinx"), s);
}

void MainWindow::help()
{
    QProcess* help = new QProcess(this);
    help->start("hh.exe reboundlinx.chm");
}

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::readData()
{
    QFile file("rd.txt");
    QTextStream out(&file);
    serialTimeOut->start(500);
    QByteArray data = serial->readAll();
    console->putData(data);
    if(!file.open(QIODevice::Append)){
        QMessageBox::information(this, "readData", tr("Cannot write rd.txt"));
    }else{
        out<<data;
    }
    file.close();
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::initActionsConnections()
{
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(help()));
}

void MainWindow::reboundlinxData(){

    const QString rawdata = "rd.txt";
    const QString outputfile = "reboundlinx.txt";
    QFile file(outputfile);

    file.remove();
//    if(!file.remove())
//        QMessageBox::information(this,"reboundlinxData",tr("Unable to Remove reboundlinx.txt!"));
    new Parser( rawdata );
}

void MainWindow::processSerialPort()
{
    foundSerialPort = checkSerialPort();
    if(foundSerialPort){openSerialPort();}
}

void MainWindow::showSplash()
{
    const int five_sec = 5000;

    ReboundLinxSplash* splash = new ReboundLinxSplash();
    splash->setModal( true );
    splash->show();

    QTimer* splash_timer = new QTimer(this);
    splash_timer->singleShot(five_sec, this, SLOT(processSerialPort()));
}

bool MainWindow::checkSerialPort()
{
    QString description;
    QString manufacturer;
    QString portname;
    const QString portmanufacturer = "FTDI";
    const QString noport = tr("No Available Ports\nCheck instrument is plugged in\nor serial port installed properly\n then restart Reboundlinx");
    const QString messageTitle = "checkSerialPort";
    const QString connected = tr("Connected to ");
    QList <QSerialPortInfo> availablePorts;
    bool r = false;
    availablePorts = QSerialPortInfo::availablePorts();

    if(!availablePorts.isEmpty()){
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            portname = info.portName();
            description = info.description();
            manufacturer = info.manufacturer();
            if(manufacturer == portmanufacturer){
                serial->setPortName(portname);
                r = true;
            }
        if( r == true )break;
        }
    }
    if(r == false){
        QMessageBox::information(this,messageTitle,noport);
    }else{
        QMessageBox::information(this ,messageTitle , connected + portname );
    }
    return(r);
}

void MainWindow::endUpload()
{
    const QString outputfile = "reboundlinx.txt";
    QFile file(outputfile);

    serialTimeOut->stop();
    QMessageBox::information(this, "endUpload", tr("Upload Complete"));
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    reboundlinxData();
    console->setPlainText("");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream ReadFile(&file);
    console->setPlainText(ReadFile.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << console->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return true;
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("Text (*.txt)"));
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return false;

    return saveFile(files.at(0));
}

void MainWindow::copy()
{
    console->selectAll();
    console->copy();
}
