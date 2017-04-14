//  gamma-analyzer-3d - 3d visualization of sessions generated by gamma-analyzer
//  Copyright (C) 2017  Dag Robole
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "scene.h"
#include <QColor>
#include <QVector3D>
#include <Qt3DRender/QCameraLens>
#include <Qt3DExtras/QForwardRenderer>

Scene::Scene()
{
    session = new Gamma::Session();
    window = new Qt3DExtras::Qt3DWindow;
    window->defaultFrameGraph()->setClearColor(QColor(27, 48, 46));

    camera = window->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 10000.0f);
    camera->setUpVector(QVector3D(0.0, 1.0, 0.0));
    camera->setPosition(QVector3D(0, 20, 100.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));

    root = new Qt3DCore::QEntity;

    cameraController = new Qt3DExtras::QOrbitCameraController(root);
    cameraController->setLinearSpeed(50.0f);
    cameraController->setLookSpeed(180.0f);
    cameraController->setCamera(camera);

    window->setRootEntity(root);
}

Scene::~Scene()
{
    delete root;
    root = nullptr;
    camera = nullptr;
    window->destroy();
    delete window;
    window = nullptr;
    delete session;
}
