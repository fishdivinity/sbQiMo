#include "mplayer.h"
#include "sbqimo.h"

MPlayer::MPlayer()
{
    this->isPlay = false;
    this->isRunning = false;
    this->mediaFilePath = WORK_DIR "/media";
    this->playMode = MediaWd::MUSIC;
    this->playerThread = new SystemFuncThread();
    QObject::connect(this->playerThread,
                     &SystemFuncThread::finishSysCmd,
                     this,
                     &MPlayer::playThreadDone);
}

MPlayer::~MPlayer()
{
    delete this->playerThread;
}

void MPlayer::load(QString file, int playMode)
{
    this->mediaFilePath = file;
    this->playMode = playMode;
}

void MPlayer::run()
{
    char cmdBuf[1024] = {0};
    if(this->isPlay) {
        return ;
    }
    if(this->playMode == MediaWd::MUSIC) {
        sprintf(cmdBuf, "mplayer %s 1>/dev/null ",
                this->mediaFilePath.toStdString().c_str());
        qDebug() << "MediaWd::MUSIC cmdBuf=" << cmdBuf;
    } else if(this->playMode == MediaWd::VIDEO) {
        sprintf(cmdBuf, "mplayer %s -zoom -x 800 -y 480 1>/dev/null ",
                this->mediaFilePath.toStdString().c_str());
        qDebug() << "MediaWd::MUSIC cmdBuf=" << cmdBuf;
    }
    this->playerThread->loadCmd(QString(cmdBuf));
    this->playerThread->start();
    this->isRunning = true;
    qDebug() << __func__ << "cmdBuf: " << cmdBuf;
}

void MPlayer::play()
{
    if(!this->isRunning) { return ; }
    qDebug() << __func__;
    if(!this->isPlay) {
        system("killall -SIGCONT mplayer");
    }
    this->isPlay = true;
}

void MPlayer::pause()
{
    if(!this->isRunning) { return ; }
    qDebug() << __func__;
    if(this->isPlay) {
        system("killall -SIGSTOP mplayer");
    }
    this->isPlay = false;
}

void MPlayer::stop()
{
    qDebug() << __func__;
    system("killall mplayer");
    this->isRunning = false;
    this->playerThread->terminate();
}

void MPlayer::playThreadDone(int status)
{
    this->isRunning = false;
    emit playDone(status);
}
