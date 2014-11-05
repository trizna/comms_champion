import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.1
import cc.GlobalConstants 1.0
import cc.GuiAppMgr 1.0

ToolBar {
    id: thisToolbar
    state: GuiAppMgr.recvState
    
    states: [
        State {
            name: GlobalConstants.waitingState
            PropertyChanges { 
                target: startStopButton
                tooltip: "Start"
                iconSource: GlobalConstants.startIconPathFromQml
            }
            
            PropertyChanges { 
                target: saveButton
                enabled: true
            }
        },
        
        State {
            name: GlobalConstants.runningState
            PropertyChanges{
                target: startStopButton
                tooltip: "Stop"
                iconSource: GlobalConstants.stopIconPathFromQml
            }
            
            PropertyChanges { 
                target: saveButton
                enabled: false
            }
        }
    ]
    
    style: ToolBarStyle {
        background: Rectangle {
            color: GlobalConstants.background
        }
    }
    
    RowLayout {
        ToolButton {
            id: startStopButton
            onClicked: {
                if (thisToolbar.state == GlobalConstants.waitingState) {
                    GuiAppMgr.recvStartClicked()
                }
                else {
                    console.assert(
                        thisToolbar.state == GlobalConstants.runningState, 
                        "Unknown state" + thisToolbar.state);
                    GuiAppMgr.recvStopClicked()
                }
            }
        }
        
        ToolButton {
            id: saveButton
            tooltip: "Save Messages"
            iconSource: GlobalConstants.saveIconPathFromQml
            
            onClicked: {
                thisToolbar.saveClicked()
            }
        }
    }
    
    Connections {
        target: GuiAppMgr
        onSigSetRecvState: {
            thisToolbar.state = state
        }
    }

}
