/**********************************************
**    This file is part of Lorris
**    http://tasssadar.github.com/Lorris/
**
**    See README and COPYING
***********************************************/

#ifndef PYTHONENGINE_H
#define PYTHONENGINE_H

#include "../../../../../dep/pythonqt/src/PythonQt.h"

#include "scriptengine.h"

class PythonEngine;
class Joystick;
class QComboBox;

class PythonFunctions : public QObject
{
    Q_OBJECT
public:
    PythonFunctions(PythonEngine *engine, QObject *parent);

public slots:
    void sendData(const QByteArray& data);
    int getWidth();
    int getHeight();
    void throwException(const QString& text);
    Joystick *getJoystick(int id);
    void closeJoystick(Joystick *joy);
    QStringList getJoystickNames();
    QTimer *newTimer();
    void AddComboBoxItems(QComboBox *box, QStringList items);
    void moveWidget(QWidget *w, int x, int y);
    void resizeWidget(QWidget *w, int width, int height);
    DataWidget *newWidget(int type, QString title, int width, int height, int x, int y);
    DataWidget *newWidget(int type, QString title, int width, int height)
    {
        return newWidget(type, title, width, height, -1, -1);
    }
    DataWidget *newWidget(int type, QString title)
    {
        return newWidget(type, title, -1, -1, -1, -1);
    }

private:
    PythonEngine *m_engine;
};

class PythonEngine : public ScriptEngine
{
    Q_OBJECT

    friend class PythonFunctions;

public:
    PythonEngine(WidgetArea *area, quint32 w_id, Terminal *terminal, QObject *parent = 0);
    ~PythonEngine();
    
    void setSource(const QString& source);
    QString dataChanged(analyzer_data *data, quint32 index);
    void onWidgetAdd(DataWidget *w);
    void onWidgetRemove(DataWidget *w);
    void callEventHandler(const QString& eventId);
    void onSave();

public slots:
    void keyPressed(const QString &key);

private slots:
    void onTitleChange(const QString& newTitle);
    void widgetDestroyed(QObject *widget);

private:
    static QString getNewModuleName();

    PythonQtObjectPtr m_module;
    bool m_evaluating;
    PythonFunctions m_functions;
};

#endif // PYTHONENGINE_H