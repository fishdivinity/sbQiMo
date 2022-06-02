#ifndef MPLAYER_H
#define MPLAYER_H

#include <QThread>
#include <QObject>
#include <QDebug>


#ifndef __cplusplus
extern "C"
{
#endif

//c程序的接口来自linux，所以，该qt程序只能在linux下进行编译
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#ifndef __cplusplus
}
#endif

class SystemFuncThread : public QThread
{
    Q_OBJECT
public:
    explicit SystemFuncThread(QString command = "")
    { this->command = command; }
    void run() override
    {
        qDebug() << "SystemFuncThread start.";
        int ret = system(command.toStdString().c_str());
        emit finishSysCmd(ret);
    }
    void loadCmd(QString command) { this->command = command; }
private:
    QString command;

signals:
    void finishSysCmd(int status);
};


class MPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MPlayer();
    ~MPlayer();
    void load(QString file, int playMode);
    void run();
    void play();
    void pause();
    void stop();
public: /* variable */
    bool isPlay;
    bool isRunning;

signals:
    void playDone(int status);

private slots:
    void playThreadDone(int status);

private:
    int playMode;
    QString mediaFilePath;
    SystemFuncThread *playerThread;

};

#endif // MPLAYER_H
