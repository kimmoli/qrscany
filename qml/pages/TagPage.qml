import QtQuick 2.0
import Sailfish.Silica 1.0


Dialog
{
    id: page

    property string qrtag: "Not found"

    function isHttpLink(url)
    {
        if (url.match("^https?://"))
        {
            return true;
        }
        return false;
    }

    onDone:
    {
        if (result === DialogResult.Accepted)
        {
            qrtag = textArea.text
        }
    }

    SilicaFlickable
    {
        anchors.fill: parent
        PullDownMenu
        {
            visible: isHttpLink(qrtag)
            MenuItem
            {
                text: "Open URL"
                visible: isHttpLink(qrtag)
                onClicked:
                {
                    Qt.openUrlExternally(qrtag);
                }
            }
        }

        DialogHeader
        {
            id: pageHeader
            title: "tag found"
            acceptText: "Send over UDP"
            cancelText: "Scan new"
        }

        TextArea
        {
            id: textArea
            anchors.top: pageHeader.bottom
            anchors.topMargin: Theme.paddingLarge
            anchors.left: parent.left
            anchors.leftMargin: Theme.paddingSmall
            anchors.right: parent.right
            anchors.rightMargin: Theme.paddingSmall
            anchors.bottom: parent.bottom
            anchors.bottomMargin: Theme.paddingLarge
            text: qrtag
        }
    }
}
