import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0
import QZXing 2.3
import Sailfish.Media 1.0
import UdpIf 1.0

Page
{
    id: page

    property string imagePath: dataPath + "/" + "tempqr.jpg"

    property QZXing barcode: QZXing
    {
        onDecodingFinished:
        {
            console.log("decoding finished");
        }

        onTagFound:
        {
            console.log(tag);
            var dialog = pageStack.push(Qt.resolvedUrl("TagPage.qml"), { qrtag: tag });
            dialog.accepted.connect(function()
            {
                console.log("dialog accepted")
                udp.sendMessage(dialog.qrtag, true)
            } )
        }
    }

    SilicaFlickable
    {
        anchors.fill: parent

        PullDownMenu
        {
            MenuItem
            {
                text: "About"
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }

        Camera
        {
            id: camera
            flash.mode: Camera.FlashOff
            captureMode: Camera.CaptureStillImage
            focus.focusMode: Camera.FocusContinuous
            imageCapture
            {
                resolution: "800x600"
                onImageSaved:
                {
                    console.warn("image saved");
                    console.log(imagePath);
                    var message = {"imagePath": imagePath, "barcode": barcode};
                    var ret = barcode.decodeImageFromFile(imagePath);
                    if (ret)
                    {
                        messageLabel.text = ret;
                    }
                    else
                    {
                        messageLabel.text = "no tag found";
                    }
                }
                onCaptureFailed:
                {
                    console.error("error: " + camera.imageCapture.errorString);
                }
                onImageCaptured:
                { // does not work: missing SailfishOS feature
                    console.log("Image captured");
                }
            }
            onError:
            {
                console.error("error: " + camera.errorString);
            }
        }

        PageHeader
        {
            anchors.top: parent.top
            id: cameraHeader
            title: "Camera"
        }
        GStreamerVideoOutput
        {
            id: videoPreview
            anchors.centerIn: parent
            source: camera
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    console.log("capturing");
                    messageLabel.text = "";
                    camera.imageCapture.captureToLocation(imagePath);
                }
            }
        }
        Label
        {
            id: messageLabel
            anchors.top: videoPreview.bottom
            anchors.topMargin: 10
            color: Theme.primaryColor
        }

        Button
        {
            id: captureButton
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Capture"
            onClicked:
            {
                console.log("capturing");
                messageLabel.text = "";
                camera.imageCapture.captureToLocation(imagePath);
            }
        }
    }

    UdpIf
    {
        id: udp
        Component.onCompleted:
        {
            readInitParams()
            console.log("udp completed")
        }
    }

}


