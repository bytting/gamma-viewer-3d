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

#include "gammaanalyzer3d.h"
#include "ui_gammaanalyzer3d.h"
#include "colorspectrum.h"
#include "gridentity.h"
#include "spectrumentity.h"
#include "scene.h"
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QAction>
#include <QColor>
#include <QVector3D>
#include <QGeoCoordinate>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QObjectPicker>

GammaAnalyzer3D::GammaAnalyzer3D(QWidget *parent)
    :
      QMainWindow(parent),
      ui(new Ui::GammaAnalyzer3D)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setupWidgets();
    setupSignals();
}

GammaAnalyzer3D::~GammaAnalyzer3D()
{
    delete ui;
}

void GammaAnalyzer3D::closeEvent(QCloseEvent *event)
{
    event->ignore();
    onActionExit();
}

void GammaAnalyzer3D::setupWidgets()
{
    labelStatus = new QLabel(statusBar());
    statusBar()->addWidget(labelStatus);
}

void GammaAnalyzer3D::setupSignals()
{
    QObject::connect(
                ui->actionExit,
                &QAction::triggered,
                this,
                &GammaAnalyzer3D::onActionExit);

    QObject::connect(
                ui->actionLoadDoserateScript,
                &QAction::triggered,
                this,
                &GammaAnalyzer3D::onLoadDoserateScript);

    QObject::connect(
                ui->actionOpenSession,
                &QAction::triggered,
                this,
                &GammaAnalyzer3D::onOpenSession);
}

void GammaAnalyzer3D::onActionExit()
{
    try
    {
        for(auto p : scenes)
            delete p.second;

        scenes.clear();

        QApplication::exit();
    }
    catch(const std::exception &e)
    {
        qDebug() << e.what();
    }
}

void GammaAnalyzer3D::onOpenSession()
{
    try
    {
        auto sessionDir = QFileDialog::getExistingDirectory(
                    this,
                    tr("Open session directory"),
                    QDir::homePath(),
                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(sessionDir.isEmpty())
            return;

        sessionDir = QDir::toNativeSeparators(sessionDir);

        auto it = scenes.find(sessionDir);
        if(it != scenes.end())
        {
            auto *scene = it->second;
            scene->camera->setUpVector(QVector3D(0.0, 1.0, 0.0));
            scene->camera->setPosition(QVector3D(0, 20, 100.0f));
            scene->camera->setViewCenter(QVector3D(0, 0, 0));
            scene->window->show();
            scene->window->raise();
            scene->window->requestActivate();
            return;
        }

        auto *scene = new Scene(QColor(27, 46, 46));

        if(QFile::exists(doserateScript))
            scene->session->loadDoserateScript(doserateScript);
        scene->session->loadPath(sessionDir);
        scene->window->setTitle(scene->session->name());

        new GridEntityXZ(0.0f, 10, 10.0f, QColor(255, 255, 255), scene->root);

        Palette::ColorSpectrum colorSpectrum(
                    scene->session->minDoserate(),
                    scene->session->maxDoserate());

        auto halfX = (scene->session->maxX() - scene->session->minX()) / 2.0;
        auto halfY = (scene->session->maxY() - scene->session->minY()) / 2.0;

        for(Gamma::Spectrum *spec : scene->session->getSpectrumList())
        {
            QVector3D position(
                        (spec->x1() - scene->session->minX() - halfX) * 10000.0,
                        spec->coordinates.altitude() - scene->session->minAltitude(),
                        (spec->y1() - scene->session->minY() - halfY) * -10000.0);

            auto *entity = new SpectrumEntity(
                        position,
                        colorSpectrum(spec->doserate()),
                        spec,
                        scene->root);

            QObject::connect(
                        entity->picker(),
                        &Qt3DRender::QObjectPicker::pressed,
                        this,
                        &GammaAnalyzer3D::onSpectrumPicked);
        }

        scene->camera->setUpVector(QVector3D(0.0, 1.0, 0.0));
        scene->camera->setPosition(QVector3D(0, 20, 100.0f));
        scene->camera->setViewCenter(QVector3D(0, 0, 0));

        scene->window->show();

        scenes[sessionDir] = scene;
        labelStatus->setText("Session " + sessionDir + " loaded");
    }
    catch(GA::Exception &e)
    {
        QMessageBox::warning(this, "Warning", e.what());
    }
    catch(const std::exception &e)
    {
        qDebug() << e.what();
    }
}

void GammaAnalyzer3D::onLoadDoserateScript()
{
    try
    {
        QString scriptFileName = QFileDialog::getOpenFileName(
                    this,
                    tr("Load doserate script"),
                    QDir::homePath(),
                    tr("Lua script (*.lua)"));

        doserateScript = QDir::toNativeSeparators(scriptFileName);

        ui->lblDoserateScript->setText(
                    QStringLiteral("Loaded doserate script: ")
                    + doserateScript);
    }
    catch(const std::exception &e)
    {
        qDebug() << e.what();
    }
}

void GammaAnalyzer3D::onSpectrumPicked(Qt3DRender::QPickEvent *evt)
{
    try
    {
        if(evt->button() != Qt3DRender::QPickEvent::LeftButton)
            return;

        auto entity = qobject_cast<SpectrumEntity*>(sender()->parent());
        if(!entity)
        {
            qDebug() << "GammaAnalyzer3D::onSpectrumPicked: qobject_cast failed";
            return;
        }

        auto spec = entity->spectrum();

        ui->lblSessionSpectrum->setText(
                    QStringLiteral("Session / Spectrum: ") +
                    spec->sessionName() + " / " +
                    QString::number(spec->sessionIndex()));
        ui->lblCoordinates->setText(
                    QStringLiteral("Coordinates: ") +
                    spec->coordinates.toString(QGeoCoordinate::Degrees));
        ui->lblLivetimeRealtime->setText(
                    QStringLiteral("Livetime / Realtime: ") +
                    QString::number(spec->livetime() / 1000000.0) + "s / " +
                    QString::number(spec->realtime() / 1000000.0) + "s");
        ui->lblDoserate->setText(
                    QStringLiteral("Doserate: ") +
                    QString::number(spec->doserate(), 'E') + " μSv");
        ui->lblDate->setText(
                    QStringLiteral("Date: ") +
                    spec->gpsTimeStart().toLocalTime().
                    toString("yyyy-MM-dd hh:mm:ss"));
    }
    catch(const std::exception &e)
    {
        qDebug() << e.what();
    }
}
