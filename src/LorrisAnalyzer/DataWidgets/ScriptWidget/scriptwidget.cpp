/**********************************************
**    This file is part of Lorris
**    http://tasssadar.github.com/Lorris/
**
**    See README and COPYING
***********************************************/

#include <QLabel>

#include "scriptwidget.h"
#include "scripteditor.h"
#include "scriptenv.h"
#include "../../../shared/terminal.h"

ScriptWidget::ScriptWidget(QWidget *parent) : DataWidget(parent)
{
    setTitle(tr("Script"));
    setIcon(":/dataWidgetIcons/script.png");

    m_widgetType = WIDGET_SCRIPT;
    m_editor = NULL;

    m_terminal = new Terminal(this);
    layout->setContentsMargins(5, 0, 5, 5);
    layout->addWidget(m_terminal, 4);

    resize(120, 100);

    m_env = NULL;
}

ScriptWidget::~ScriptWidget()
{
    delete m_editor;
}

void ScriptWidget::setUp(Storage *storage)
{
    DataWidget::setUp(storage);

    QAction *src_act = contextMenu->addAction(tr("Set source..."));
    connect(src_act,    SIGNAL(triggered()),                 this,       SLOT(setSourceTriggered()));

    m_env = new ScriptEnv((WidgetArea*)parent(), getId(),  this);

    connect(m_terminal, SIGNAL(keyPressed(QString)),         m_env,      SLOT(keyPressed(QString)));
    connect(m_env,      SIGNAL(clearTerm()),                 m_terminal, SLOT(clear()));
    connect(m_env,      SIGNAL(appendTerm(QString)),         m_terminal, SLOT(appendText(QString)));
    connect(m_env,      SIGNAL(appendTermRaw(QByteArray)),   m_terminal, SLOT(appendText(QByteArray)));
    connect(m_env,      SIGNAL(SendData(QByteArray)),        this,       SIGNAL(SendData(QByteArray)));
}

void ScriptWidget::newData(analyzer_data *data, quint32 index)
{
    // FIXME: is it correct?
    //if(!m_updating)
    //    return;

    QString res = m_env->dataChanged(data, index);
    if(!res.isEmpty())
        m_terminal->appendText(res);
}

void ScriptWidget::saveWidgetInfo(DataFileParser *file)
{
    DataWidget::saveWidgetInfo(file);

    // source
    file->writeBlockIdentifier("scriptWSource");
    {
        QByteArray source = m_env->getSource().toUtf8();
        quint32 len = source.length();

        file->write((char*)&len, sizeof(quint32));
        file->write(source.data(), len);
    }

    // terminal data
    file->writeBlockIdentifier("scriptWTerm");
    {
        QByteArray data = m_terminal->getData();
        quint32 len = data.length();

        file->write((char*)&len, sizeof(quint32));
        file->write(data.data(), len);
    }

    // storage data
    m_env->onSave();
    m_env->getStorage()->saveToFile(file);
}

void ScriptWidget::loadWidgetInfo(DataFileParser *file)
{
    DataWidget::loadWidgetInfo(file);

    QString source = "";
    // source
    if(file->seekToNextBlock("scriptWSource", BLOCK_WIDGET))
    {
        quint32 size = 0;
        file->read((char*)&size, sizeof(quint32));

        source = QString::fromUtf8(file->read(size), size);
    }

    // terminal data
    if(file->seekToNextBlock("scriptWTerm", BLOCK_WIDGET))
    {
        quint32 size = 0;
        file->read((char*)&size, sizeof(quint32));

        QByteArray data(file->read(size));
        m_terminal->appendText(data);
    }

    // storage data
    m_env->getStorage()->loadFromFile(file);

    try
    {
        m_env->setSource(source);
    } catch(const QString&) { }
}

void ScriptWidget::setSourceTriggered()
{
    delete m_editor;

    m_editor = new ScriptEditor(m_env->getSource(), getTitle());
    m_editor->show();

    connect(m_editor, SIGNAL(applySource(bool)), SLOT(sourceSet(bool)));
}

void ScriptWidget::sourceSet(bool close)
{
    try
    {
        m_env->setSource(m_editor->getSource());

        if(close)
        {
            m_editor->deleteLater();
            m_editor = NULL;
        }
        emit updateData();
    }
    catch(const QString& text)
    {
        Utils::ThrowException(text, m_editor);
    }
}

void ScriptWidget::moveEvent(QMoveEvent *)
{
    if(m_env)
        m_env->setPos(pos().x(), pos().y());
}

void ScriptWidget::resizeEvent(QResizeEvent *)
{
    if(m_env)
        m_env->setSize(size());
}

void ScriptWidget::onWidgetAdd(DataWidget *w)
{
    if(m_env)
        m_env->onWidgetAdd(w);
}

void ScriptWidget::onWidgetRemove(DataWidget *w)
{
    if(m_env)
        m_env->onWidgetRemove(w);
}

void ScriptWidget::onScriptEvent(const QString& eventId)
{
    if(m_env)
        m_env->callEventHandler(eventId);
}

ScriptWidgetAddBtn::ScriptWidgetAddBtn(QWidget *parent) : DataWidgetAddBtn(parent)
{
    setText(tr("Script"));
    setIconSize(QSize(17, 17));
    setIcon(QIcon(":/dataWidgetIcons/script.png"));

    m_widgetType = WIDGET_SCRIPT;
}
