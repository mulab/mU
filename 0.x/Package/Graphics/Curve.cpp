#include "Curve.h"

Curve::Curve() {
    lineStrips.clear();
    isColorfulCurve = false;
}

Curve::Curve(ArrayList<Point2d*> v) {
    lineStrips.clear();
    LineStrip* lineStrip = new LineStrip;
    for (unsigned int i = 0; i < v.size(); i++)
        lineStrip->vert.add(v[i]);
    lineStrips.add(lineStrip);
    isColorfulCurve = false;
}

Curve::~Curve() {
    for (unsigned int i = 0; i < lineStrips.size(); i++) {
        for (unsigned int j = 0; j < lineStrips[i]->vert.size(); j++) {
            delete lineStrips[i]->vert[j];
        }
        for (unsigned int j = 0; j < lineStrips[i]->color.size(); j++) {
            delete lineStrips[i]->color[j];
        }
        delete lineStrips[i];
    }
    lineStrips.clear();
}

Color* Curve::getRainbowColor(double x) {
    double red = 0;
    double green = 0;
    double blue = 0;
    if (x <= 63.0 / 255) {
        x = 4 * (x - 0);
        blue = 1;
        green = x;
    } else if (x <= 127.0 / 255) {
        x = 4 * (x - 64.0 / 255);
        green = 1;
        blue = 1 - x;
    } else if (x <= 191.0 / 255) {
        x = 4 * (x - 128.0 / 255);
        green = 1;
        red = x;
    } else {
        x = 4 * (x - 192.0 / 255);
        red = 1;
        green = 1 - x;
    }
    if (red > 1)
        red = 1;
    if (red < 0)
        red = 0;
    if (green > 1)
        green = 1;
    if (green < 0)
        green = 0;
    if (blue > 1)
        blue = 1;
    if (blue < 0)
        blue = 0;
    red *= 255;
    green *= 255;
    blue *= 255;
    return new Color(red, green, blue);
}