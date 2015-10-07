#ifndef IMAGECACHEBASE_H
#define IMAGECACHEBASE_H

#include <string>
#include <Magick++.h>

class ImageCacheBase
{
public:

    ImageCacheBase() { }
    virtual ~ImageCacheBase() { }

    virtual Magick::Image retrieve( const std::string &key ) = 0;
    virtual void store( const std::string &key, Magick::Image img ) = 0;
};

#endif // IMAGECACHEBASE_H
