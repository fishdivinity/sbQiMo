#include "albumwd.h"
#include "ui_albumwd.h"
#include "sbqimo.h"

AlbumWd::AlbumWd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AlbumWd)
{
    ui->setupUi(this);
    this->autoPlayThread = new AlbumWdAutoPlayThread();
    this->album_dir = WORK_DIR "/image";
    this->play_mode = AlbumWd::MANUAL;
    imgIndex = 0;
    imgFileInfoList = nullptr;
    connect(this->autoPlayThread,
            &AlbumWdAutoPlayThread::autoPlayTimeOut,
            this,
            &AlbumWd::autoPlayTimeOut);

}

AlbumWd::~AlbumWd()
{
    delete ui;
    if(this->imgFileInfoList != nullptr) {
        delete this->imgFileInfoList;
    }
    delete this->autoPlayThread;
}

int AlbumWd::getAlbumDirByDialog()
{
    qDebug() << __func__;
    QString dir;
    dir = QFileDialog::getExistingDirectory(this,
            tr("Select Album Directory"),
            ".",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir.length() == 0) {
        qCritical() << __func__ << " getExistingDirectory is null";
        return -1;
    }
    this->album_dir = dir;
    qDebug() << __func__ << " DIR:" << this->album_dir;
    this->getAlbumFileByDir();
    this->imgIndex = 0;
    this->setCurrentImageByIndex(this->imgIndex);
    return 0;
}

int AlbumWd::getAlbumFileByDir()
{
    qDebug() << __func__;
    QDir *dir =  new QDir(this->album_dir);
    QStringList filter;
    filter << "*.png" << "*.jpg" << "*.bmp";
    dir->setNameFilters(filter);
    if(this->imgFileInfoList != nullptr) { delete this->imgFileInfoList; }
    this->imgFileInfoList = new QList<QFileInfo>(dir->entryInfoList(filter));
    for(int i = 0; i < this->imgFileInfoList->size(); ++i) {
        qDebug() << __func__ << " loading: " << this->imgFileInfoList->at(i).absolutePath();
    }
    qDebug() << __func__ << " loading done. count=" << this->imgFileInfoList->size();
    delete dir;
    return 0;
}

int AlbumWd::setCurrentImageByIndex(int index)
{
    qDebug() << __func__ << " index:" << index;
    QFileInfo fileinfo = this->imgFileInfoList->at(index);
    QImage img = QImage(fileinfo.absoluteFilePath());
    if(img.isNull()) {
        qDebug() << __func__ << " Loading Image Fail.";
        return -1;
    }
    img = img.scaled(this->ui->lbImage->size(), Qt::KeepAspectRatio/*, Qt::SmoothTransformation*/);
    this->ui->lbImage->setPixmap(QPixmap::fromImage(img));
    this->ui->lbImageName->setText(fileinfo.fileName());
    return 0;
}

void AlbumWd::autoPlayTimeOut()
{
    qDebug() << __func__;
    if(this->imgFileInfoList == nullptr || this->imgFileInfoList->size() == 0) {
        qDebug() << __func__ << " none image, so not switch new image.";
        return ;
    }
    if(this->imgIndex == this->imgFileInfoList->size() - 1) {
        this->imgIndex = 0;
    } else {
        this->imgIndex++;
    }
    this->setCurrentImageByIndex(this->imgIndex);
}

void AlbumWd::on_pBtnSetPlayMode_clicked()
{
    qDebug() << __func__;
    if(this->play_mode == AlbumWd::MANUAL) {
        this->play_mode = AlbumWd::AUTO;
        this->ui->pBtnSetPlayMode->setText("Manual");
        this->autoPlayThread->start();
        qDebug() << __func__ << "set play mode Auto";
    } else if (this->play_mode == AlbumWd::AUTO) {
        this->play_mode = AlbumWd::MANUAL;
        this->ui->pBtnSetPlayMode->setText("Auto");
        this->autoPlayThread->terminate();
        qDebug() << __func__ << "set play mode Manual";
    }
}

void AlbumWd::on_pBtnReturn_clicked()
{
    qDebug() << "from album return main Window.";
    sbQiMo *parent;
    parent = (sbQiMo *)this->parentWidget();
    this->hide();
    parent->show();
}

void AlbumWd::on_pBtnLoadDir_clicked()
{
    qDebug() << __func__;
    this->getAlbumDirByDialog();
}

void AlbumWd::on_pBtnPrev_clicked()
{
    qDebug() << __func__;
    if(this->imgFileInfoList == nullptr || this->imgFileInfoList->size() == 0) {
        qDebug() << __func__ << "no to choose image dir.";
        QMessageBox::critical(this, "ERROR", "no to choose image directory.");
        return;
    }
    if(this->imgIndex == 0) {
        this->imgIndex = this->imgFileInfoList->size() - 1;
    } else {
        this->imgIndex--;
    }
    this->setCurrentImageByIndex(this->imgIndex);

}

void AlbumWd::on_pBtnNext_clicked()
{
    if(this->imgFileInfoList == nullptr || this->imgFileInfoList->size() == 0) {
        qDebug() << __func__ << "no to choose image dir.";
        QMessageBox::critical(this, "ERROR", "no to choose image directory.");
        return;
    }
    qDebug() << __func__;
    if(this->imgIndex == this->imgFileInfoList->size() - 1) {
        this->imgIndex = 0;
    } else {
        this->imgIndex++;
    }
    this->setCurrentImageByIndex(this->imgIndex);
}
