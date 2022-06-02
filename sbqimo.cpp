#include "sbqimo.h"
#include "ui_sbqimo.h"

sbQiMo::sbQiMo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::sbQiMo)
{
    ui->setupUi(this);
    albumSonWd = new AlbumWd(this);
    mediaSonWd = new MediaWd(this);


}

sbQiMo::~sbQiMo()
{
    delete ui;
    delete albumSonWd;
    delete mediaSonWd;
}


void sbQiMo::on_pBtnAlbum_clicked()
{
    qDebug() << __func__;
    this->hide();
    this->albumSonWd->show();
}

void sbQiMo::on_pBtnMedia_clicked()
{
    qDebug() << __func__;
    this->hide();
    this->mediaSonWd->show();
}

void sbQiMo::on_pBtnExit_clicked()
{
    qDebug() << __func__;
    this->close();
}
