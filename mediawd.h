#ifndef MEDIAWD_H
#define MEDIAWD_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QImage>
#include <QThread>
#include <QMessageBox>
#include "mplayer.h"

namespace Ui {
class MediaWd;
}

class MediaWd : public QMainWindow
{
    Q_OBJECT

public:
    enum MediaPlayMode_t {
        MUSIC = 0,
        VIDEO,
    };

public:
    explicit MediaWd(QWidget *parent = nullptr);
    ~MediaWd();

    int getMediaFileByDir();
    int getMediaFileByDialog();
    int setCurrentMediaByIndex(int index);

private slots:
    void mediaPlayDone();

    void on_pBtnReturn_clicked();

    void on_pBtnLoadDir_clicked();

    void on_pBtnPlayMode_clicked();

    void on_pBtnPrev_clicked();

    void on_pBtnPlayPause_clicked();

    void on_pBtnNext_clicked();

private:
    Ui::MediaWd *ui;
    MPlayer *mplayer;
    int musicIndex;
    int videoIndex;
    QString music_dir;
    QString video_dir;
    MediaPlayMode_t playMode;
    QList<QFileInfo> *musicFileInfoList;
    QList<QFileInfo> *videoFileInfoList;
};

#endif // MEDIAWD_H
