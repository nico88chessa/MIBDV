#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <QDebug>
#include <QString>
#include <QMessageLogger>
#include <QFile>
#include <QStandardPaths>
#include <QtGlobal>
#include <QDir>

#include "configure.h"
#include "fstream"

namespace PROGRAM_NAMESPACE {

class Logger {

    friend inline void messageHandlerFunction(QtMsgType type, const QMessageLogContext& context, const QString& msg);

private:
    std::ofstream file;

private:
    Logger();

    ~Logger();

    void messageHandler(QtMsgType type,
                        const QMessageLogContext &context,
                        const QString &msg);
    void rotateLog();


public:
    Logger(const Logger& l) = delete;
    void operator=(const Logger& l) = delete;
    static Logger& instance();

    void enter(const char* functionName) const;
    void exit(const char* functionName) const;

    QDebug debug() const;
    QDebug info() const;
    QDebug warn() const;
    QDebug error() const;
    [[noreturn]] void fatal(const char* msg, ...) const;

};

//void messageHandlerFunction(QtMsgType type, const QMessageLogContext& context, const QString& msg);

#ifdef LOG_VERBOSE
#define traceEnter Logger::instance().enter(QT_MESSAGELOG_FUNC)
#define traceExit Logger::instance().exit(QT_MESSAGELOG_FUNC)
#define traceDebug Logger::instance().debug
#else
#define traceEnter
#define traceExit
// vedere qlogging.h per questo trick
#define traceDebug if (false) QMessageLogger().noDebug
#endif

#define traceInfo Logger::instance().info
#define traceWarn Logger::instance().warn
#define traceErr Logger::instance().error
#define traceFatal Logger::instance().fatal

}

#endif // LOGGER_HPP
