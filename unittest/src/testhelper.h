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
#ifndef TESTHELPER_H_E13E9C78_4267_4B35_B301_7F62110AF6C1
#define TESTHELPER_H_E13E9C78_4267_4B35_B301_7F62110AF6C1
#include <ostream>
#include <QString>
#include <QStringList>
#include <QVariant>

// It's important that the << operator is defined in the SAME
// namespace that defines the class.  C++'s look-up rules rely on that.
QT_BEGIN_NAMESPACE

::std::ostream& operator<<(::std::ostream& os, const QStringList& l);
::std::ostream& operator<<(::std::ostream& os, const QVariant& v);

QT_END_NAMESPACE

#endif // TESTHELPER_H_E13E9C78_4267_4B35_B301_7F62110AF6C1
