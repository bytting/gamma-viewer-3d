#include "colorspectrum.h"
#include "exceptions.h"
#include <cmath>

namespace Gamma
{

ColorSpectrum::ColorSpectrum(double minValue, double maxValue, bool useLogarithmicScale)
{
    setMinMaxValues(minValue, maxValue);
    setUseLogarithmicScale(useLogarithmicScale);
}

void ColorSpectrum::setMinMaxValues(double minValue, double maxValue)
{
    if(minValue > maxValue)
        throw NumericRangeError("ColorSpectrum::setMinMaxValues: minValue > maxValue");

    setMinValue(minValue);
    setMaxValue(maxValue);
}

QColor ColorSpectrum::colorFromValue(double value)
{
    if(value == 0.0)
        return QColor(0, 255, 0);

    QColor color;
    double minVal = mMinValue;
    double maxVal = mMaxValue;

    if(mUseLogarithmicScale)
    {
        minVal = std::log(minVal);
        maxVal = std::log(maxVal);
        value = std::log(value);
    }

    double f = (value - minVal) / (maxVal - minVal);

    auto a = (1.0 - f) / 0.25;	// invert and group
    auto X = std::floor(a);	// the integer part
    auto Y = std::floor(255.0 * (a - X)); // the fractional part from 0 to 255

    switch((int)X)
    {
    case 0:
        color.setRgb(255, Y, 0);
        break;
    case 1:
        color.setRgb(255 - Y, 255, 0);
        break;
    case 2:
        color.setRgb(0, 255, Y);
        break;
    case 3:
        color.setRgb(0, 255 - Y, 255);
        break;
    case 4:
        color.setRgb(0, 0, 255);
        break;
    }

    return color;
}

} // namespace Gamma
