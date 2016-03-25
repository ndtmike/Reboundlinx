/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: DataPacket.h
**
** DataPacket - reboundlinx software
** Divides the DataPacket into reall data that can be used.
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2015
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/
/*
  This routine decodes an individual data packet.

    MPA /
  Strength    Rebound
   Byte #      Byte #   Field
  ---------   -------   -----
   01         01        (0x02) STX: Start of packet
   02 -  04   02 - 04   Test Number
   05         05        Data Type: $52 = Rebound, $53 = Strength
   06         06        Units: $23 = Rebound number, $4D = MPA, $50 = PSI
   07 -  08   07 - 08   Discard Value
   09 -  15   09 - 10   Reading # 1
   16 -  22   11 - 12   Reading # 2
   23 -  29   13 - 14   Reading # 3
   30 -  36   15 - 16   Reading # 4
   37 -  43   17 - 18   Reading # 5
   44 -  50   19 - 20   Reading # 6
   51 -  57   21 - 22   Reading # 7
   58 -  59   23 - 24   Reading # 8
   65 -  71   25 - 26   Reading # 9
   72 -  78   27 - 28   Reading #10
   79 -  85   29 - 30   Reading #11
   86 -  92   31 - 32   Reading #12
   93 -  99   33 - 34   Reading #13
  100 - 106   35 - 36   Reading #14
  107 - 113   37 - 38   Reading #15
  114 - 120   39 - 40   Reading #16
  121 - 127   41 - 42   Reading #17
  128 - 134   43 - 44   Reading #18
  135 - 141   45 - 46   Reading #19
  142 - 148   47 - 48   Reading #20
  149 - 155   49 - 50   Average
  156 - 162   51 - 52   Median
  163 - 169   53 - 54   Minimum
  170 - 176   55 - 56   Maximum
  177         57        Angle: $43(C) = Ceiling, $46(F) = Floor, $57(W) = Wall
  178         58        Correlation: $55(U) = Cube, $59(Y) = Cylinder
  179 - 180   59 - 60   Hour
  181 - 182   61 - 62   Minute
  183 - 184   63 - 64   Month
  185 - 186   65 - 66   Day
  187 - 188   67 - 68   Year
  189         69        Spare
  190         70        (0x03)ETX: End of packet

******************************************************************************
*/

#ifndef DATAPACKET_H
#define DATAPACKET_H

#include <QString>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>

class DataPacket
{
public:
    DataPacket();
    DataPacket(QByteArray Packet);
    ~DataPacket();

    QString testnumber;
    QString datatype;
    QString units;
    QString discardvalue;
    QList<QString> reading;
    QString average;
    QString median;
    QString minimum;
    QString maximum;
    QString angle;
    QString correlation;
    QDateTime time;

    bool error_flag;

private:
    void readDataPacket(QByteArray& p) ;
    QString readTestNumber( QByteArray& p)const;
    QString readDatatype(QByteArray& p)const;
    QString readUnits(QByteArray& p)const;
    QString readDiscardValue(QByteArray& p)const;
    QString readAverage(QByteArray& p);
    QString readMedian(QByteArray& p);
    QString readMinimum(QByteArray& p);
    QString readMaximum(QByteArray& p);
    QString readAngle(QByteArray& p)const;
    QString readCorrelation(QByteArray& p)const;
    QDateTime readDateTime(QByteArray& p)const;
    bool readReading(QByteArray& p);
    QByteArray formatReadings(int& length, int& index, QByteArray w );
};

#endif // DATAPACKET_H
