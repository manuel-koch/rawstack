#include "histfactory.h"
#include "workerbase.h"

#include <memory>
#include <vector>
#include <QDebug>


typedef std::pair<Magick::Color,unsigned long> GmHistPair;
typedef std::vector<GmHistPair>                GmHistVector;
typedef std::vector<double>                    DataVector;


class HistogramData
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

    HistogramData(Channel channel, size_t nofBins)
        : m_channel(channel)
        , m_max(0)
        , m_data(nofBins)
    {
    }

    virtual ~HistogramData() { }

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

    // Normalize histogram data to selected maximum
    void normalize( int maxNof )
    {
        m_max = 0;
        std::for_each( m_data.begin(), m_data.end(), [&](double &n)
        {
           n /= maxNof;
           m_max = std::max( m_max, n );
        });
    }

    // convert to list of histogram bins, normalized to 0.0 - 1.0
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


class RedData : public HistogramData
{
public:
    RedData(size_t nofBins) : HistogramData(HistogramData::RedChannel,nofBins)
    {
    }
    virtual ~RedData() { }
    virtual double value( const Magick::Color &c )
    {
        return c.scaleQuantumToDouble( c.redQuantum() );
    }
};


class GreenData : public HistogramData
{
public:
    GreenData(size_t nofBins) : HistogramData(HistogramData::GreenChannel,nofBins)
    {
    }
    virtual ~GreenData() { }
    virtual double value( const Magick::Color &c )
    {
        return c.scaleQuantumToDouble( c.greenQuantum() );
    }
};


class BlueData : public HistogramData
{
public:
    BlueData(size_t nofBins) : HistogramData(HistogramData::BlueChannel,nofBins)
    {
    }
    virtual ~BlueData() { }
    virtual double value( const Magick::Color &c )
    {
        return c.scaleQuantumToDouble( c.blueQuantum() );
    }
};


class LumData : public HistogramData
{
public:
    LumData(size_t nofBins) : HistogramData(HistogramData::LumChannel,nofBins)
    {
    }
    virtual ~LumData() { }
    virtual double value( const Magick::Color &c )
    {
        return c.scaleQuantumToDouble( c.intensity() );
    }
};


typedef std::unique_ptr<HistogramData> HistDataUPtr;
typedef std::vector<HistDataUPtr>      HistDataVector;


HistFactory::HistFactory(WorkerBase *worker, QObject *parent)
    : QObject(parent)
    , m_worker(worker)
    , m_nofBins(256)
{
    connect( m_worker, SIGNAL(cycleChanged(int)), this, SLOT(onCycleChanged(int)) );
}

void HistFactory::onCycleChanged(int cycle)
{
    Q_UNUSED(cycle);

    Magick::Image image = m_worker->gmimage();
    if( !image.isValid() )
    {
        qWarning() << "HistFactory::onCycleChanged() invalid input image" << m_worker;
        setRed( QVariantList() );
        setGreen( QVariantList() );
        setBlue( QVariantList() );
        setLum( QVariantList() );
        return;
    }

    qDebug() << "HistFactory::onCycleChanged() scale input image";
    image.scale( Magick::Geometry(600,600) );

    HistDataVector histograms;
    histograms.push_back( HistDataUPtr( new RedData(m_nofBins) ) );
    histograms.push_back( HistDataUPtr( new GreenData(m_nofBins) ) );
    histograms.push_back( HistDataUPtr( new BlueData(m_nofBins) ) );
    histograms.push_back( HistDataUPtr( new LumData(m_nofBins) ) );

    qDebug() << "HistFactory::onCycleChanged() create histogram data";
    GmHistVector histogram;
    if( image.isValid() )
        Magick::colorHistogram( &histogram, image );
    else
        qWarning() << "HistFactory::onCycleChanged() invalid image";

    qDebug() << "HistFactory::onCycleChanged() channel data of" << histogram.size() << "colors";
    std::for_each( histogram.begin(), histogram.end(), [&](const GmHistPair &p)
    {
        const Magick::Color &c = p.first;
        const unsigned long &n = p.second;
        std::for_each( histograms.begin(), histograms.end(), [&](HistDataUPtr &data)
        {
            data->add( c, n );
        });
    });

    qDebug() << "HistFactory::onCycleChanged() find maximum";
    double maxY = 0;
    std::for_each( histograms.begin(), histograms.end(), [&](HistDataUPtr &data)
    {
        maxY = std::max( maxY, data->max() );
    });
    qDebug() << "HistFactory::onCycleChanged() maximum" << maxY;

    qDebug() << "HistFactory::onCycleChanged() normalize";
    std::for_each( histograms.begin(), histograms.end(), [&](HistDataUPtr &data)
    {
        data->normalize( maxY );
        switch( data->channel() )
        {
            case HistogramData::RedChannel:   setRed( data->toList() );   break;
            case HistogramData::GreenChannel: setGreen( data->toList() ); break;
            case HistogramData::BlueChannel:  setBlue( data->toList() );  break;
            case HistogramData::LumChannel:   setLum( data->toList() );   break;
            default: break;
        }
    });

    qDebug() << "HistFactory::onCycleChanged() done";
}

void HistFactory::setRed(const QVariantList &red)
{
    if( red == m_red )
        return;
    m_red = red;
    emit redChanged( m_red );
}

void HistFactory::setGreen(const QVariantList &green)
{
    if( green == m_green )
        return;
    m_green = green;
    emit greenChanged( m_green );
}

void HistFactory::setBlue(const QVariantList &blue)
{
    if( blue == m_blue )
        return;
    m_blue = blue;
    emit blueChanged( m_blue );
}

void HistFactory::setLum(const QVariantList &lum)
{
    if( lum == m_lum )
        return;
    m_lum = lum;
    emit lumChanged( m_lum );
}
