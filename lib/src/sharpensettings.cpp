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
#include "sharpensettings.h"

const char *SharpenSettings::Name      = "sharpen";
const char *SharpenSettings::Radius    = "sharpen.radius";
const char *SharpenSettings::Sigma     = "sharpen.sigma";
const char *SharpenSettings::Amount    = "sharpen.amount";
const char *SharpenSettings::Threshold = "sharpen.threshold";

const double SharpenSettings::DefaultRadius    = 1.0;
const double SharpenSettings::DefaultSigma     = 1.0;
const double SharpenSettings::DefaultAmount    = 1.0;
const double SharpenSettings::DefaultThreshold = 0.0;
