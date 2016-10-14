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
    Q_PROPERTY(MenuEntry* screenOverview   READ screenOverview   CONSTANT)
    Q_PROPERTY(MenuEntry* screenDevelop    READ screenDevelop    CONSTANT)
    Q_PROPERTY(MenuEntry* screenGridToggle READ screenGridToggle CONSTANT)
    Q_PROPERTY(MenuEntry* screenGridFive   READ screenGridFive   CONSTANT)
    Q_PROPERTY(MenuEntry* screenGridTen    READ screenGridTen    CONSTANT)
    Q_PROPERTY(MenuEntry* screenGridTwenty READ screenGridTwenty CONSTANT)
    Q_PROPERTY(MenuEntry* screenGridThirds READ screenGridThirds CONSTANT)
    Q_PROPERTY(MenuEntry* screenGridPhi    READ screenGridPhi    CONSTANT)
    Q_PROPERTY(MenuEntry* screenZoomIn     READ screenZoomIn     CONSTANT)
    Q_PROPERTY(MenuEntry* screenZoomOut    READ screenZoomOut    CONSTANT)
    Q_PROPERTY(MenuEntry* screenZoomFit    READ screenZoomFit    CONSTANT)
    Q_PROPERTY(MenuEntry* screenExposure   READ screenExposure   CONSTANT)
    Q_PROPERTY(MenuEntry* imgSettingsCopy  READ imgSettingsCopy  CONSTANT)
    Q_PROPERTY(MenuEntry* imgSettingsPaste READ imgSettingsPaste CONSTANT)

public:

    explicit MenuModel( QObject* parent = nullptr );

    static MenuModel *getInstance();
    static void setInstance(MenuModel*);

    MenuEntry* screenOverview()   const { return m_screenOverview; }
    MenuEntry* screenDevelop()    const { return m_screenDevelop; }

    MenuEntry* screenGridToggle() const { return m_screenGridToggle; }
    MenuEntry* screenGridFive()   const { return m_screenGridFive; }
    MenuEntry* screenGridTen()    const { return m_screenGridTen; }
    MenuEntry* screenGridTwenty() const { return m_screenGridTwenty; }
    MenuEntry* screenGridThirds() const { return m_screenGridThirds; }
    MenuEntry* screenGridPhi()    const { return m_screenGridPhi; }

    MenuEntry* screenZoomIn()     const { return m_screenZoomIn; }
    MenuEntry* screenZoomOut()    const { return m_screenZoomOut; }
    MenuEntry* screenZoomFit()    const { return m_screenZoomFit; }

    MenuEntry* screenExposure()   const { return m_screenExposure; }

    MenuEntry* imgSettingsCopy()   const { return m_imgSettingsCopy; }
    MenuEntry* imgSettingsPaste()  const { return m_imgSettingsPaste; }

private:

    static MenuModel*  ms_model;

    MenuEntry* m_screenOverview;
    MenuEntry* m_screenDevelop;

    MenuEntry* m_screenGridToggle;
    MenuEntry* m_screenGridFive;
    MenuEntry* m_screenGridTen;
    MenuEntry* m_screenGridTwenty;
    MenuEntry* m_screenGridThirds;
    MenuEntry* m_screenGridPhi;

    MenuEntry* m_screenZoomIn;
    MenuEntry* m_screenZoomOut;
    MenuEntry* m_screenZoomFit;

    MenuEntry* m_screenExposure;

    MenuEntry* m_imgSettingsCopy;
    MenuEntry* m_imgSettingsPaste;
};

#endif // MENUMODEL_H
