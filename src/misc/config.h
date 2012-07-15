/**********************************************
**    This file is part of Lorris
**    http://tasssadar.github.com/Lorris/
**
**    See README and COPYING
***********************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include <QHash>
#include <QVariant>

#include "singleton.h"

enum cfg_quint32
{
    CFG_QUINT32_CONNECTION_TYPE = 0,
    CFG_QUINT32_TAB_TYPE,
    CFG_QUINT32_SERIAL_BAUD,
    CFG_QUINT32_SHUPITO_MODE,
    CFG_QUINT32_SHUPITO_PRG_SPEED,
    CFG_QUINT32_LANGUAGE,
    CFG_QUINT32_TERMINAL_FMT,
    CFG_QUINT32_TCP_PORT,
    CFG_QUINT32_TERMINAL_INPUT,
    CFG_QUINT32_PROXY_PORT,
    CFG_QUINT32_SHUPITO_VERIFY,
    CFG_QUINT32_ANALYZER_PLAY_DEL,
    CFG_QUITN32_SHUPITO_TERM_FMT,
    CFG_QUINT32_ANALYZER_GRID_SIZE,
    CFG_QUINT32_LAST_UPDATE_CHECK,

    CFG_QUINT32_NUM
};

enum cfg_string
{
    CFG_STRING_SERIAL_PORT = 0,
    CFG_STRING_SHUPITO_PORT,
    CFG_STRING_HEX_FOLDER,
    CFG_STRING_ANALYZER_FOLDER,
    CFG_STRING_SHUPITO_HEX_FOLDER,
    CFG_STRING_SHUPITO_TUNNEL,
    CFG_STRING_TCP_ADDR,
    CFG_STRING_PROXY_ADDR,
    CFG_STRING_ANALYZER_JS,
    CFG_STRING_TERMINAL_TEXTFILE,
    CFG_STRING_TERMINAL_SETTINGS,
    CFG_STRING_SHUPITO_TERM_SET,
    CFG_STRING_ANALYZER_IMPORT,
    CFG_STRING_WINDOW_PARAMS,
    CFG_STRING_GRAPH_EXPORT,
    CFG_STRING_APP_FONT,
    CFG_STRING_SCRIPT_WND_PARAMS,

    CFG_STRING_NUM
};

enum cfg_bool
{
    CFG_BOOL_SHUPITO_TUNNEL = 0,
    CFG_BOOL_SHUPITO_SHOW_LOG,
    CFG_BOOL_SHUPITO_SHOW_FUSES,
    CFG_BOOL_SHUPITO_OVERVOLTAGE,
    CFG_BOOL_SHUPITO_TURNOFF_VCC,
    CFG_BOOL_SHUPITO_TRANSLATE_FUSES,
    CFG_BOOL_SHUPITO_HIDE_RESERVED,
    CFG_BOOL_ANALYZER_ENABLE_GRID,
    CFG_BOOL_ANALYZER_SHOW_GRID,
    CFG_BOOL_SHUPITO_SHOW_SETTINGS,
    CFG_BOOL_TERMINAL_SHOW_BOOTLOADER,
    CFG_BOOL_TERMINAL_SHOW_WARN,
    CFG_BOOL_SHUPITO_SHOW_FLASH_WARN,
    CFG_BOOL_AUTO_UPDATE,
    CFG_BOOL_CHECK_FOR_UPDATE,
    CFG_BOOL_LOAD_LAST_SESSION,
    CFG_BOOL_SESSION_CONNECT,
    CFG_BOOL_PORTABLE,
    CFG_BOOL_SHOW_SCRIPT_ERROR,

    CFG_BOOL_NUM
};

enum cfg_variant
{
    CFG_VARIANT_CONNECTIONS,
    CFG_VARIANT_USB_ENUMERATOR,
    CFG_VARIANT_NUM
};

enum cfg_float
{
    CFG_FLOAT_SHUPITO_OVERVOLTAGE_VAL = 0,

    CFG_FLOAT_NUM
};

class Config : public Singleton<Config>
{
public:
    Config();
    ~Config();

    void openSettings();
    void closeSettings();
    void resetToDefault();

    quint32 get(cfg_quint32 item);
    QString get(cfg_string item);
    bool    get(cfg_bool item);
    QVariant get(cfg_variant item);
    float   get(cfg_float item);

    void set(cfg_quint32 item, quint32        val);
    void set(cfg_string  item, const QString& val);
    void set(cfg_bool    item, bool           val);
    void set(cfg_variant item, QVariant const & val);
    void set(cfg_float   item, float          val);

private:
    QSettings *m_settings;
};

#define sConfig Config::GetSingleton()

#endif // CONFIG_H