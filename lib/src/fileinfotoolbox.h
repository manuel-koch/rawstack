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
#ifndef FILEINFOTOOLBOX_H
#define FILEINFOTOOLBOX_H

#include <QDebug>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QMimeType>

class FileInfoToolbox
{
public:

    static bool isRaw( const QFileInfo &path )
    {
        if( path.isDir() )
            return false;

        QMimeDatabase mime;
        QMimeType mimeType = mime.mimeTypeForFile(path);

        qDebug() << "FileInfoToolbox::isRaw(" << path.completeBaseName() << ")" << mimeType.name();

        QStringList validTypes;
        validTypes << "image/x-canon-cr2"
                   << "image/x-nikon-nef"
                   << "image/x-sony-arw"
                   << "image/x-panasonic-raw"
                   << "image/x-panasonic-raw2"
                   << "image/x-pentax-pef";
        return validTypes.indexOf(mimeType.name()) != -1;
    }
};

#endif // FILEINFOTOOLBOX_H

