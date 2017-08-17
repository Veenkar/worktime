#ifndef WORKTIME_H
#define WORKTIME_H

#include <QMainWindow>

//qt headers
#include <qtimer.h>
#include <QTime>

namespace Ui {
class Worktime;
}

class Worktime : public QMainWindow
{
    Q_OBJECT

public:
    explicit Worktime(QWidget *parent = 0);
    ~Worktime();

private:
    Ui::Worktime *ui;
    QTimer* timer;
    QTime last_inactive;
    QTime curr_inactive;
    bool inactive_written;
    QTime INACTIVE_DELAY;

protected slots:
    void update();
    bool writeToLog(const QString &str);
};

#endif // WORKTIME_H
