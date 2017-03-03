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

#ifndef DETECTOR_H
#define DETECTOR_H

#include <QString>
#include <QJsonObject>

namespace gamma
{

typedef std::vector<double> CoefficientList;
typedef CoefficientList::size_type CoefficientListSize;

class Detector
{
public:

    Detector() {}
    explicit Detector(const QJsonObject &obj);

    QString typeName() const { return mTypeName; }
    int HV() const { return mHV; }
    int numChannels() const { return mNumChannels; }
    QString serialnumber() const { return mSerialnumber; }
    double coarseGain() const { return mCoarseGain; }
    double fineGain() const { return mFineGain; }
    int livetime() const { return mLivetime; }
    int LLD() const { return mLLD; }
    int ULD() const { return mULD; }
    const CoefficientList& energyCurveCoefficients() const
    {
        return mEnergyCurveCoefficients;
    }

    void loadJson(const QJsonObject &obj);

    double getEnergy(int index);

private:

    QString mTypeName;
    int mHV;
    int mNumChannels;
    QString mSerialnumber;
    double mCoarseGain, mFineGain;
    int mLivetime;
    int mLLD, mULD;
    std::vector<double> mEnergyCurveCoefficients;
};

} // namespace gamma

#endif // DETECTOR_H