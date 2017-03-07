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

#ifndef DETECTORTYPE_H
#define DETECTORTYPE_H

#include <QString>
#include <QJsonObject>

namespace gad
{

class DetectorType
{
public:

    DetectorType() {}
    explicit DetectorType(const QJsonObject &obj);

    QString name() const { return mName; }
    int maxNumChannels() const { return mMaxNumChannels; }
    int minHV() const { return mMinHV; }
    int maxHV() const { return mMaxHV; }
    QString GEScript() const { return mGEScript; }

    void loadJson(const QJsonObject &obj);

private:

    QString mName;
    int mMaxNumChannels = 0;
    int mMinHV = 0;
    int mMaxHV = 0;
    QString mGEScript;
};

} // namespace gad

#endif // DETECTORTYPE_H
