//  gamma-viewer-3d - 3d visualization of sessions generated by gamma-analyzer
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

#include "spectrum.h"
#include "detector.h"

namespace Gamma
{

Spectrum::Spectrum(const QSqlQuery &query)
{
    loadQuery(query);
}

int Spectrum::channel(ChannelListSize index) const
{
    if(index >= mChannels.size())
        throw Exception_IndexOutOfBounds("Spectrum::channel");

    return mChannels[index];
}

void Spectrum::loadQuery(const QSqlQuery &query)
{
    int idSessionName = query.record().indexOf("session_name");
    int idSessionIndex = query.record().indexOf("session_index");
    int idRealtime = query.record().indexOf("realtime");
    int idLivetime = query.record().indexOf("livetime");
    int idLatitude = query.record().indexOf("latitude");
    int idLongitude = query.record().indexOf("longitude");
    int idAltitude = query.record().indexOf("altitude");
    int idStartTime = query.record().indexOf("start_time");
    int idChannels = query.record().indexOf("channels");

    mSessionName = query.value(idSessionName).toString();
    mSessionIndex = query.value(idSessionIndex).toInt();
    mRealtime = query.value(idRealtime).toInt();
    mLivetime = query.value(idLivetime).toInt();
    coordinate.setLatitude(query.value(idLatitude).toDouble());
    coordinate.setLongitude(query.value(idLongitude).toDouble());
    coordinate.setAltitude(query.value(idAltitude).toDouble());
    mGpsTimeStart = QDateTime::fromString(
                query.value(idStartTime).toString(),
                Qt::DateFormat::ISODate);

    auto strChans = query.value(idChannels).toString();
    auto strChanList = strChans.split(
                ' ', QString::SplitBehavior::SkipEmptyParts);

    mChannels.clear();

    for(const auto &chan : strChanList)
        mChannels.emplace_back(chan.toInt());

    position = Geo::geodeticToCartesian(coordinate);
}

static double GEValue(lua_State *L, double energy)
{
    double ge;

    lua_getglobal(L, "gevalue");
    lua_pushnumber(L, energy);
    lua_call(L, 1, 1);
    ge = (double)lua_tonumber(L, -1);
    lua_pop(L, 1);

    return ge;
}

void Spectrum::calculateDoserate(const Detector &detector, lua_State *L)
{
    mDoserate = 0.0;

    // Trim off discriminators
    int startChan = (int)((double)detector.numChannels() *
                          ((double)detector.LLD() / 100.0));
    int endChan = (int)((double)detector.numChannels() *
                        ((double)detector.ULD() / 100.0));
    if(endChan > detector.numChannels()) // FIXME: Can not exceed 100% atm
        endChan = detector.numChannels();

    // Accumulate doserates from each channel
    for (int i = startChan; i < endChan; i++)
    {
        double sec = (double)mLivetime / 1000000.0;
        double cps = mChannels[i] / sec;
        double E = detector.getEnergy(i);
        if (E < 0.05) // Energies below 0.05 are invalid
            continue;
        double GE = GEValue(L, E / 1000.0);
        double chanDose = GE * cps * 60.0;
        mDoserate += chanDose;
    }
}

} // namespace Gamma
