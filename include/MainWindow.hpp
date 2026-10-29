#pragma once

#include <QMainWindow>
#include <QByteArray>
#include <QCloseEvent>
#include <QOpenGLWidget>
#include <QTreeWidget>
#include <QFormLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>


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



    //Scene and Properties panels
    QTreeWidget* sceneTree = nullptr;
    QFormLayout* propertiesLayout = nullptr;
    ID selectedObjectID = 0; //0 if no object is selected

    //Fields for editable properties
    QLineEdit* nameField = nullptr;
    QDoubleSpinBox* positionFields[3] = {};
    QDoubleSpinBox* rotationFields[3] = {};
    QDoubleSpinBox* scaleFields[3] = {};
    QDoubleSpinBox* massField = nullptr;
    QDoubleSpinBox* chargeField = nullptr;

    void refreshScenePanel();
    void showSelectedObjectProperties(ID id);
    void clearPropertiesPanel();
    void selectSceneObject(ID id);

    void applyPropertyEdits();
};