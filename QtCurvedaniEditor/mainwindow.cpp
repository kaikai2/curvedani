#include <QMenuBar>
#include <QToolBar>
#include <QCloseEvent>
#include <QTextEdit>
#include <QSlider>
#include <QDockWidget>
#include "spinslider.h"
#include "editor.h"
#include "mainwindow.h"

MainWindow::MainWindow()
{
    _editor = new Editor(this);
    setCentralWidget(_editor);

    createMenus();
    createToolBars();
    createDockWidgets();
    loadPlugins();
    readSettings();

    resize(800, 600);// for test;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::createMenus()
{
    _newAct = new QAction(tr("&New"), this);
    _newAct->setShortcut(tr("Ctrl+N"));

    _openAct = new QAction(tr("&Open"), this);
    _openAct->setShortcut(tr("Ctrl+O"));

    _saveAct = new QAction(tr("&Save"), this);
    _saveAct->setShortcut(tr("Ctrl+S"));

    _exitAct = new QAction(tr("E&xit"), this);
    _exitAct->setShortcut(tr("Ctrl+X"));
    connect(_exitAct, SIGNAL(triggered()), this, SLOT(close()));

    _undoAct = new QAction(tr("&Undo"), this);
    _undoAct->setShortcut(tr("Ctrl+Z"));

    _redoAct = new QAction(tr("&Redo"), this);
    _redoAct->setShortcut(tr("Ctrl+Y"));

    _openRecentMenu = new QMenu(tr("Open Recent..."), this);
    _openRecentMenu->addAction("xxx.xx files"); // for test;

    _fileMenu = new QMenu(tr("&File"), this);
    _fileMenu->addAction(_newAct);
    _fileMenu->addAction(_openAct);
    _fileMenu->addAction(_saveAct);
    _fileMenu->addMenu(_openRecentMenu);
    _fileMenu->addSeparator();
    _fileMenu->addAction(_exitAct);

    _editMenu = new QMenu(tr("&Edit"), this);
    _editMenu->addAction(_undoAct);
    _editMenu->addAction(_redoAct);

    _windowMenu = new QMenu(tr("&Window"), this);

    menuBar()->addMenu(_fileMenu);
    menuBar()->addMenu(_editMenu);
    menuBar()->addMenu(_windowMenu);
}

void MainWindow::createToolBars()
{

}

void MainWindow::createDockWidgets()
{
    addDockWidget(Qt::LeftDockWidgetArea, _editor->optionDock(), Qt::Vertical);
    _windowMenu->addAction(_editor->optionDock()->toggleViewAction());

    addDockWidget(Qt::LeftDockWidgetArea, _editor->bonesDock(), Qt::Vertical);
    _windowMenu->addAction(_editor->bonesDock()->toggleViewAction());

    addDockWidget(Qt::LeftDockWidgetArea, _editor->motionDock(), Qt::Vertical);
    _windowMenu->addAction(_editor->motionDock()->toggleViewAction());
}

void MainWindow::loadPlugins()
{
    qDebug() << "MainWindow loadplugins" << this << this->thread();
}

void MainWindow::readSettings()
{

}

void MainWindow::writeSettings()
{

}

