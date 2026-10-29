#include "MainWindow.hpp"

#include "QT_GLRenderer.hpp"

#include <iostream>

#include <QDockWidget>
#include <QWidget>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QSettings>
#include <QCloseEvent>
#include <QAction>
#include <QSignalBlocker>


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
        ID id = scene.createParticle("Particle");

        refreshScenePanel();
        selectSceneObject(id);

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
    QWidget* panel = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(panel);

    sceneTree = new QTreeWidget(panel);
    sceneTree->setHeaderHidden(true);
    sceneTree->setSelectionMode(QAbstractItemView::SingleSelection);

    layout->addWidget(sceneTree);

    connect(sceneTree, &QTreeWidget::currentItemChanged, this,
        [this](QTreeWidgetItem* current, QTreeWidgetItem*){
            if(current == nullptr){
                selectedObjectID = 0;
                clearPropertiesPanel();
                return;
            }

            ID id = static_cast<ID>(
                current->data(0, Qt::UserRole).toULongLong()
            );

            selectedObjectID = id;
            showSelectedObjectProperties(id);
        });

    refreshScenePanel();
    return panel;
}
QWidget* MainWindow::createPropertiesPanel(){
    QWidget* panel = new QWidget();
    propertiesLayout = new QFormLayout(panel);

    nameField = new QLineEdit(panel);
    propertiesLayout->addRow("Name:", nameField);

    const char* axes[] = {"X:", "Y:", "Z:"};

    for(int i = 0; i < 3; i++){
        positionFields[i] = new QDoubleSpinBox(panel);
        positionFields[i]->setRange(-1000000.0, 1000000.0);
        positionFields[i]->setDecimals(3);
        positionFields[i]->setSingleStep(0.1);
        propertiesLayout->addRow(
            QString("Position %1").arg(axes[i]),
            positionFields[i]
        );

        rotationFields[i] = new QDoubleSpinBox(panel);
        rotationFields[i]->setRange(-360000.0, 360000.0);
        rotationFields[i]->setDecimals(2);
        rotationFields[i]->setSingleStep(1.0);
        propertiesLayout->addRow(
            QString("Rotation %1").arg(axes[i]),
            rotationFields[i]
        );

        scaleFields[i] = new QDoubleSpinBox(panel);
        scaleFields[i]->setRange(-10000.0, 10000.0);
        scaleFields[i]->setDecimals(3);
        scaleFields[i]->setSingleStep(0.1);
        propertiesLayout->addRow(
            QString("Scale %1").arg(axes[i]),
            scaleFields[i]
        );
    }

    massField = new QDoubleSpinBox(panel);
    massField->setRange(0.0001, 1000000000.0);
    massField->setDecimals(4);
    massField->setSingleStep(0.1);
    propertiesLayout->addRow("Mass:", massField);

    chargeField = new QDoubleSpinBox(panel);
    chargeField->setRange(-1000000000.0, 1000000000.0);
    chargeField->setDecimals(4);
    chargeField->setSingleStep(0.1);
    propertiesLayout->addRow("Charge:", chargeField);

    connect(nameField, &QLineEdit::editingFinished, this, [this](){
        SceneObject* object = scene.getObject(selectedObjectID);

        if(object == nullptr){
            return;
        }

        QString name = nameField->text().trimmed();

        if(name.isEmpty()){
            nameField->setText(QString::fromStdString(object->getName()));
            return;
        }

        object->setName(name.toStdString());

        for(int i = 0; i < sceneTree->topLevelItemCount(); i++){
            QTreeWidgetItem* item = sceneTree->topLevelItem(i);

            if(item->data(0, Qt::UserRole).toULongLong() == selectedObjectID){
                item->setText(0, "Particle: " + name);
                break;
            }
        }
    });

    for(auto* field : positionFields){
        connect(field, &QDoubleSpinBox::valueChanged,
            this, [this](double){ applyPropertyEdits(); });
    }

    for(auto* field : rotationFields){
        connect(field, &QDoubleSpinBox::valueChanged,
            this, [this](double){ applyPropertyEdits(); });
    }

    for(auto* field : scaleFields){
        connect(field, &QDoubleSpinBox::valueChanged,
            this, [this](double){ applyPropertyEdits(); });
    }

    connect(massField, &QDoubleSpinBox::valueChanged,
        this, [this](double){ applyPropertyEdits(); });

    connect(chargeField, &QDoubleSpinBox::valueChanged,
        this, [this](double){ applyPropertyEdits(); });

    clearPropertiesPanel();
    return panel;
}




//helper to display a 3d vector
static QString vectorToString(const glm::vec3& value){
    return QString("X: %1, Y: %2, Z: %3")
        .arg(value.x, 0, 'f', 2)
        .arg(value.y, 0, 'f', 2)
        .arg(value.z, 0, 'f', 2);
}


//Scene panel
void MainWindow::refreshScenePanel(){
    if(sceneTree == nullptr){
        return;
    }

    sceneTree->clear();

    for(const auto& [id, object] : scene.getObjects()){
        QString typeName;

        switch(object->getType()){
            case SceneObjectType::PointParticle:
                typeName = "Particle";
                break;
            case SceneObjectType::FieldVolume:
                typeName = "Field";
                break;
        }

        QString itemName = QString::fromStdString(object->getName());

        QTreeWidgetItem* item = new QTreeWidgetItem(sceneTree);
        item->setText(0, typeName + ": " + itemName);
        item->setData(0, Qt::UserRole, QVariant::fromValue<qulonglong>(id));
    }
}
void MainWindow::showSelectedObjectProperties(ID id){
    SceneObject* object = scene.getObject(id);

    if(object == nullptr){
        selectedObjectID = 0;
        clearPropertiesPanel();
        return;
    }

    selectedObjectID = id;

    const QSignalBlocker blockName(nameField);
    const QSignalBlocker blockMass(massField);
    const QSignalBlocker blockCharge(chargeField);

    nameField->setText(QString::fromStdString(object->getName()));
    nameField->setEnabled(true);

    const Transform& transform = object->getTransform();

    const float position[] = {
        transform.position.x,
        transform.position.y,
        transform.position.z
    };

    const float rotation[] = {
        transform.rotation.x,
        transform.rotation.y,
        transform.rotation.z
    };

    const float scale[] = {
        transform.scale.x,
        transform.scale.y,
        transform.scale.z
    };

    for(int i = 0; i < 3; i++){
        const QSignalBlocker blockPosition(positionFields[i]);
        const QSignalBlocker blockRotation(rotationFields[i]);
        const QSignalBlocker blockScale(scaleFields[i]);

        positionFields[i]->setValue(position[i]);
        rotationFields[i]->setValue(rotation[i]);
        scaleFields[i]->setValue(scale[i]);

        positionFields[i]->setEnabled(true);
        rotationFields[i]->setEnabled(true);
        scaleFields[i]->setEnabled(true);
    }

    bool isParticle = object->getType() == SceneObjectType::PointParticle;

    massField->setEnabled(isParticle);
    chargeField->setEnabled(isParticle);

    if(isParticle){
        auto* particle = static_cast<PointParticle*>(object);
        massField->setValue(particle->getMass());
        chargeField->setValue(particle->getCharge());
    }
}







//Properties panel
void MainWindow::clearPropertiesPanel(){
    nameField->clear();
    nameField->setEnabled(false);

    for(int i = 0; i < 3; i++){
        positionFields[i]->setValue(0.0);
        rotationFields[i]->setValue(0.0);
        scaleFields[i]->setValue(1.0);

        positionFields[i]->setEnabled(false);
        rotationFields[i]->setEnabled(false);
        scaleFields[i]->setEnabled(false);
    }

    massField->setValue(1.0);
    chargeField->setValue(0.0);
    massField->setEnabled(false);
    chargeField->setEnabled(false);
}
void MainWindow::selectSceneObject(ID id){
    if(sceneTree == nullptr){
        return;
    }

    for(int i = 0; i < sceneTree->topLevelItemCount(); i++){
        QTreeWidgetItem* item = sceneTree->topLevelItem(i);

        if(item->data(0, Qt::UserRole).toULongLong() == id){
            sceneTree->setCurrentItem(item);
            return;
        }
    }
}







void MainWindow::applyPropertyEdits(){
    SceneObject* object = scene.getObject(selectedObjectID);

    if(object == nullptr){
        return;
    }

    glm::vec3 position(
        positionFields[0]->value(),
        positionFields[1]->value(),
        positionFields[2]->value()
    );

    glm::vec3 rotation(
        rotationFields[0]->value(),
        rotationFields[1]->value(),
        rotationFields[2]->value()
    );

    glm::vec3 scale(
        scaleFields[0]->value(),
        scaleFields[1]->value(),
        scaleFields[2]->value()
    );

    object->setPosition(position);
    object->setRotation(rotation);
    object->setScale(scale);

    if(object->getType() == SceneObjectType::PointParticle){
        auto* particle = static_cast<PointParticle*>(object);
        particle->setMass(massField->value());
        particle->setCharge(chargeField->value());
    }

    if(rendererWidget){
        rendererWidget->syncScene();
    }
}
