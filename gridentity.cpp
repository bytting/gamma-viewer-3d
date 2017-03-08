#include "gridentity.h"

GridEntity::GridEntity(Qt3DCore::QEntity *parent, unsigned int count, float interval) :
    mMesh(new Qt3DRender::QGeometryRenderer()),
    mGeometry(new Qt3DRender::QGeometry(mMesh)),
    mDataBuffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, mGeometry)),
    mPositionAttribute(new Qt3DRender::QAttribute())
{
    int halfCount = count / 2;
    int numVerts = (halfCount * 4) * 2;
    float halfLength = interval * (float)halfCount;

    QByteArray vertexBufferData;
    vertexBufferData.resize(numVerts * 3 * sizeof(float));
    float *ptr = reinterpret_cast<float*>(vertexBufferData.data());

    int i = 0;
    float x = -interval * (float)halfCount;
    for(; i < (numVerts * 3) / 2; i += 6)
    {
        ptr[i] = x;
        ptr[i+1] = 0.0f;
        ptr[i+2] = -halfLength;

        ptr[i+3] = x;
        ptr[i+4] = 0.0f;
        ptr[i+5] = halfLength - interval;

        x += interval;
    }

    float z = -interval * (float)halfCount;
    for(; i < numVerts * 3; i += 6)
    {
        ptr[i] = -halfLength;
        ptr[i+1] = 0.0f;
        ptr[i+2] = z;

        ptr[i+3] = halfLength - interval;
        ptr[i+4] = 0.0f;
        ptr[i+5] = z;

        z += interval;
    }

    mDataBuffer->setData(vertexBufferData);

    mPositionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    mPositionAttribute->setBuffer(mDataBuffer);
    mPositionAttribute->setDataType(Qt3DRender::QAttribute::Float);
    mPositionAttribute->setDataSize(3);
    mPositionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());

    mGeometry->addAttribute(mPositionAttribute);

    mMesh->setInstanceCount(1);
    mMesh->setIndexOffset(0);
    mMesh->setFirstInstance(0);
    mMesh->setVertexCount(numVerts);
    mMesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    mMesh->setGeometry(mGeometry);

    mMaterial = new Qt3DExtras::QPhongMaterial(parent);
    mMaterial->setAmbient(QColor(255, 255, 255));

    mTransform = new Qt3DCore::QTransform();

    parent->addComponent(mMesh);
    parent->addComponent(mMaterial);
    parent->addComponent(mTransform);
}

GridEntity::~GridEntity()
{
    delete mTransform;
    mTransform = nullptr;
    delete mMaterial;
    mMaterial = nullptr;
    delete mPositionAttribute;
    mPositionAttribute = nullptr;
    delete mDataBuffer;
    mDataBuffer = nullptr;
    delete mGeometry;
    mGeometry = nullptr;
    delete mMesh;
    mMesh = nullptr;
}
