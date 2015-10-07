#ifndef ROTATECONFIG_H
#define ROTATECONFIG_H

#include "configbase.h"

class RotateConfig : public ConfigBase
{
    Q_OBJECT
    Q_PROPERTY(double degree READ degree WRITE setDegree NOTIFY degreeChanged)

public:

    static const char *Name;
    constexpr static const double DefaultDegree = 0.0;

    explicit RotateConfig( QObject *parent = NULL );
    virtual ~RotateConfig();

    // ConfigBase interface
    virtual QDomNode toXML(QDomNode &node ) const override;
    virtual bool fromXML( QDomNode const &node ) override;
    virtual QByteArray hash( const QByteArray &baseHash = QByteArray() ) override;

    double degree() const { return m_degree; }

public slots:

    void setDegree(double degree);

signals:

    void degreeChanged(double degree);

private:

    double m_degree;
};

#endif // ROTATECONFIG_H
