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

#ifndef SCENE_H
#define SCENE_H

#include "session.h"
#include "selectionentity.h"
#include <memory>
#include <QColor>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DCore/QEntity>

struct Scene
{
    explicit Scene(const QColor &clearColor);
    Scene(const Scene &rhs) = delete;
    ~Scene();

    Scene &operator = (const Scene &) = delete;

    std::unique_ptr<Gamma::Session> session;
    Qt3DExtras::Qt3DWindow *window;
    Qt3DCore::QEntity *root;
    Qt3DRender::QCamera *camera;
    Qt3DExtras::QOrbitCameraController *cameraController;
    std::unique_ptr<SelectionEntity> selected, marked;

    bool hasChildEntity(Qt3DCore::QEntity *entity) const;
};

#endif // SCENE_H
