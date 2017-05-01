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
 * Copyright 2017 Manuel Koch
 */
#include "testhelper.h"

#include <QString>
#include <QStringList>
#include <QVariant>

// It's important that the << operator is defined in the SAME
// namespace that defines the class.  C++'s look-up rules rely on that.
QT_BEGIN_NAMESPACE

::std::ostream& operator<<(::std::ostream& os, const QStringList& l)
{
    int i = 0;
    for( const QString &s : l )
    {
        os << "\"" << s.toStdString() << "\"";
        if( i+1 < l.size() )
            os << " ";
        i++;
    }
    return os;
}

::std::ostream& operator<<(::std::ostream& os, const QVariant& v)
{
    return os << v.toString().toStdString();
}

QT_END_NAMESPACE
