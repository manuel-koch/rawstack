rawstack
========

Rawstack uses [ufraw](http://ufraw.sourceforge.net/) to extract multiple differently exposed images from a single RAW camera image.
It uses [GraphicsMagick](http://www.graphicsmagick.org) to manipulate the intermediate images.
The extracted images are then blended into one single output image using [enfuse](http://enblend.sourceforge.net/).

This creates kind of Poor-Men-HDR image from a stack of input images that are extracted from a single RAW camera image.

Rawstack is utilizing the Qt application framework incl. QML to build the GUI application.

Remarks
-------

Current implementation using some hardcoced paths to ufraw and enfuse executables that will only match an environment on Mac OSX using [MacPorts](https://www.macports.org).