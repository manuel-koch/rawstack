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
#include "ufrawsettings.h"

const char *UfrawSettings::Name          = "ufraw";
const char *UfrawSettings::Fuse          = "ufraw.fuse.nof";
const char *UfrawSettings::ExposureSigma = "ufraw.fuse.exposuresigma";
const char *UfrawSettings::Exposure      = "ufraw.exposure";
const char *UfrawSettings::WbTemperature = "ufraw.wb.temperature";
const char *UfrawSettings::WbGreen       = "ufraw.wb.green";

const int    UfrawSettings::DefaultFuse          = 9;
const double UfrawSettings::DefaultExposureSigma = 0.1;
const double UfrawSettings::DefaultExposure      = 1.0;
const int    UfrawSettings::DefaultWbTemperature = 0;
const double UfrawSettings::DefaultWbGreen       = 0.0;
