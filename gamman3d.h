#ifndef GAMMAN3D_H
#define GAMMAN3D_H

#include <QMainWindow>
#include <QtDataVisualization/Q3DScatter>
#include "session.h"

namespace Ui {
class gamman3d;
}

class gamman3d : public QMainWindow
{
    Q_OBJECT

public:
    explicit gamman3d(QWidget *parent = 0);
    ~gamman3d();

    bool initialize();

private:
    Ui::gamman3d *ui;
    Session *session;
    QtDataVisualization::Q3DScatter *scatter;
};

#endif // GAMMAN3D_H
