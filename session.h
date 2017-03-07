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

#ifndef SESSION_H
#define SESSION_H

#include "spectrum.h"
#include "exceptions.h"
#include "detectortype.h"
#include "detector.h"
#include <memory>
#include <vector>
#include <QString>

extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

namespace gad
{

typedef std::vector<Spectrum*> SpecList;
typedef SpecList::size_type SpecListSize;

class Session
{
public:

    Session();
    ~Session();

    const SpecList& getSpectrumList() const;

    const Spectrum* getSpectrum(SpecListSize index) const;

    SpecListSize spectrumCount() const { return mSpecList.size(); }

    void loadPath(QString sessionPath);

    void loadDoserateScript(QString scriptFileName);

    void clear();

    double minDoserate() const { return mMinDoserate; }
    double maxDoserate() const { return mMaxDoserate; }

    double minX() const { return mMinX; }
    double maxX() const { return mMaxX; }
    double minY() const { return mMinY; }
    double maxY() const { return mMaxY; }
    double minZ() const { return mMinZ; }
    double maxZ() const { return mMaxZ; }

    double minAltitude() const { return mMinAltitude; }
    double maxAltitude() const { return mMaxAltitude; }

    struct DirIsNotASession : public GammanException
    {
        explicit DirIsNotASession(QString dir) noexcept
            : GammanException("Directory is not a valid session: " + dir) {}
    };

    struct InvalidSessionFile : public GammanException
    {
        explicit InvalidSessionFile(QString filename) noexcept
            : GammanException("Invalid session file: " + filename) {}
    };

    struct UnableToCreateLuaState : public GammanException
    {
        explicit UnableToCreateLuaState(QString source) noexcept
            : GammanException("Unable to create Lua state: " + source) {}
    };

private:

    void loadSessionFile(QString sessionFile);

    QString mName;
    QString mComment;
    double mLivetime = 0.0;
    int mIterations = 0;

    DetectorType mDetectorType;
    Detector mDetector;

    SpecList mSpecList;    

    lua_State *L = nullptr;
    bool mScriptLoaded = false;
    double mMinDoserate = 0.0;
    double mMaxDoserate = 0.0;
    double mMinX = 0.0, mMaxX = 0.0, mMinY = 0.0;
    double mMaxY = 0.0, mMinZ = 0.0, mMaxZ = 0.0;
    double mMinAltitude = 0.0, mMaxAltitude = 0.0;
};

} // namespace gad

#endif // SESSION_H
