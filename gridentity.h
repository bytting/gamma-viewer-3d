#ifndef GRIDENTITY_H
#define GRIDENTITY_H

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QGeometry>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/Qt3DWindow>

class GridEntity
{

public:

    GridEntity(Qt3DCore::QEntity *parent, unsigned int count, float interval);
    ~GridEntity();

private:

    Qt3DRender::QGeometryRenderer *mMesh;
    Qt3DRender::QGeometry *mGeometry;
    Qt3DRender::QBuffer *mDataBuffer;
    Qt3DRender::QAttribute *mPositionAttribute;
    Qt3DExtras::QPhongMaterial *mMaterial;
    Qt3DCore::QTransform *mTransform;
};

#endif // GRIDENTITY_H
