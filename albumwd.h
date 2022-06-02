#ifndef ALBUMWD_H
#define ALBUMWD_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QImage>
#include <QThread>
#include <QMessageBox>

namespace Ui {
class AlbumWdAutoPlayThread;
class AlbumWd;
}

class AlbumWdAutoPlayThread : public QThread
{
    Q_OBJECT

public:
    AlbumWdAutoPlayThread(QWidget *parent = nullptr, unsigned long seconds = 3):QThread(parent)
    {
        this->timeoutSeconds = seconds;
    }

    void run() override
    {
        qDebug() << "AlbumWdAutoPlayThread start.";
        while(1) {
            QThread::sleep(this->timeoutSeconds);
            emit autoPlayTimeOut();
        }
    }

    void setTimeOutSeconds(unsigned long seconds)
    {
        this->timeoutSeconds = seconds;
    }

private:
    unsigned long timeoutSeconds;

signals:
    void autoPlayTimeOut();
};

class AlbumWd : public QMainWindow
{
    Q_OBJECT

public:
    explicit AlbumWd(QWidget *parent = nullptr);
    ~AlbumWd();

public:
    int getAlbumDirByDialog();
    int getAlbumFileByDir();
    int setCurrentImageByIndex(int index);

public: /***************** variable *******************/
    enum playImgMode_t {
        AUTO = 0,
        MANUAL,
    };
    QString album_dir;
    playImgMode_t play_mode;
    QList<QFileInfo> *imgFileInfoList;

private slots:
    void autoPlayTimeOut();

    void on_pBtnReturn_clicked();

    void on_pBtnLoadDir_clicked();

    void on_pBtnSetPlayMode_clicked();

    void on_pBtnPrev_clicked();

    void on_pBtnNext_clicked();

private:
    Ui::AlbumWd *ui;
    int imgIndex;
    AlbumWdAutoPlayThread *autoPlayThread;
};

#endif // ALBUMWD_H
