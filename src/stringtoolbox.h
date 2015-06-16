#ifndef STRINGTOOLBOX_H
#define STRINGTOOLBOX_H

#include <QObject>

class StringToolbox
{
public:

    /// Convert given string to boolean, use default value for empty string
    static bool toBool( QString str, bool defValue )
    {
        if( str.isEmpty() )
            return defValue;
        str = str.toLower();
        return str == "true" || str == "yes" || str.toInt();
    }

    /// Convert given string to double, use default value for empty or invalid string
    static double toDouble( QString str, double defValue )
    {
        if( str.isEmpty() )
            return defValue;
        bool ok;
        double v = str.toDouble(&ok);
        if( ok )
            return v;
        return defValue;
    }

private:

    StringToolbox();
};

#endif // STRINGTOOLBOX_H
