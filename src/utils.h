/**********************************************
**    This file is part of Lorris
**    http://tasssadar.github.com/Lorris/
**
**    See README and COPYING
***********************************************/

#ifndef NUM_FUNC_H
#define NUM_FUNC_H

#include <QString>
#include <QThread>
#include <QFont>

class QStatusBar;

class Utils : public QThread
{
    Q_OBJECT
public:
    static QString hexToString(quint8 data, bool withZeroEx = false);
    static QString parseChar(char c);

    template <typename T> static inline void swapEndian(char *val);

    static QString toBase16(quint8 const * first, quint8 const * last);
    static QString toBinary(std::size_t width, int value);

    static void msleep(unsigned long msecs) { QThread::msleep(msecs); }
    static void sleep (unsigned long secs)  { QThread::sleep(secs); }
    static void usleep(unsigned long usecs) { QThread::usleep(usecs); }

    static QFont getMonospaceFont(quint8 size = 9);

    static void ThrowException(const QString& text, QWidget* parent = 0);
    static void printToStatusBar(const QString& msg, int timeout = 5000);
    static void setStatusBar(QStatusBar *bar);

    static void playErrorSound();

private:
    static QStatusBar* m_status_bar;
};

template <typename T>
void Utils::swapEndian(char *val)
{
    for(qint8 i = sizeof(T); i > 0; i -= 2, ++val)
        std::swap(*val, *(val + i - 1));
}


#endif // NUM_FUNC_H
