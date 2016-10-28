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
#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#include "imagefactoryworker.h"
#include "imagefactoryunderexposed.h"
#include "imagefactoryoverexposed.h"

#include <QObject>

class ImageFactory : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl         image        READ image        NOTIFY imageChanged)
    Q_PROPERTY(QUrl         underExposed READ underExposed NOTIFY underExposedChanged)
    Q_PROPERTY(QUrl         overExposed  READ overExposed  NOTIFY overExposedChanged)

public:
    explicit ImageFactory(WorkerBase *worker, QObject *parent = NULL);
    ~ImageFactory();

    QUrl image()        const { return m_image.url(); }
    QUrl underExposed() const { return m_underExposed.url(); }
    QUrl overExposed()  const { return m_overExposed.url(); }

signals:

    void imageChanged(QUrl url);
    void underExposedChanged(QUrl url);
    void overExposedChanged(QUrl url);

private:

    ImageFactoryWorker        m_image;
    ImageFactoryUnderExposed  m_underExposed;
    ImageFactoryOverExposed   m_overExposed;
};

#endif // IMAGEFACTORY_H
