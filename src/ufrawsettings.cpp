#include "ufrawsettings.h"

const char *UfrawSettings::Name          = "ufraw";
const char *UfrawSettings::Fuse          = "ufraw.fuse";
const char *UfrawSettings::Exposure      = "ufraw.exposure";
const char *UfrawSettings::WbTemperature = "ufraw.wb.temperature";
const char *UfrawSettings::WbGreen       = "ufraw.wb.green";

const int    UfrawSettings::DefaultFuse          = 5;
const double UfrawSettings::DefaultExposure      = 1.0;
const int    UfrawSettings::DefaultWbTemperature = 0;
const double UfrawSettings::DefaultWbGreen       = 0.0;
