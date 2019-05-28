#ifndef INSPECTORSTATUSNOTIFIER_HPP
#define INSPECTORSTATUSNOTIFIER_HPP

#include <QObject>

#include <configure.h>


namespace PROGRAM_NAMESPACE {

class InspectorStatusNotifier : public QObject {
    Q_OBJECT

public:
    using Ptr = InspectorStatusNotifier*;
    using ConstPtr = const InspectorStatusNotifier*;

signals:
    void galilPLCInspectorIsRunningSignal(bool value);
    void galilCNInspectorIsRunningSignal(bool value);

};

}

#endif // INSPECTORSTATUSNOTIFIER_HPP
