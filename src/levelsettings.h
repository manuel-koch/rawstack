#ifndef LEVELSETTINGS_H
#define LEVELSETTINGS_H

class LevelSettings
{
public:

    static const char *Name;
    static const char *Blackpoint;
    static const char *Midpoint;
    static const char *Whitepoint;
    static const char *Brightness;
    static const char *Saturation;

    static const double DefaultBlackpoint;
    static const double DefaultMidpoint;
    static const double DefaultWhitepoint;
    static const double DefaultBrightness;
    static const double DefaultSaturation;
};

#endif // LEVELSETTINGS_H

