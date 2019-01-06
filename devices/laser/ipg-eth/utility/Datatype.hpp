#ifndef IPG_DATATYPE_HPP
#define IPG_DATATYPE_HPP

#include <QtCore>

namespace ipg {

using IPG_BYTE = quint8;
using IPG_CHAR = qint8;
using IPG_USHORT = quint16;
using IPG_SHORT = qint16;
using IPG_ULONG = quint32;
using IPG_LONG = qint32;
using IPG_64ULONG = qint64;
using IPG_FLOAT = float;

using IPG_MAIN_INTERFACE = IPG_USHORT;
using IPG_INTERFACE_CONFIG = IPG_USHORT;

}

#endif // IPG_DATATYPE_HPP
