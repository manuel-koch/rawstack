#ifndef RAWSTACKAPPINFO_H
#define RAWSTACKAPPINFO_H

#include <QString>

class RawstackAppInfo
{
public:

    static const int VersionMajor = 1;
    static const int VersionMinor = 0;

    static QString version() { return QString("%1.%2").arg(VersionMajor).arg(VersionMinor); }

private:
    RawstackAppInfo() = delete;
};

#endif // RAWSTACKAPPINFO_H
