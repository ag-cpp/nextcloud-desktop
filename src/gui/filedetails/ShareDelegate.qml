/*
 * Copyright (C) 2022 by Claudio Cambra <claudio.cambra@nextcloud.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

import com.nextcloud.desktopclient 1.0
import Style 1.0
import "../tray"
import "../"

GridLayout {
    id: root

    signal deleteShare
    signal createNewLinkShare

    signal resetMenu
    signal resetPasswordField
    signal showPasswordSetError(string errorMessage);

    signal toggleAllowEditing(bool enable)
    signal toggleAllowResharing(bool enable)
    signal togglePasswordProtect(bool enable)
    signal toggleExpirationDate(bool enable)
    signal toggleNoteToRecipient(bool enable)

    signal setLinkShareLabel(string label)
    signal setExpireDate(var milliseconds) // Since QML ints are only 32 bits, use a variant
    signal setPassword(string password)
    signal setNote(string note)

    property int iconSize: 32
    property FileDetails fileDetails: FileDetails {}
    property StackView rootStackView: StackView {}

    property bool canCreateLinkShares: true

    readonly property bool isLinkShare: model.shareType === ShareModel.ShareTypeLink
    readonly property bool isPlaceholderLinkShare: model.shareType === ShareModel.ShareTypePlaceholderLink

    readonly property string text: model.display ?? ""
    readonly property string detailText: model.detailText ?? ""
    readonly property string iconUrl: model.iconUrl ?? ""
    readonly property string avatarUrl: model.avatarUrl ?? ""

    anchors.left: parent.left
    anchors.right: parent.right

    columns: 3
    rows: linkDetailLabel.visible ? 1 : 2

    columnSpacing: Style.standardSpacing / 2
    rowSpacing: Style.standardSpacing / 2

    Item {
        id: imageItem

        property bool isAvatar: root.avatarUrl !== ""

        Layout.row: 0
        Layout.column: 0
        Layout.rowSpan: root.rows
        Layout.preferredWidth: root.iconSize
        Layout.preferredHeight: root.iconSize

        Rectangle {
            id: backgroundOrMask
            anchors.fill: parent
            radius: width / 2
            color: Style.ncBlue
            visible: !imageItem.isAvatar
        }

        Image {
            id: shareIconOrThumbnail

            anchors.centerIn: parent

            verticalAlignment: Image.AlignVCenter
            horizontalAlignment: Image.AlignHCenter
            fillMode: Image.PreserveAspectFit

            source: imageItem.isAvatar ? root.avatarUrl : root.iconUrl + "/white"
            sourceSize.width: imageItem.isAvatar ? root.iconSize : root.iconSize / 2
            sourceSize.height: imageItem.isAvatar ? root.iconSize : root.iconSize / 2

            visible: !imageItem.isAvatar
        }

        OpacityMask {
            anchors.fill: parent
            source: shareIconOrThumbnail
            maskSource: backgroundOrMask
            visible: imageItem.isAvatar
        }
    }

    EnforcedPlainTextLabel {
        id: shareTypeLabel

        Layout.fillWidth: true
        Layout.alignment: linkDetailLabel.visible ? Qt.AlignBottom : Qt.AlignVCenter
        Layout.row: 0
        Layout.column: 1
        Layout.rowSpan: root.rows

        text: root.text
        color: Style.ncTextColor
        elide: Text.ElideRight
    }

    EnforcedPlainTextLabel {
        id: linkDetailLabel

        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        Layout.row: 1
        Layout.column: 1

        text: root.detailText
        color: Style.ncSecondaryTextColor
        elide: Text.ElideRight
        visible: text !== ""
    }

    RowLayout {        
        Layout.row: 0
        Layout.column: 2
        Layout.rowSpan: root.rows
        Layout.fillHeight: true

        spacing: 0

        CustomButton {
            id: createLinkButton

            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: Style.iconButtonWidth
            Layout.preferredHeight: width

            toolTipText: qsTr("Create a new share link")

            bgColor: Style.lightHover
            bgNormalOpacity: 0

            imageSource: "image://svgimage-custom-color/add.svg/" + Style.ncTextColor

            visible: root.isPlaceholderLinkShare && root.canCreateLinkShares
            enabled: visible

            onClicked: root.createNewLinkShare()
        }

        CustomButton {
            id: copyLinkButton

            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: Style.iconButtonWidth
            Layout.preferredHeight: width

            toolTipText: qsTr("Copy share link location")

            bgColor: Style.lightHover
            bgNormalOpacity: 0

            imageSource: "image://svgimage-custom-color/copy.svg/" + Style.ncTextColor
            icon.width: 16
            icon.height: 16

            visible: root.isLinkShare
            enabled: visible

            onClicked: {
                clipboardHelper.text = root.link;
                clipboardHelper.selectAll();
                clipboardHelper.copy();
                clipboardHelper.clear();
            }

            TextEdit { id: clipboardHelper; visible: false}
        }

        CustomButton {
            id: moreButton

            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: Style.iconButtonWidth
            Layout.preferredHeight: width

            toolTipText: qsTr("Share options")

            bgColor: Style.lightHover
            bgNormalOpacity: 0

            imageSource: "image://svgimage-custom-color/more.svg/" + Style.ncTextColor

            visible: !root.isPlaceholderLinkShare
            enabled: visible

            onClicked: root.rootStackView.push(shareDetailsPageComponent, {}, StackView.PushTransition)

            Component {
                id: shareDetailsPageComponent
                ShareDetailsPage {
                    id: shareDetailsPage

                    width: parent.width
                    height: parent.height

                    fileDetails: root.fileDetails
                    shareModelData: model

                    canCreateLinkShares: root.canCreateLinkShares

                    onCloseShareDetails: root.rootStackView.pop(root.rootStackView.initialItem, StackView.PopTransition)

                    onToggleAllowEditing: root.toggleAllowEditing(enable)
                    onToggleAllowResharing: root.toggleAllowResharing(enable)
                    onTogglePasswordProtect: root.togglePasswordProtect(enable)
                    onToggleExpirationDate: root.toggleExpirationDate(enable)
                    onToggleNoteToRecipient: root.toggleNoteToRecipient(enable)

                    onSetLinkShareLabel: root.setLinkShareLabel(label)
                    onSetExpireDate: root.setExpireDate(milliseconds) // Since QML ints are only 32 bits, use a variant
                    onSetPassword: root.setPassword(password)
                    onSetNote: root.setNote(note)

                    onDeleteShare: {
                        root.deleteShare();
                        closeShareDetails();
                    }
                    onCreateNewLinkShare: {
                        root.createNewLinkShare();
                        closeShareDetails();
                    }

                    Connections {
                        target: root
                        function onResetMenu() { shareDetailsPage.resetMenu() }
                        function onResetPasswordField() { shareDetailsPage.resetPasswordField() }
                        function onShowPasswordSetError(errorMessage) { shareDetailsPage.showPasswordSetError(errorMessage) }
                    }
                }
            }
        }
    }
}
