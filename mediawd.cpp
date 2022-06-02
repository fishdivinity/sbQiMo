#include "mediawd.h"
#include "ui_mediawd.h"
#include "sbqimo.h"

MediaWd::MediaWd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediaWd)
{
    ui->setupUi(this);
    this->mplayer = new MPlayer();
    this->musicFileInfoList = nullptr;
    this->videoFileInfoList = nullptr;
    this->musicIndex = 0;
    this->videoIndex = 0;
    this->playMode = MediaWd::MUSIC;
    this->music_dir = WORK_DIR "/music";
    this->video_dir = WORK_DIR "/video";
    connect(this->mplayer, &MPlayer::playDone,
            this, &MediaWd::mediaPlayDone);
}

MediaWd::~MediaWd()
{
    delete ui;
    delete mplayer;
    if(this->musicFileInfoList != nullptr) {
        delete this->musicFileInfoList;
    }
    if(this->videoFileInfoList != nullptr) {
        delete this->videoFileInfoList;
    }
}

int MediaWd::getMediaFileByDialog()
{
    qDebug() << __func__;
    QString dir;
    const char *dialog_title;

    if(this->playMode == MediaWd::MUSIC) {
        dialog_title = QString("Select Music Directory").toStdString().c_str();
    } else if (this->playMode == MediaWd::VIDEO) {
        dialog_title = QString("Select Video Directory").toStdString().c_str();
    }
    dir = QFileDialog::getExistingDirectory(this,
            tr(dialog_title),
            ".",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir.length() == 0) {
        qCritical() << __func__ << " getExistingDirectory is null";
        return -1;
    }
    if(this->playMode == MediaWd::MUSIC) {
        this->music_dir = dir;
        this->musicIndex = 0;
    } else if(this->playMode == MediaWd::VIDEO) {
        this->video_dir = dir;
        this->videoIndex = 0;
    }
    qDebug() << __func__ << " DIR:" << dir;
    this->getMediaFileByDir();
    this->setCurrentMediaByIndex(0);
    return 0;
}

int MediaWd::setCurrentMediaByIndex(int index)
{
    qDebug() << __func__ << "index: " << index;

    QFileInfo fileInfo;
    if(this->playMode == MediaWd::MUSIC) {
        if(this->musicFileInfoList == nullptr || this->musicFileInfoList->size() == 0){
            qDebug() << __func__ << "no to choose dir.";
            return -1;
        }
        fileInfo = this->musicFileInfoList->at(index);
        this->mplayer->load(fileInfo.absoluteFilePath(), MediaWd::MUSIC);
    } else if (this->playMode == MediaWd::VIDEO) {
        if(this->videoFileInfoList == nullptr || this->videoFileInfoList->size() == 0){
            qDebug() << __func__ << "no to choose dir.";
            return -1;
        }
        fileInfo = this->videoFileInfoList->at(index);
        this->mplayer->load(fileInfo.absoluteFilePath(), MediaWd::VIDEO);
    }
    if(this->mplayer->isRunning) {
        this->mplayer->stop();
    }
    this->mplayer->run();
    return 0;
}

int MediaWd::getMediaFileByDir()
{
    qDebug() << __func__;
    QDir *dir;
//    QList<QFileInfo> *fileInfoListPtr;
    QStringList filter;

    if(this->playMode == MediaWd::MUSIC) {
        dir = new QDir(this->music_dir);
        filter << "*.mp3" << "*.wav";
        this->musicIndex = 0;
        dir->setNameFilters(filter);
        this->musicFileInfoList = new QList<QFileInfo>(dir->entryInfoList(filter));
        for(int i = 0; i < this->musicFileInfoList->size(); ++i) {
            qDebug() << __func__ << " loading: " << this->musicFileInfoList->at(i).absoluteFilePath();
        }
        qDebug() << __func__ << " loading done. count=" << this->musicFileInfoList->size();
    } else if (this->playMode == MediaWd::VIDEO) {
        dir = new QDir(this->video_dir);
        filter << "*.mp4" << "*.avi";
        this->videoIndex = 0;
        dir->setNameFilters(filter);
        if(this->videoFileInfoList != nullptr) { delete this->videoFileInfoList; }
        this->videoFileInfoList = new QList<QFileInfo>(dir->entryInfoList(filter));
        for(int i = 0; i < this->videoFileInfoList->size(); ++i) {
            qDebug() << __func__ << " loading: " << this->videoFileInfoList->at(i).absoluteFilePath();
        }
        qDebug() << __func__ << " loading done. count=" << this->videoFileInfoList->size();
    }
    delete dir;
    return 0;
}

void MediaWd::on_pBtnReturn_clicked()
{
    qDebug() << "from Media return main Window.";
    sbQiMo *parent;
    parent = (sbQiMo *)this->parentWidget();
    this->hide();
    parent->show();
}

void MediaWd::on_pBtnLoadDir_clicked()
{
    qDebug() << __func__;
    this->getMediaFileByDialog();
}

void MediaWd::on_pBtnPlayMode_clicked()
{
    qDebug() << __func__;
    if(this->playMode == MediaWd::MUSIC) {
        this->playMode = MediaWd::VIDEO;
        this->ui->pBtnPlayMode->setText("V");
    } else if(this->playMode == MediaWd::VIDEO) {
        this->playMode = MediaWd::MUSIC;
        this->ui->pBtnPlayMode->setText("M");
    }
    qDebug() << "Current Mode: " << this->playMode;
}

void MediaWd::on_pBtnPrev_clicked()
{
    qDebug() << __func__;
    if(this->playMode == MediaWd::MUSIC) {
        if(this->musicFileInfoList == nullptr || this->musicFileInfoList->size() == 0) {
            qDebug() << __func__ << "no to choose music dir.";
            QMessageBox::critical(this, "ERROR", "no to choose music directory.");
            return ;
        }
        if(this->musicIndex == 0) {
            this->musicIndex = this->musicFileInfoList->size() - 1;
        } else {
            this->musicIndex--;
        }
        this->setCurrentMediaByIndex(this->musicIndex);
    } else if(this->playMode == MediaWd::VIDEO) {
        if(this->videoFileInfoList == nullptr || this->videoFileInfoList->size() == 0) {
            qDebug() << __func__ << "no to choose video dir.";
            QMessageBox::critical(this, "ERROR", "no to choose video directory.");
            return ;
        }
        if(this->videoIndex == 0) {
            this->videoIndex = this->videoFileInfoList->size() - 1;
        } else {
            this->videoIndex--;
        }
        this->setCurrentMediaByIndex(this->videoIndex);
    }
}

void MediaWd::on_pBtnPlayPause_clicked()
{
    qDebug() << __func__;
    if(this->musicFileInfoList == nullptr || this->musicFileInfoList->size() == 0
    || this->videoFileInfoList == nullptr || this->videoFileInfoList->size() == 0) {
        qDebug() << __func__ << "no to choose dir.";
        QMessageBox::critical(this, "ERROR", "no to choose directory.");
        return;
    }
    if(this->mplayer->isPlay) {
        this->mplayer->pause();
        qDebug() << __func__ << "pause";
    } else {
        this->mplayer->play();
        qDebug() << __func__ << "playing";
    }
}

void MediaWd::on_pBtnNext_clicked()
{
    qDebug() << __func__;
    if(this->playMode == MediaWd::MUSIC) {
        if(this->musicFileInfoList == nullptr || this->musicFileInfoList->size() == 0) {
            qDebug() << __func__ << "no to choose music dir.";
            QMessageBox::critical(this, "ERROR", "no to choose music directory.");
            return ;
        }
        if(this->musicIndex == this->musicFileInfoList->size() - 1) {
            this->musicIndex = 0;
        } else {
            this->musicIndex++;
        }
        this->setCurrentMediaByIndex(this->musicIndex);
    } else if(this->playMode == MediaWd::VIDEO) {
        if(this->videoFileInfoList == nullptr || this->videoFileInfoList->size() == 0) {
            qDebug() << __func__ << "no to choose video dir.";
            QMessageBox::critical(this, "ERROR", "no to choose video directory.");
            return ;
        }
        if(this->videoIndex == this->videoFileInfoList->size() - 1) {
            this->videoIndex = 0;
        } else {
            this->videoIndex++;
        }
        this->setCurrentMediaByIndex(this->videoIndex);
    }
}

void MediaWd::mediaPlayDone()
{
    qDebug() << __func__;
    if(this->playMode == MediaWd::MUSIC) {
        if(this->musicFileInfoList == nullptr || this->musicFileInfoList->size() == 0) {
            qDebug() << __func__ << "no to choose music dir.";
            return ;
        }
        if(this->musicIndex == this->musicFileInfoList->size() - 1) {
            this->musicIndex = 0;
        } else {
            this->musicIndex++;
        }
        this->setCurrentMediaByIndex(this->musicIndex);
    } else if(this->playMode == MediaWd::VIDEO) {
        if(this->videoFileInfoList == nullptr || this->videoFileInfoList->size() == 0) {
            qDebug() << __func__ << "no to choose video dir.";
            return ;
        }
        if(this->videoIndex == this->videoFileInfoList->size() - 1) {
            this->videoIndex = 0;
        } else {
            this->videoIndex++;
        }
        this->setCurrentMediaByIndex(this->videoIndex);
    }
}
