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

#ifndef SPECTRUM_H
#define SPECTRUM_H

#include "exceptions.h"
#include "detector.h"
#include <vector>
#include <QString>
#include <QDateTime>
#include <QGeoCoordinate>

extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

namespace Gamma
{

class Spectrum
{
public:

    typedef std::vector<int> ChanList;
    typedef ChanList::size_type ChanListSize;

    explicit Spectrum(QString filename);
    virtual ~Spectrum() {}

    QString sessionName() const { return mSessionName; }
    int sessionIndex() const { return mSessionIndex; }

    QGeoCoordinate coordinates;

    QDateTime gpsTimeStart() const { return mGpsTimeStart; }
    int realtime() const { return mRealtime; }
    int livetime() const { return mLivetime; }

    ChanListSize numChannels() const { return mChannels.size(); }
    const ChanList& channels() const { return mChannels; }
    int channel(ChanListSize index) const;

    void calculateDoserate(const Detector &det, lua_State* L);
    double doserate() const { return mDoserate; }

    double x1() const { return mX1; }
    double y1() const { return mY1; }
    double z1() const { return mZ1; }

    struct InvalidSpectrumFile : public GA::Exception
    {
        explicit InvalidSpectrumFile(QString filename) noexcept
            : GA::Exception("Invalid spectrum file: " + filename) {}
    };

private:

    void loadFile(QString filename);

    QString mSessionName;
    int mSessionIndex;
    QDateTime mGpsTimeStart;
    int mRealtime;
    int mLivetime;
    ChanList mChannels;
    double mDoserate = 0.0;
    double mX1, mY1, mZ1;
};

} // namespace Gamma

#endif // SPECTRUM_H
