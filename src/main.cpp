#include "main.hpp"
#include "MainWindow.hpp"


int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    setStyles(&app);

    MainWindow window;
    window.show();

    return app.exec();
}

void setStyles(QApplication* app){
    app->setStyleSheet(R"(
        /* global */
        QMainWindow {
            background: #151515;
            color: #eeeeee;
        }
    
        QWidget {
            color: #eeeeee;
        }
    
        /* top menu bar */
        QMenuBar {
            background: #202020;
            color: #eeeeee;
            border: 1px solid #b700ff;
            spacing: 2px;
        }
    
        QMenuBar::item {
            background: transparent;
            padding: 5px 10px;
        }
    
        QMenuBar::item:selected {
            background: #383838;
        }
    
        QMenuBar::item:pressed {
            background: #444444;
        }
    
        /* top menu items */
        QMenu {
            background: #202020;
            color: #eeeeee;
            border: 1px solid #b700ff;
            padding: 4px;
        }
    
        QMenu::item {
            padding: 6px 30px 6px 10px;
        }
    
        QMenu::item:selected {
            background: #383838;
        }
    
        QMenu::item:pressed {
            background: #444444;
        }
    
        QMenu::item:disabled {
            color: #666666;
        }
    
        QMenu::separator {
            height: 1px;
            background: #5f0080;
            margin: 4px 6px;
        }
    
    
        /* dock widgets */
        QDockWidget {
            background: #202020;
            color: #eeeeee;
        }
    
        QDockWidget::title {
            background: #303030;
            color: #eeeeee;
            padding: 5px;
            border-bottom: 1px solid #5f0080;
        }
    
        QWidget[role="dockContentWrapper"] {
            background: #1a1a1a;
            border: 1px solid #b700ff;
        }
    
        QDockWidget::close-button,
        QDockWidget::float-button {
            background: transparent;
            border: none;
        }
    
        QDockWidget::close-button:hover,
        QDockWidget::float-button:hover {
            background: #383838;
        }
    
    
        /* toolbar */
        QToolBar {
            background: #202020;
            color: #eeeeee;
            border: 1px solid #b700ff;
            spacing: 5px;
            padding: 2px;
        }
    
        QToolBar::separator {
            background: #5f0080;
            width: 1px;
            margin: 4px 5px;
        }
    
        QToolButton {
            background: transparent;
            color: #eeeeee;
            border: 1px solid transparent;
            padding: 5px;
        }
    
        QToolButton:hover {
            background: #383838;
            border: 1px solid #5f0080;
        }
    
        QToolButton:pressed {
            background: #444444;
            border: 1px solid #b700ff;
        }
    
        QToolButton:checked {
            background: #3a1645;
            border: 1px solid #b700ff;
        }
    
        /* buttons */
        QPushButton {
            background: #282828;
            color: #eeeeee;
            border: 1px solid #5f0080;
            padding: 5px 10px;
        }
    
        QPushButton:hover {
            background: #383838;
            border: 1px solid #b700ff;
        }
    
        QPushButton:pressed {
            background: #444444;
            border: 1px solid #b700ff;
        }
    
        QPushButton:disabled {
            background: #202020;
            color: #666666;
            border: 1px solid #333333;
        }
    
    
        /* tree */
        QTreeWidget {
            background: #1a1a1a;
            color: #eeeeee;
            border: 1px solid #5f0080;
            outline: none;
        }
    
        QTreeWidget::item {
            padding: 4px;
        }
    
        QTreeWidget::item:hover {
            background: #303030;
        }
    
        QTreeWidget::item:selected {
            background: #3a1645;
            color: white;
            border: 1px solid #b700ff;
        }
    
        /* inputs */
        QLineEdit,
        QSpinBox,
        QDoubleSpinBox,
        QComboBox {
            background: #202020;
            color: #eeeeee;
            border: 1px solid #5f0080;
            padding: 4px;
        }
    
        QLineEdit:hover,
        QSpinBox:hover,
        QDoubleSpinBox:hover,
        QComboBox:hover {
            border: 1px solid #8f00c7;
        }
    
        QLineEdit:focus,
        QSpinBox:focus,
        QDoubleSpinBox:focus,
        QComboBox:focus {
            border: 1px solid #b700ff;
            background: #252025;
        }
    
    
        /* sliders */
        QSlider::groove:horizontal {
            height: 4px;
            background: #383838;
        }
    
        QSlider::sub-page:horizontal {
            background: #7d00aa;
        }
    
        QSlider::add-page:horizontal {
            background: #252525;
        }
    
        QSlider::handle:horizontal {
            background: #eeeeee;
            border: 1px solid #b700ff;
            width: 12px;
            margin: -5px 0;
        }
    
        QSlider::handle:horizontal:hover {
            background: #ffffff;
            border: 1px solid #d65cff;
        }
    
    
        /* labels */
        QLabel {
            color: #eeeeee;
        }
    
        /* scroll bars */
        QScrollBar:vertical {
            background: #1a1a1a;
            width: 12px;
            border: 1px solid #5f0080;
        }
    
        QScrollBar::handle:vertical {
            background: #383838;
            min-height: 20px;
            border: 1px solid #5f0080;
        }
    
        QScrollBar::handle:vertical:hover {
            background: #4a4a4a;
            border: 1px solid #b700ff;
        }
    
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            background: #202020;
            border: none;
            height: 0px;
        }
    
        QScrollBar:horizontal {
            background: #1a1a1a;
            height: 12px;
            border: 1px solid #5f0080;
        }
    
        QScrollBar::handle:horizontal {
            background: #383838;
            min-width: 20px;
            border: 1px solid #5f0080;
        }
    
        QScrollBar::handle:horizontal:hover {
            background: #4a4a4a;
            border: 1px solid #b700ff;
        }
    
        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal {
            background: #202020;
            border: none;
            width: 0px;
        }
    )");
}
