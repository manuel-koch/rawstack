/*
 * This file is part of Rawstack.
 *
 * Rawstack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rawstack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rawstack. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Manuel Koch
 */
#include "histogrammaker.h"

#include <QDebug>
#include <QElapsedTimer>


typedef std::pair<Magick::Color,unsigned long> GmHistPair;
typedef std::vector<GmHistPair>                GmHistVector;
typedef std::vector<double>                    DataVector;


class Data
{
public:
    enum Channel
    {
        NoChannel    = 0x00,
        RedChannel   = 0x01,
        GreenChannel = 0x02,
        BlueChannel  = 0x04,
        LumChannel   = 0x08,
    };

    Data(Channel channel, size_t nofBins)
        : m_channel(channel)
        , m_max(0)
        , m_data(nofBins)
    {
    }

    virtual ~Data() { }

    Channel channel() const { return m_channel; }

    double max() const { return m_max; }

    // Add color / count data to histogram
    void add( const Magick::Color &c, int nof )
    {
        double v = std::max( 0.0, std::min( 1.0, value(c) ) );
        int  idx = static_cast<int>( (m_data.size()-1) * v + 0.5 );
        m_data[idx] += nof;
        m_max = std::max( m_max, m_data[idx] );
    }

    // Normalize histogram data ( 0.0 - 1.0 ) using selected maximum
    void normalize( int maxNof )
    {
        m_max = 0;
        std::for_each( m_data.begin(), m_data.end(), [&](double &n)
        {
           n /= maxNof;
           m_max = std::max( m_max, n );
        });
    }

    // convert to list of histogram bins
    QVariantList toList() const
    {
        QVariantList l;
        std::for_each( m_data.begin(), m_data.end(), [&](const double &n)
        {
           l.push_back( n );
        });
        return l;
    }

    // Dump
    void dump()
    {
        qDebug() << "channel" << m_channel << "max" << m_max;
        for( size_t i=0; i<m_data.size(); i++ )
        {
           qDebug() << i << m_data[i];
        }
    }

    // Get value ( 0.0 ... 1.0 ) for selected color
    virtual double value( const Magick::Color &c )
    {
        Q_UNUSED(c);
        return 0;
    }

private:

    Channel     m_channel;
    double      m_max;
    DataVector  m_data;
};


class RedData : public Data
{
public:
    RedData(size_t nofBins) : Data(Data::RedChannel,nofBins)
    {
    }
    virtual ~RedData() { }
    virtual double value( const Magick::Color &c )
    {
        return c.scaleQuantumToDouble( c.redQuantum() );
    }
};


class GreenData : public Data
{
public:
    GreenData(size_t nofBins) : Data(Data::GreenChannel,nofBins)
    {
    }
    virtual ~GreenData() { }
    virtual double value( const Magick::Color &c )
    {
        return c.scaleQuantumToDouble( c.greenQuantum() );
    }
};


class BlueData : public Data
{
public:
    BlueData(size_t nofBins) : Data(Data::BlueChannel,nofBins)
    {
    }
    virtual ~BlueData() { }
    virtual double value( const Magick::Color &c )
    {
        return c.scaleQuantumToDouble( c.blueQuantum() );
    }
};


class LumData : public Data
{
public:
    LumData(size_t nofBins) : Data(Data::LumChannel,nofBins)
    {
    }
    virtual ~LumData() { }
    virtual double value( const Magick::Color &c )
    {
        return c.scaleQuantumToDouble( c.intensity() );
    }
};


typedef std::unique_ptr<Data>      HistDataUPtr;
typedef std::vector<HistDataUPtr>  HistDataVector;


HistogramMaker::HistogramMaker(QObject *parent)
    : HistogramData(parent)
    , m_nofBins(256)
{
    // EMPTY
}

void HistogramMaker::analyze(Magick::Image image)
{
    qDebug() << "HistogramMaker::analyze()";

    QElapsedTimer elapsed;
    elapsed.start();

    if( !image.isValid() )
    {
        qWarning() << "HistogramMaker::analyze() invalid input image";
        setRed( QVariantList() );
        setGreen( QVariantList() );
        setBlue( QVariantList() );
        setLum( QVariantList() );
        return;
    }

    qDebug() << "HistogramMaker::analyze() scaling" << image.size().width() << "x" << image.size().height();
    image.scale( "400>" ); // scale with same aspect ratio
    qDebug() << "HistogramMaker::analyze() scaled to" << image.size().width() << "x" << image.size().height();

    HistDataVector histograms;
    histograms.push_back( HistDataUPtr( new RedData(m_nofBins) ) );
    histograms.push_back( HistDataUPtr( new GreenData(m_nofBins) ) );
    histograms.push_back( HistDataUPtr( new BlueData(m_nofBins) ) );
    histograms.push_back( HistDataUPtr( new LumData(m_nofBins) ) );

    qDebug() << "HistogramMaker::analyze() create histogram data";
    GmHistVector histogram;
    Magick::colorHistogram( &histogram, image );

    qDebug() << "HistogramMaker::analyze() channel data of" << histogram.size() << "colors";
    std::for_each( histogram.begin(), histogram.end(), [&](const GmHistPair &p)
    {
        const Magick::Color &c = p.first;
        const unsigned long &n = p.second;
        std::for_each( histograms.begin(), histograms.end(), [&](HistDataUPtr &data)
        {
            data->add( c, n );
        });
    });

    qDebug() << "HistogramMaker::analyze() find maximum";
    double maxY = 0;
    std::for_each( histograms.begin(), histograms.end(), [&](HistDataUPtr &data)
    {
        maxY = std::max( maxY, data->max() );
    });
    qDebug() << "HistogramMaker::analyze() maximum" << maxY;

    qDebug() << "HistogramMaker::analyze() normalize";
    std::for_each( histograms.begin(), histograms.end(), [&](HistDataUPtr &data)
    {
        data->normalize( maxY );
        switch( data->channel() )
        {
            case Data::RedChannel:   setRed( data->toList() );   break;
            case Data::GreenChannel: setGreen( data->toList() ); break;
            case Data::BlueChannel:  setBlue( data->toList() );  break;
            case Data::LumChannel:   setLum( data->toList() );   break;
            default: break;
        }
    });

    qDebug() << "HistogramMaker::analyze() done in" << elapsed.elapsed() << "ms";
}

