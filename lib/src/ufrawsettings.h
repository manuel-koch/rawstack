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
#ifndef UFRAWSETTINGS_H
#define UFRAWSETTINGS_H

class UfrawSettings
{
public:

    static const char *Name;
    static const char *Fuse;
    static const char *ExposureSigma;
    static const char *Exposure;
    static const char *WbTemperature;
    static const char *WbGreen;

    static const int    DefaultFuse;
    static const double DefaultExposureSigma;
    static const double DefaultExposure;
    static const int    DefaultWbTemperature;
    static const double DefaultWbGreen;
};

#endif // UFRAWSETTINGS_H

