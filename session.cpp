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
#include <stdexcept>
#include <memory>
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
{
    L = luaL_newstate();
    if(!L)
        throw UnableToCreateLuaState("Session::Session");
    luaL_openlibs(L);
}

Session::~Session()
{
    try
    {
        clear();
        lua_close(L);
    }
    catch(const std::exception& e)
    {
        qDebug() << e.what();
    }
}

const Spectrum* Session::getSpectrum(SpecListSize index) const
{
    if(index >= mSpecList.size())
        throw GA::IndexOutOfBounds("Session::getSpectrum");

    return mSpecList[index];
}

const SpecList& Session::getSpectrumList() const
{
    return mSpecList;
}

void Session::loadPath(QString sessionPath)
{
    const QDir sessionDir(sessionPath);
    if(!sessionDir.exists())
        throw GA::DirDoesNotExist(sessionDir.absolutePath());

    const QDir spectrumDir(sessionPath + QDir::separator() +
                     QStringLiteral("json"));
    if (!spectrumDir.exists())
        throw DirIsNotASession(spectrumDir.absolutePath());

    const auto sessionFile = sessionPath + QDir::separator() +
            QStringLiteral("session.json");
    if(!QFile::exists(sessionFile))
        throw DirIsNotASession(sessionDir.absolutePath());

    clear();

    loadSessionFile(sessionFile);

    const auto entryInfoList = spectrumDir.entryInfoList(
                QStringList() << "*.json",
                QDir::NoDotAndDotDot | QDir::Files);

    bool first = true;
    for(const auto& info : entryInfoList)
    {
        try
        {
            auto spec = new Spectrum(info.absoluteFilePath());

            if(mScriptLoaded)
                spec->calculateDoserate(mDetector, L);

            /*double minX = std::min(spec->x1(), spec->x2());
            double minY = std::min(spec->y1(), spec->y2());
            double minZ = std::min(spec->z1(), spec->z2());

            double maxX = std::max(spec->x1(), spec->x2());
            double maxY = std::max(spec->y1(), spec->y2());
            double maxZ = std::max(spec->z1(), spec->z2());*/

            /*double minAltitude = std::min(
                        spec->altitudeStart(),
                        spec->altitudeEnd());
            double maxAltitude = std::max(
                        spec->altitudeStart(),
                        spec->altitudeEnd());*/

            if(first)
            {
                mMinDoserate = spec->doserate();
                mMaxDoserate = spec->doserate();

                /*mMinX = minX;
                mMinY = minY;
                mMinZ = minZ;

                mMaxX = maxX;
                mMaxY = maxY;
                mMaxZ = maxZ;*/

                mMinX = mMaxX = spec->x1();
                mMinY = mMaxY = spec->y1();
                mMinZ = mMaxZ = spec->z1();

                /*mMinAltitude = minAltitude;
                mMaxAltitude = maxAltitude;*/

                mMinAltitude = mMaxAltitude = spec->coordinates.altitude();

                first = false;
            }
            else
            {
                if(spec->doserate() < mMinDoserate)
                    mMinDoserate = spec->doserate();
                if(spec->doserate() > mMaxDoserate)
                    mMaxDoserate = spec->doserate();

                /*if(minX < mMinX)
                    mMinX = minX;
                if(minY < mMinY)
                    mMinY = minY;
                if(minZ < mMinZ)
                    mMinZ = minZ;

                if(maxX > mMaxX)
                    mMaxX = maxX;
                if(maxY > mMaxY)
                    mMaxY = maxY;
                if(maxZ > mMaxZ)
                    mMaxZ = maxZ;*/

                if(mMinX > spec->x1())
                    mMinX = spec->x1();
                if(mMinY > spec->y1())
                    mMinY = spec->y1();
                if(mMinZ > spec->z1())
                    mMinZ = spec->z1();

                if(mMaxX < spec->x1())
                    mMaxX = spec->x1();
                if(mMaxY < spec->y1())
                    mMaxY = spec->y1();
                if(mMaxZ < spec->z1())
                    mMaxZ = spec->z1();

                /*if(minAltitude < mMinAltitude)
                    mMinAltitude = minAltitude;
                if(maxAltitude > mMaxAltitude)
                    mMaxAltitude = maxAltitude;*/

                if(mMinAltitude > spec->coordinates.altitude())
                    mMinAltitude = spec->coordinates.altitude();
                if(mMaxAltitude < spec->coordinates.altitude())
                    mMaxAltitude = spec->coordinates.altitude();
            }

            mSpecList.push_back(spec);
        }
        catch(const GA::Exception& e)
        {
            qDebug() << e.what();
        }
    }
}

void Session::loadSessionFile(QString sessionFile)
{
    QFile jsonFile(sessionFile);
    if(!jsonFile.open(QFile::ReadOnly))
        throw GA::UnableToLoadFile(sessionFile);

    auto doc = QJsonDocument().fromJson(jsonFile.readAll());
    if(!doc.isObject())
        throw InvalidSessionFile(sessionFile);

    auto root = doc.object();

    if(!root.contains("Name"))
        throw GA::MissingJsonValue("Session:Name");
    mName = root.value("Name").toString();

    if(!root.contains("Comment"))
        throw GA::MissingJsonValue("Session:Comment");
    mComment = root.value("Comment").toString();

    if(!root.contains("Livetime"))
        throw GA::MissingJsonValue("Session:Livetime");
    mLivetime = root.value("Livetime").toInt();

    if(!root.contains("Iterations"))
        throw GA::MissingJsonValue("Session:Iterations");
    mIterations = root.value("Iterations").toInt();

    if(!root.contains("DetectorType"))
        throw GA::MissingJsonValue("Session:DetectorType");
    mDetectorType.loadJson(root.value("DetectorType").toObject());

    if(!root.contains("Detector"))
        throw GA::MissingJsonValue("Session:Detector");
    mDetector.loadJson(root.value("Detector").toObject());
}

void Session::loadDoserateScript(QString scriptFileName)
{
    if(luaL_dofile(L, scriptFileName.toStdString().c_str()))
        qDebug() << "luaL_dofile failed";
    else mScriptLoaded = true;
}

void Session::clear()
{
    for(auto spec : mSpecList)
        delete spec;

    mSpecList.clear();

    mMinDoserate = mMaxDoserate = 0.0;
    mMinX = mMinY = mMinZ = 0.0;
    mMaxX = mMaxY = mMaxZ = 0.0;
    mMinAltitude = mMaxAltitude = 0.0;
}

} // namespace Gamma
