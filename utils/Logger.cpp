#include "Logger.hpp"
#include <QDateTime>

using namespace mibdv;

static constexpr char TIME_FORMAT[] = "yyyy-MM-dd hh:mm:ss.zzz";

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    QByteArray localTime = QDateTime::currentDateTime().toString(TIME_FORMAT).toLocal8Bit();

    switch (type) {
    case QtDebugMsg:
        // fprintf(stderr, "%s DEBUG: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fprintf(stderr, "%s DEBUG: %s\n", localTime.constData(), localMsg.constData());
        fflush(stderr);
        break;
    case QtInfoMsg:
        fprintf(stderr, "%s INFO: %s\n", localTime.constData(), localMsg.constData());
        fflush(stderr);
        break;
    case QtWarningMsg:
        fprintf(stderr, "%s WARN: %s\n", localTime.constData(), localMsg.constData());
        fflush(stderr);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "%s CRITICAL: %s\n", localTime.constData(), localMsg.constData());
        fflush(stderr);
        break;
    case QtFatalMsg:
        fprintf(stderr, "%s FATAL: %s\n", localTime.constData(), localMsg.constData());
        fflush(stderr);
        break;
    }
}

Logger::Logger() {
    qInstallMessageHandler(myMessageOutput);
}

Logger& Logger::instance() {
    static Logger l;
    return l;
}

void Logger::enter(const char* functionName) const {
    qDebug() << functionName << "ENTER";
}

void Logger::exit(const char* functionName) const {
    qDebug() << functionName << "EXIT";
}

QDebug Logger::debug() const {
    return qDebug();
}

QDebug Logger::info() const {
    return qInfo();
}

QDebug Logger::error() const {
    return qCritical();
}

void Logger::fatal(const char *msg, ...) const {
    va_list va;
    va_start(va, msg);
    qFatal(msg, va);
    va_end(va);
}

