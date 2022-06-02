#ifndef MUSICWD_H
#define MUSICWD_H

#include <QMainWindow>

namespace Ui {
class MusicWd;
}

class MusicWd : public QMainWindow
{
    Q_OBJECT

public:
    explicit MusicWd(QWidget *parent = nullptr);
    ~MusicWd();

private:
    Ui::MusicWd *ui;
};

#endif // MUSICWD_H
