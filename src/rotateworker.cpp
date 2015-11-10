#include "rotateworker.h"
#include "rotatesettings.h"
#include "configdbentry.h"
#include "configsetting.h"

#include <math.h>

#include <QDebug>
#include <QThread>

// Given a original image that will be rotated by selected degrees,
// computes geometry of largest possible axis-aligned rectangle
// within the rotated image.
//
// Converted from Python implementation at
// http://tiku.io/questions/134849/rotate-image-and-crop-out-black-borders
// originally based on StackOverflow post at
// http://stackoverflow.com/questions/5789239/calculate-largest-rectangle-in-a-rotated-rectangle
Magick::Geometry largestRotatedGeometrie( const Magick::Image &img, double degree )
{
    double rad = degree * M_PI / 180.0;
    int quadrant = static_cast<int>( floor(rad / M_PI_2) ) & 3;
    double sign_alpha = (quadrant & 1) == 0 ? rad : M_PI - rad;
    double alpha = fmod( (fmod(sign_alpha, M_PI) + M_PI), M_PI );
    int w = img.size().width();
    int h = img.size().height();
    double bb_w = w * cos(alpha) + h * sin(alpha);
    double bb_h = w * sin(alpha) + h * cos(alpha);
    double gamma = w < h ? atan2(bb_w, bb_w) : atan2(bb_w, bb_w);
    double delta = M_PI - alpha - gamma;
    double length = w < h ? h : w;
    double d = length * cos(alpha);
    double a = d * sin(alpha) / sin(delta);
    double y = a * cos(gamma);
    double x = y * tan(gamma);
    double cw = bb_w - 2 * x;
    double ch = bb_h - 2 * y;
    return Magick::Geometry( cw, ch, x, y );
}

RotateWorker::RotateWorker()
    : WorkerBase("rotate")
{
    // EMPTY
}

RotateWorker::~RotateWorker()
{
    // EMPTY
}

void RotateWorker::registerSettingsImpl()
{
    hashSetting(RotateSettings::Degree);
}

void RotateWorker::developImpl(bool preview, WorkerBase *predecessor)
{
    Q_UNUSED(preview);

    qDebug() << "RotateWorker::developImpl()" << this << predecessor;

    double degree = config()->settings()->getSetting(RotateSettings::Degree)->value().toDouble();
    if( degree )
    {
        qDebug() << "RotateWorker::developImpl()" << this << degree;
        Magick::Geometry croppedGeomentry = largestRotatedGeometrie( m_img, degree );
        m_img.rotate( degree );
        setProgress( 0.5 );
        m_img.crop( croppedGeomentry );
    }
}
