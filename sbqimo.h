#ifndef SBQIMO_H
#define SBQIMO_H

#include <QMainWindow>
#include "albumwd.h"
#include "mediawd.h"

#define WORK_DIR "/data/sbQiMo"
#define __FUNCTION__   __func__

QT_BEGIN_NAMESPACE
namespace Ui { class sbQiMo; }
QT_END_NAMESPACE

class sbQiMo : public QMainWindow
{
    Q_OBJECT

public:
    sbQiMo(QWidget *parent = nullptr);
    ~sbQiMo();

private slots:
    void on_pBtnAlbum_clicked();

    void on_pBtnExit_clicked();

    void on_pBtnMedia_clicked();

private:
    Ui::sbQiMo *ui;

    AlbumWd *albumSonWd;
    MediaWd *mediaSonWd;

};
#endif // SBQIMO_H
