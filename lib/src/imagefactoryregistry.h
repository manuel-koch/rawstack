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
#ifndef IMAGEFACTORYREGISTRY_H
#define IMAGEFACTORYREGISTRY_H

#include <QObject>
#include <QMap>

class ImageFactoryBase;

class ImageFactoryRegistry : public QObject
{
    Q_OBJECT

public:

    explicit ImageFactoryRegistry(QObject *parent = NULL);
    ~ImageFactoryRegistry();

    static ImageFactoryRegistry *getInstance() { return ms_instance; }
    static void setInstance( ImageFactoryRegistry *instance ) { ms_instance = instance; }

    void registerFactory(ImageFactoryBase *factory);
    void unregisterFactory(ImageFactoryBase *factory);

    ImageFactoryBase *getFactory(QString id);

private:

    static ImageFactoryRegistry *ms_instance;

    QMap<QString,ImageFactoryBase*> m_factories;
};

#endif // IMAGEFACTORYREGISTRY_H
