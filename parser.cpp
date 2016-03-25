/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: parser.cpp
**
** parser.cpp - reboundlinx software
** Divides the input stream up into data packets
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2015
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/

#include "parser.h"
#include "datapacket.h"

Parser::Parser()
{
    QString df = QFileDialog::getOpenFileName(this); //not sure if this working?
    openFileSuccess = checkDataFileOpen(df);
    inputData = "";
}

Parser::Parser( const QString &df )
{
    openFileSuccess = checkDataFileOpen(df);
    inputData = "";
    if(openFileSuccess){ //check that string passed is valid
        openFileSuccess = readInputData();
        openFileSuccess = writeOutputData();
    }
}
Parser::~Parser()
{

}

bool Parser::checkDataFileOpen( const QString &df)
{
    bool ofs = true;
    QFile file(df);

    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(this, "Parser", tr("fail"));
        ofs = false;
        dataFile = "";
    }else{
        ofs = true;
        dataFile = df;
    }
    file.close();
    return(ofs);
}

bool Parser::readInputData()
{
    QFile file(dataFile);
    QTextStream in(&file);
    bool ifs = true;

    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(this, "readInputData", tr("fail"));
        ifs = false;
    }else{
//        QMessageBox::information(this, "readInputData", tr("Datafileopen"));
        ifs = true;
        inputData = (QByteArray) file.readAll();
    }
   return(ifs);
}

int Parser::numPackets(){
    const char etx = 0x03;
    int n = 0;

    n = inputData.count(etx);
    return(n);
}

QByteArray Parser::getPacket(QByteArray& ta){
    const char etx = 0x03;
    int n = ta.indexOf(etx);
    QByteArray ra = ta.left(n);
    ta = ta.remove(0,n+1);
    return ra;
}

bool Parser::writeOutputData(){
    bool r = false;

    //Setup Output data
    const QString outputfile = "reboundlinx.txt";
    QFile file(outputfile);
    QByteArray ta = inputData;

    if(!file.open(QIODevice::WriteOnly|QIODevice::Append)){
        QMessageBox::information(this, "writeDataPacket", tr("fail"));
        r = false;
    }else{
        int n = numPackets();
        r = true;
        for( int i = 0; i<n; ++i){
            QByteArray packet = getPacket(ta);
            DataPacket* p = new DataPacket(packet);
            writeDataPacket( p, file);
        }
    }
//    QMessageBox::information(this, "writeOutputData", tr("Write Data Success!!"));
    return r;
}

void Parser::writeDataPacket(DataPacket* p, QFile& file)
{
    QTextStream opl(&file);
    const QString format = "MM-dd-yy hh:mm";
    QString datetime;

    datetime = p->time.toString( format );

    opl<<"Test Number: " << p->testnumber <<'\t'<< datetime << '\n'
       <<"Data Type: " << p->datatype <<'\n'
       <<"Units: " << p->units <<'\n'
       <<"Discard Value: " << p->discardvalue <<'\n'
       <<"Reading:\n";

    for(int i = 0; i < p->reading.size();++i){
        opl<<p->reading[i]<<'\n';
    }

    opl<<"Average: " << p->average <<'\n'
       <<"Median:  " << p->median <<'\n'
       <<"Minimum: " << p->minimum <<'\n'
       <<"Maximum: " << p->maximum <<'\n'
       <<"Angle: " << p->angle <<'\n'
       <<"Correlation: " << p->correlation <<'\n'
       <<'\n';
}
