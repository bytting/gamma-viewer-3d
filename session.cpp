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

#include "session.h"
#include <exception>
#include <cmath>
#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

namespace Gamma
{

Session::Session()
    :
      L(luaL_newstate()),
      mScriptLoaded(false),
      mLivetime(0.0),
      mIterations(0),
      mMinDoserate(0.0),
      mMaxDoserate(0.0),
      mMinX(0.0),
      mMaxX(0.0),
      mMinY(0.0),
      mMaxY(0.0),
      mMinZ(0.0),
      mMaxZ(0.0),
      mMinLatitude(0.0),
      mMaxLatitude(0.0),
      mMinLongitude(0.0),
      mMaxLongitude(0.0),
      mMinAltitude(0.0),
      mMaxAltitude(0.0),
      mLogarithmicColorScale(true)
{
    if(!L.get())
        throw Exception_UnableToCreateLuaState("Session::Session");

    luaL_openlibs(L.get());
}

Session::~Session()
{
    try
    {
        clear();
    }
    catch(const std::exception& e)
    {
        qDebug() << e.what();
    }
}

const SpectrumList &Session::spectrumList() const
{
    return mSpectrumList;
}

const Spectrum &Session::spectrum(SpectrumListSize index) const
{
    if(index >= mSpectrumList.size())
        throw Exception_IndexOutOfBounds("Session::spectrum");

    return *mSpectrumList[index];
}

void Session::loadPath(QString sessionPath)
{
    const QDir sessionDir(sessionPath);
    if(!sessionDir.exists())
        throw Exception_DirDoesNotExist(sessionDir.absolutePath());

    const QDir spectrumDir(sessionPath + QDir::separator() +
                     QStringLiteral("json"));
    if (!spectrumDir.exists())
        throw Exception_DirIsNotASession(spectrumDir.absolutePath());

    const QString sessionFile = sessionPath + QDir::separator() +
            QStringLiteral("session.json");
    if(!QFile::exists(sessionFile))
        throw Exception_DirIsNotASession(sessionDir.absolutePath());

    loadSessionFile(sessionFile);

    const auto fileEntries = spectrumDir.entryInfoList(
                QStringList() << "*.json",
                QDir::NoDotAndDotDot | QDir::Files);

    clear();

    bool firstIteration = true;
    for(const auto &fileEntry : fileEntries)
    {
        try
        {
            auto spec = std::make_unique<Spectrum>(fileEntry.absoluteFilePath());

            if(mScriptLoaded)
                spec->calculateDoserate(mDetector, L.get());

            if(firstIteration)
            {
                firstIteration = false;
                mMinDoserate = mMaxDoserate = spec->doserate();
                mMinX = mMaxX = spec->position.x();
                mMinY = mMaxY = spec->position.y();
                mMinZ = mMaxZ = spec->position.z();
                mMinLatitude = mMaxLatitude = spec->coordinate.latitude();
                mMinLongitude = mMaxLongitude = spec->coordinate.longitude();
                mMinAltitude = mMaxAltitude = spec->coordinate.altitude();
            }
            else
            {
                if(mMinDoserate > spec->doserate())
                    mMinDoserate = spec->doserate();
                if(mMaxDoserate < spec->doserate())
                    mMaxDoserate = spec->doserate();

                if(mMinX > spec->position.x())
                    mMinX = spec->position.x();
                if(mMaxX < spec->position.x())
                    mMaxX = spec->position.x();

                if(mMinY > spec->position.y())
                    mMinY = spec->position.y();
                if(mMaxY < spec->position.y())
                    mMaxY = spec->position.y();

                if(mMinZ > spec->position.z())
                    mMinZ = spec->position.z();
                if(mMaxZ < spec->position.z())
                    mMaxZ = spec->position.z();

                if(mMinLatitude > spec->coordinate.latitude())
                    mMinLatitude = spec->coordinate.latitude();
                if(mMaxLatitude < spec->coordinate.latitude())
                    mMaxLatitude = spec->coordinate.latitude();

                if(mMinLongitude > spec->coordinate.longitude())
                    mMinLongitude = spec->coordinate.longitude();
                if(mMaxLongitude < spec->coordinate.longitude())
                    mMaxLongitude = spec->coordinate.longitude();

                if(mMinAltitude > spec->coordinate.altitude())
                    mMinAltitude = spec->coordinate.altitude();
                if(mMaxAltitude < spec->coordinate.altitude())
                    mMaxAltitude = spec->coordinate.altitude();
            }

            mSpectrumList.emplace_back(std::move(spec));
        }
        catch(const Exception &e)
        {
            qDebug() << e.what();
        }
        catch(const std::exception &e)
        {
            qDebug() << e.what();
        }
    }

    mHalfX = (mMaxX - mMinX) / 2.0;
    mHalfY = (mMaxY - mMinY) / 2.0;
    mHalfZ = (mMaxZ - mMinZ) / 2.0;

    centerPosition.setX(mMinX + mHalfX);
    centerPosition.setY(mMinY + mHalfY);
    centerPosition.setZ(mMinZ + mHalfZ);
    centerCoordinate.setAnglesFromCartesian(centerPosition);

    northCoordinate = centerCoordinate.atDistanceAndAzimuth(50.0, 0.0);
    northPosition = northCoordinate.toCartesian();
}

void Session::loadSessionFile(QString sessionFile)
{
    QFile jsonFile(sessionFile);
    if(!jsonFile.open(QFile::ReadOnly))
        throw Exception_UnableToLoadFile(sessionFile);

    auto doc = QJsonDocument().fromJson(jsonFile.readAll());
    if(!doc.isObject())
        throw Exception_InvalidSessionFile(sessionFile);

    auto root = doc.object();

    if(!root.contains("Name"))
        throw Exception_MissingJsonValue("Session:Name");
    if(!root.contains("Comment"))
        throw Exception_MissingJsonValue("Session:Comment");
    if(!root.contains("Livetime"))
        throw Exception_MissingJsonValue("Session:Livetime");
    if(!root.contains("Iterations"))
        throw Exception_MissingJsonValue("Session:Iterations");
    if(!root.contains("DetectorType"))
        throw Exception_MissingJsonValue("Session:DetectorType");
    if(!root.contains("Detector"))
        throw Exception_MissingJsonValue("Session:Detector");

    mName = root.value("Name").toString();
    mComment = root.value("Comment").toString();
    mLivetime = root.value("Livetime").toInt();
    mIterations = root.value("Iterations").toInt();
    mDetectorType.loadJson(root.value("DetectorType").toObject());
    mDetector.loadJson(root.value("Detector").toObject());
}

void Session::loadDoserateScript(QString scriptFileName)
{
    if(luaL_dofile(L.get(), scriptFileName.toStdString().c_str()))
        throw Exception_LoadDoserateScriptFailed(scriptFileName);
    mScriptLoaded = true;
}

void Session::clear()
{
    mSpectrumList.clear();

    mName = "";
    mIterations = 0;
    mLivetime = mMinDoserate = mMaxDoserate = 0.0;
    mMinX = mMaxX = mMinY = mMaxY = mMinZ = mMaxZ = 0.0;
    mMinLatitude = mMaxLatitude = 0.0;
    mMinLongitude = mMaxLongitude = 0.0;
    mMinAltitude = mMaxAltitude = 0.0;
}

QVector3D Session::makeScenePosition(const QVector3D &position, double altitude) const
{
    return QVector3D(position.x() - mMinX - mHalfX,
                     altitude - mMinAltitude,
                     -1.0 * (position.y() - mMinY - mHalfY));
}

QVector3D Session::makeScenePosition(const Spectrum &spec) const
{
    return makeScenePosition(spec.position, spec.coordinate.altitude());
}

QColor Session::makeDoserateColor(double doserate) const
{
    if(doserate == 0.0)
        return QColor(0, 255, 0);

    QColor color;
    auto minVal = mMinDoserate;
    auto maxVal = mMaxDoserate;

    if(mLogarithmicColorScale)
    {
        minVal = std::log(minVal);
        maxVal = std::log(maxVal);
        doserate = std::log(doserate);
    }

    auto f = (doserate - minVal) / (maxVal - minVal);
    auto a = (1.0 - f) / 0.25;	// invert and group
    auto x = std::floor(a);	// the integer part
    auto y = std::floor(255.0 * (a - x)); // the fractional part from 0 to 255

    switch((int)x)
    {
    case 0:
        color.setRgb(255, y, 0);
        break;
    case 1:
        color.setRgb(255 - y, 255, 0);
        break;
    case 2:
        color.setRgb(0, 255, y);
        break;
    case 3:
        color.setRgb(0, 255 - y, 255);
        break;
    case 4:
        color.setRgb(0, 0, 255);
        break;
    }

    return color;
}

QColor Session::makeDoserateColor(const Spectrum &spec) const
{
    return makeDoserateColor(spec.doserate());
}

} // namespace Gamma
