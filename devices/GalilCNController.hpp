#ifndef GALILCNCONTROLLER_HPP
#define GALILCNCONTROLLER_HPP

#include <gclib.h>
#include <gclibo.h>

#include <QString>
#include <QScopedPointer>

#include <configure.h>
#include <utils/Logger.hpp>
#include <devices/GalilCNControllerUtils.hpp>

class GalilCNController {
    using Ptr = GalilCNController *;
    using ConstPtr = const GalilCNController *;

private:
    QScopedPointer<GCon> handle;

public:
    GalilCNController();
    ~GalilCNController();

    bool connect(const QString& ip);

};

#endif // GALILCNCONTROLLER_HPP
