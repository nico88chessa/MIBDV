#ifndef IPG_LASER_TEMP_TEST_SYNC_EXECUTOR_HPP
#define IPG_LASER_TEMP_TEST_SYNC_EXECUTOR_HPP

#include "abstract_sync_executor.hpp"
#include "../utility/constants.hpp"
#include <QTcpSocket>

namespace ipg {

class TestSyncExecutor : public AbstractSyncExecutor {

public:
    typedef TestSyncExecutor* Ptr;
    typedef const TestSyncExecutor* ConstPtr;

private:
    QTcpSocket socket;
    QString ip;
    quint16 port;

protected:

    virtual int syncCaller(const QByteArray& bytesIn, QByteArray& bytesOut) {

        bytesOut = bytesIn;
        return 0;

    }

};


}

#endif // TEST_SYNC_EXECUTOR_HPP
