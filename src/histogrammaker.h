#ifndef HISTOGRAMMAKER_H
#define HISTOGRAMMAKER_H

#include "histogramdata.h"

#include <Magick++.h>

#include <QObject>

class HistogramMaker : public HistogramData
{
    Q_OBJECT

public:

    explicit HistogramMaker(QObject *parent = NULL);

    void analyze(Magick::Image image);

private:

    size_t m_nofBins;
};

#endif // HISTOGRAMMAKER_H
