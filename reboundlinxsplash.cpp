#include "reboundlinxsplash.h"
#include "ui_reboundlinxsplash.h"

ReboundLinxSplash::ReboundLinxSplash(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReboundLinxSplash)
{
    const int five_sec = 5000;
    ui->setupUi(this);
    QTimer* init_timer = new QTimer(this);
    connect(init_timer, SIGNAL(timeout()), this, SLOT(update()));
    init_timer->singleShot(five_sec, this, SLOT(closeSplash()));
}

ReboundLinxSplash::~ReboundLinxSplash()
{
    delete ui;
}

void ReboundLinxSplash::closeSplash()
{
    close();
}
