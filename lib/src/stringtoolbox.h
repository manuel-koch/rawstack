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

    /// Convert given string to int, use default value for empty or invalid string
    static int toInt( QString str, int defValue )
    {
        if( str.isEmpty() )
            return defValue;
        bool ok;
        int v = str.toInt(&ok);
        if( ok )
            return v;
        return defValue;
    }

private:

    StringToolbox();
};

#endif // STRINGTOOLBOX_H
