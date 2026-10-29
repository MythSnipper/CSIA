#include "MainWindow.hpp"

#include "QT_GLRenderer.hpp"

#include <iostream>

#include <QDockWidget>
#include <QWidget>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QSettings>
#include <QCloseEvent>

auto QDFEATURES = QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable;

//callback: save layout on close
void MainWindow::closeEvent(QCloseEvent* event){
    saveLayout();

    QMainWindow::closeEvent(event);
}

//constructs panels, saves as default layout, then restores previous user saved layout
MainWindow::MainWindow(){
    resize(1280, 720);

    setDockNestingEnabled(true);
    setAnimated(false);

    addToolBar(Qt::TopToolBarArea, createToolbarDock());
    
    rendererDock = createRendererDock();
    timelineDock = createTimelineDock();
    sceneDock = createSceneDock();
    propertiesDock = createPropertiesDock();

    setupDefaultLayout();

    //save this as default layout
    defaultLayout = saveState();

    createMenuBar();
    //restore user previous layout
    restoreLayout();
}

//sets up default layout after the dock widgets are created
void MainWindow::setupDefaultLayout(){
    //This function is also used for restoring so this is included
    //make everything visible
    rendererDock->show();
    timelineDock->show();
    sceneDock->show();
    propertiesDock->show();

    //make them not floating
    rendererDock->setFloating(false);
    timelineDock->setFloating(false);
    sceneDock->setFloating(false);
    propertiesDock->setFloating(false);

    addDockWidget(Qt::LeftDockWidgetArea, rendererDock);

    splitDockWidget(rendererDock, sceneDock, Qt::Horizontal);
    resizeDocks({rendererDock, sceneDock}, {3, 2}, Qt::Horizontal);

    splitDockWidget(rendererDock, timelineDock, Qt::Vertical);
    resizeDocks({rendererDock, timelineDock}, {4, 1}, Qt::Vertical);

    splitDockWidget(sceneDock, propertiesDock, Qt::Horizontal);
    resizeDocks({sceneDock, propertiesDock}, {1, 1}, Qt::Horizontal);
}

//create fixed top menu bar
void MainWindow::createMenuBar(){
    QMenu* fileMenu = menuBar()->addMenu("&File");
    QMenu* editMenu = menuBar()->addMenu("&Edit");
    QMenu* viewMenu = menuBar()->addMenu("&View");

    //File
    fileMenu->addAction("Open...");
    fileMenu->addAction("Save");
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QMainWindow::close);

    //Edit
    editMenu->addAction("Undo");
    editMenu->addAction("Redo");

    // View
    viewMenu->addAction(rendererDock->toggleViewAction());
    viewMenu->addAction(sceneDock->toggleViewAction());
    viewMenu->addAction(propertiesDock->toggleViewAction());
    viewMenu->addAction(timelineDock->toggleViewAction());

    viewMenu->addSeparator();

    viewMenu->addAction(
        "Restore Default Layout",
        this,
        &MainWindow::setupDefaultLayout
    );
}

//save and restore the layout of panels
void MainWindow::saveLayout(){
    QSettings settings(
        "QuantumUser",
        "QuantumEngine"
    );

    settings.setValue(
        "geometry",
        saveGeometry()
    );

    settings.setValue(
        "windowState",
        saveState()
    );
}
void MainWindow::restoreLayout(){
    QSettings settings(
        "QuantumUser",
        "QuantumEngine"
    );

    restoreGeometry(
        settings.value("geometry").toByteArray()
    );

    restoreState(
        settings.value("windowState").toByteArray()
    );
}

//Create dockable widgets wrapping normal widgets
QToolBar* MainWindow::createToolbarDock(){
    QToolBar* toolbar = new QToolBar("Toolbar", this);
    toolbar->setObjectName("toolbar");

    toolbar->setMovable(true);
    toolbar->setFloatable(true);

    QAction* addParticleAction = toolbar->addAction("Add Particle");

    connect(addParticleAction, &QAction::triggered, this, [this](){
        scene.createParticle("Particle");
    
        if(rendererWidget){
            rendererWidget->syncScene();
        }
    });


    toolbar->addAction("Add Field");

    return toolbar;
}
QDockWidget* MainWindow::createRendererDock(){
    QDockWidget* dock = new QDockWidget("Renderer", this);
    dock->setObjectName("rendererDock");

    QWidget* dockContentWrapper = new QWidget(dock);
    dockContentWrapper->setProperty("role", "dockContentWrapper");

    QOpenGLWidget* renderer = createRenderer();

    QVBoxLayout* dockContentWrapperLayout = new QVBoxLayout(dockContentWrapper);
    dockContentWrapperLayout->addWidget(renderer);

    dock->setWidget(dockContentWrapper);

    dock->setFeatures(
        QDFEATURES
    );

    dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    return dock;
}
QDockWidget* MainWindow::createTimelineDock(){
    QDockWidget* dock = new QDockWidget("Timeline", this);
    dock->setObjectName("timelineDock");

    QWidget* dockContentWrapper = new QWidget(dock);
    dockContentWrapper->setProperty("role", "dockContentWrapper");

    QWidget* timeline = createTimeline();

    QVBoxLayout* dockContentWrapperLayout = new QVBoxLayout(dockContentWrapper);
    dockContentWrapperLayout->addWidget(timeline);

    dock->setWidget(dockContentWrapper);

    dock->setFeatures(
        QDFEATURES
    );

    dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    return dock;
}
QDockWidget* MainWindow::createSceneDock(){
    QDockWidget* dock = new QDockWidget("Scene", this);
    dock->setObjectName("sceneDock");

    QWidget* dockContentWrapper = new QWidget(dock);
    dockContentWrapper->setProperty("role", "dockContentWrapper");

    QWidget* panel = createScenePanel();

    QVBoxLayout* dockContentWrapperLayout = new QVBoxLayout(dockContentWrapper);
    dockContentWrapperLayout->addWidget(panel);

    dock->setWidget(dockContentWrapper);

    dock->setFeatures(
        QDFEATURES
    );

    dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    return dock;
}
QDockWidget* MainWindow::createPropertiesDock(){
    QDockWidget* dock = new QDockWidget("Properties", this);
    dock->setObjectName("propertiesDock");

    QWidget* dockContentWrapper = new QWidget(dock);
    dockContentWrapper->setProperty("role", "dockContentWrapper");

    QWidget* panel = createPropertiesPanel();

    QVBoxLayout* dockContentWrapperLayout = new QVBoxLayout(dockContentWrapper);
    dockContentWrapperLayout->addWidget(panel);

    dock->setWidget(dockContentWrapper);

    dock->setFeatures(
        QDFEATURES
    );

    dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    return dock;
}

//Create widgets
QOpenGLWidget* MainWindow::createRenderer(){
    rendererWidget = new QT_GLRenderer();
    rendererWidget->setScene(&scene);

    return rendererWidget;
}
QWidget* MainWindow::createTimeline(){
    // timeline
    QWidget* timeline = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(timeline);

    // slider and time display
    QWidget* slidertime = new QWidget();
    QHBoxLayout* slidertimeLayout = new QHBoxLayout(slidertime);

    QSlider* slider = new QSlider(Qt::Horizontal);
    QLabel* time = new QLabel("0.00 s");

    slidertimeLayout->addWidget(slider, 1);
    slidertimeLayout->addWidget(time);

    // control buttons
    QWidget* buttons = new QWidget();
    QHBoxLayout* buttonsLayout = new QHBoxLayout(buttons);

    QPushButton* previous = new QPushButton("◀");
    QPushButton* play = new QPushButton("▶");
    QPushButton* pause = new QPushButton("▶|");
    QWidget* none = new QWidget();

    buttonsLayout->addWidget(previous);
    buttonsLayout->addWidget(play);
    buttonsLayout->addWidget(pause);
    buttonsLayout->addWidget(none, 1);

    layout->addWidget(slidertime);
    layout->addWidget(buttons);

    return timeline;
}
QWidget* MainWindow::createScenePanel(){
    QWidget* scenePanel = new QWidget();

    QLabel* sceneText = new QLabel("objects listed here", scenePanel);
    sceneText->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    
    return scenePanel;
}
QWidget* MainWindow::createPropertiesPanel(){
    QWidget* propertiesPanel = new QWidget();

    QLabel* propertiesText = new QLabel("properties listed here", propertiesPanel);
    propertiesText->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    return propertiesPanel;
}

