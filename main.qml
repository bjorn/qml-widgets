import QtQml 2.12
import QtWidgets 5.12

MainWindow {
    id: mainWindow
    windowTitle: qsTr("Hello World")

    centralWidget: Widget {
        layout: VBoxLayout {
            PushButton {
                text: "Open File..."
                onClicked: saveAsButton.visible = !saveAsButton.visible
            }
            PushButton {
                id: saveAsButton
                text: "Save As..."
            }
            PushButton {
                text: "Quit"
                onClicked: Qt.quit();
            }

            HBoxLayout {
                spacing: 0
                PushButton {
                    text: "Open File..."
                }
                PushButton {
                    text: "Save As..."
                }
                PushButton {
                    text: "Quit"
                    onClicked: Qt.quit();
                }
            }
        }
    }

    Component.onCompleted: visible = true
}
