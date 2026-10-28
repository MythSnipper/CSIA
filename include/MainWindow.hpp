#pragma once

#include <QMainWindow>
#include <QByteArray>
#include <QCloseEvent>
#include <QOpenGLWidget>

#include <Scene.hpp>
//forward declare
class QT_GLRenderer;


class MainWindow : public QMainWindow{
public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Scene scene;
    QT_GLRenderer* rendererWidget = nullptr;


    void setupDefaultLayout();

    void createMenuBar();
    void createViewMenu();

    QByteArray defaultLayout;
    void saveLayout();
    void restoreLayout();

    QDockWidget* rendererDock;
    QDockWidget* timelineDock;
    QDockWidget* sceneDock;
    QDockWidget* propertiesDock;

    QToolBar* createToolbarDock();
    QDockWidget* createRendererDock();
    QDockWidget* createTimelineDock();
    QDockWidget* createSceneDock();
    QDockWidget* createPropertiesDock();

    QOpenGLWidget* createRenderer();
    QWidget* createTimeline();
    QWidget* createScenePanel();
    QWidget* createPropertiesPanel();
};