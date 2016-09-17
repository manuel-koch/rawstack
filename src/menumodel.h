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
#ifndef MENUMODEL_H
#define MENUMODEL_H

#include <QObject>

class MenuEntry;

class MenuModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MenuEntry* screenOverview READ screenOverview CONSTANT)
    Q_PROPERTY(MenuEntry* screenDevelop  READ screenDevelop  CONSTANT)

public:

    explicit MenuModel( QObject* parent = nullptr );

    static MenuModel *getInstance();
    static void setInstance(MenuModel*);

    MenuEntry* screenOverview() const { return m_screenOverview; }
    MenuEntry* screenDevelop() const { return m_screenDevelop; }

private:

    static MenuModel*  ms_model;

    MenuEntry* m_screenOverview;
    MenuEntry* m_screenDevelop;
};

#endif // MENUMODEL_H
