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
#ifndef IMAGEFACTORYBASE_H
#define IMAGEFACTORYBASE_H

#include <QObject>
#include <QUrl>
#include <QImage>

class WorkerBase;

class ImageFactoryBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url NOTIFY urlChanged)

public:

    explicit ImageFactoryBase();
    virtual ~ImageFactoryBase();

    QString id() const { return m_id; }
    QUrl    url() const { return m_url; }
    virtual QImage image();

signals:

    void urlChanged(QUrl url);

protected:

    void setUrl(int hash);
    void setUrl(const QByteArray &hash);

private:

    QString    m_id;
    QUrl       m_url;
};

#endif // IMAGEFACTORYBASE_H
