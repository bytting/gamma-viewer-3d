#include "gamman3d.h"
#include "ui_gamman3d.h"
#include <cmath>

#define PI 3.14159

using namespace QtDataVisualization;

gamman3d::gamman3d(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gamman3d)
{
    ui->setupUi(this);    
}

gamman3d::~gamman3d()
{
    session->clear();
    delete ui;
}

bool gamman3d::initialize()
{
    setGeometry(0, 0, 800, 600);

    session = new Session();
    session->load("/home/drb/06092016_110323");

    scatter = new Q3DScatter();
    scatter->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetDirectlyAbove);
    scatter->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);

    QScatterDataProxy *proxy = new QScatterDataProxy();
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    scatter->addSeries(series);

    QScatterDataArray *dataArray = new QScatterDataArray();
    dataArray->resize(session->SpectrumCount());
    QScatterDataItem *p = &dataArray->first();

    for(const Spectrum* spec : session->getSpectrums())
    {
        double cosLat = std::cos(spec->latitudeStart * PI / 180.0);
        double sinLat = std::sin(spec->latitudeStart * PI / 180.0);
        double cosLon = std::cos(spec->longitudeStart * PI / 180.0);
        double sinLon = std::sin(spec->longitudeStart * PI / 180.0);
        double rad = 500.0;
        double x = rad * cosLat * cosLon;
        double y = rad * cosLat * sinLon;
        double z = rad * sinLat;
        p->setPosition(QVector3D(y, x, z));
        p++;
    }

    series->dataProxy()->resetArray(dataArray);
    series->setItemSize(0.15);
    series->setMeshSmooth(true);

    QWidget *container = QWidget::createWindowContainer(scatter);
    setCentralWidget(container);
    scatter->show();

    return true;
}
