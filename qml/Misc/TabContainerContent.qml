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
import QtQuick 2.5

Item {
    id: theContent

    /// @brief The title of tab
    property string title

    /// @brief Request selected expanded size of content when this tab is shown.
    ///        Using zero size will result in default size of tab container.
    property int expandedSize: 0

    anchors.fill: parent

    states: [
        State {
            name: "hidden"
            PropertyChanges {
                target:  theContent
                opacity: 0
                visible: false
            }
        },
        State {
            name: "visible"
            PropertyChanges {
                target:  theContent
                opacity: 1.0
                visible: true
            }
        }
    ]

    transitions: [
        Transition {
            from: "visible"
            to:   "hidden"
            FadeOutAnimation { target: theContent }
        },
        Transition {
            from: "hidden"
            to:   "visible"
            FadeInAnimation { target: theContent }
      }
    ]
}
