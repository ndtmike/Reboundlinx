#ifndef REBOUNDLINXSPLASH_H
#define REBOUNDLINXSPLASH_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class ReboundLinxSplash;
}

class ReboundLinxSplash : public QDialog
{
    Q_OBJECT

public:
    explicit ReboundLinxSplash(QWidget *parent = 0);
    ~ReboundLinxSplash();

private:
    Ui::ReboundLinxSplash *ui;

private slots:
    void closeSplash();
};

#endif // REBOUNDLINXSPLASH_H
