#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QDebug>

class QToolBar;
class SpinSlider;
class Editor;

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent* event);

private:
    void createMenus();
    void createToolBars();
    void createDockWidgets();
    void loadPlugins();
    void readSettings();
    void writeSettings();

    Editor*     _editor;
    SpinSlider* _zoomSlider;

    QToolBar*   _toolBar;
    QMenu*      _fileMenu;
    QMenu*      _editMenu;
    QMenu*      _openRecentMenu;
    QMenu*      _windowMenu;
    QAction*    _newAct;
    QAction*    _openAct;
    QAction*    _saveAct;
    QAction*    _exitAct;
    QAction*    _redoAct;
    QAction*    _undoAct;

};

#endif // MAINWINDOW_H
