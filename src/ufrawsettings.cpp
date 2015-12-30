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
