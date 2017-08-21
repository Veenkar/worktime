#include "worktime.h"
#include "ui_worktime.h"
#include <QFile>

// Exclude rarely-used stuff from Windows headers
//#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT   0x0500
#define WINVER	       0x0500

//qt header files
#include <QDebug>


// Windows Header Files:
#include <windows.h>
#include <iostream>
#pragma comment(lib, "user32.lib")

double getIdleCount()
{
    double tickCount;
    double idleCount;
    LASTINPUTINFO lpi;
    lpi.cbSize = sizeof(LASTINPUTINFO);
    bool res = GetLastInputInfo(&lpi);
    tickCount = GetTickCount();
    if (!res)
    {
        qDebug()<<"failed, use GetLastError to get error code";
    }


    // lpi.dwTime now holds the tick count when last input was made

    //cout << lpi.dwTime << endl;

    idleCount = ( tickCount - lpi.dwTime );
    //cout << idleCount << endl;
    return idleCount;
}
QTime getIdleTime()
{
    QTime time = QTime(0, 0, 0);
    time = time.addMSecs(getIdleCount());
    //qDebug()<<time.toString(Qt::ISODateWithMs);
    return time;
}
QString getIdleTimeStr()
{
    return getIdleTime().toString(Qt::ISODateWithMs);
}

Worktime::Worktime(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Worktime)
{
    ui->setupUi(this);

    QString filename = "data.txt";
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray content = file.readAll();
        ui->textEdit->setText(content);
        file.close();
    }

    //cfg
    INACTIVE_DELAY = QTime(0, 15, 0);

    ui->delay->setText(INACTIVE_DELAY.toString(Qt::ISODate));
    inactive_written = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

Worktime::~Worktime()
{
    delete ui;
}

void Worktime::update()
{
    last_inactive=curr_inactive;
    curr_inactive=getIdleTime();
    qDebug()<<getIdleCount();
    QString timestr = curr_inactive.toString(Qt::ISODate);
    ui->inactive->setText(timestr);
    QDateTime curr_dt = QDateTime::currentDateTime();
    if( !inactive_written && curr_inactive >= INACTIVE_DELAY )
    {
        QDateTime dt = curr_dt.addMSecs(-curr_inactive.msecsSinceStartOfDay());
        writeToLog(QString("inactive since: %1").arg(dt.toString(Qt::ISODate)));
        inactive_written = true;
    }
    else if(last_inactive >= INACTIVE_DELAY && curr_inactive< last_inactive)
    {
        writeToLog(QString("back to active: %1").arg(curr_dt.toString(Qt::ISODate)));
        inactive_written = false;
    }
}

bool Worktime::writeToLog(const QString &str)
{
    QString filename = "data.txt";
    QFile file(filename);
    if (file.open(QIODevice::Append)) {
        QTextStream stream(&file);
        stream << str << endl;
        ui->textEdit->append(str+"\n");
        file.close();
    }
}
