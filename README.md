Rawstack
========

Rawstack creates kind of Poor-Men-HDR image from a stack of input images that are extracted from a single RAW camera image.

It uses...
- [GraphicsMagick](http://www.graphicsmagick.org) to manipulate images.
- [Exiv2](http://www.exiv2.org/) to extract EXIF information from camera RAW image.
- [ufraw](http://ufraw.sourceforge.net/) to extract multiple differently exposed images from a single RAW camera image.
- [enfuse](http://enblend.sourceforge.net/) to blend up to 9 extracted images into one single output image.
- [Qt](http://www.qt.io) to build the GUI application.

Remarks
-------

Current implementation using some hardcoced paths to ufraw and enfuse executables that will only match an environment on Mac OSX using e.g. [MacPorts](http://www.macports.org).

I currently only build rawstack on Mac OSX, thus it may not build/run on other environments.
