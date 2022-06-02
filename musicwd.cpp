#include "musicwd.h"
#include "ui_musicwd.h"

MusicWd::MusicWd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MusicWd)
{
    ui->setupUi(this);
}

MusicWd::~MusicWd()
{
    delete ui;
}
