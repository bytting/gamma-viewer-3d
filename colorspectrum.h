#ifndef COLORSPECTRUM_H
#define COLORSPECTRUM_H

#include <QColor>

namespace Gamma
{

class ColorSpectrum
{
public:

    ColorSpectrum(double minValue, double maxValue, bool useLogarithmicScale = true);

    void setMinValue(double min) { mMinValue = min; }
    void setMaxValue(double max) { mMaxValue = max; }
    void setMinMaxValues(double min, double max);

    double minValue() const { return mMinValue; }
    double maxValue() const { return mMaxValue; }

    bool useLogarithmicScale() const { return mUseLogarithmicScale; }
    void setUseLogarithmicScale(bool state) { mUseLogarithmicScale = state; }

    QColor colorFromValue(double value);

private:

    double mMinValue = 0.0;
    double mMaxValue = 0.0;
    bool mUseLogarithmicScale;
};

} // namespace Gamma

#endif // COLORSPECTRUM_H
