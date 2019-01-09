#ifndef ABSTRACTMOTIONUTILS_HPP
#define ABSTRACTMOTIONUTILS_HPP

#include <QtCore>
#include <QString>

#include <configure.h>

namespace PROGRAM_NAMESPACE {

enum class MotionStopCode : int {
    MOTION_NAN,
    MOTION_IS_RUNNING,
    MOTION_STOP_CORRECTLY,
    MOTION_STOP_COMMAND,
    MOTION_STOP_ON_ERROR
};

class AbstractMotionUtils {

    Q_DECLARE_TR_FUNCTIONS(AbstractMotionUtils)

public:

    static QString getMotionStopCodeDescription(const MotionStopCode& value) {

        QString descr = "";

        switch(value) {
            case MotionStopCode::MOTION_NAN: descr = tr("Motion unknown stop code"); break;
            case MotionStopCode::MOTION_IS_RUNNING: descr = tr("Motion is running"); break;
            case MotionStopCode::MOTION_STOP_CORRECTLY: descr = tr("Motion stop correctly"); break;
            case MotionStopCode::MOTION_STOP_COMMAND: descr = tr("Motion stop command"); break;
            case MotionStopCode::MOTION_STOP_ON_ERROR: descr = tr("Motion stop on error"); break;
        }
        return descr;

    }

};

}

#endif // ABSTRACTMOTIONUTILS_HPP
