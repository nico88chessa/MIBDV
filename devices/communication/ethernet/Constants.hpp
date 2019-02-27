#ifndef COMMUNICATION_ETHERNET_CONSTANTS_HPP
#define COMMUNICATION_ETHERNET_CONSTANTS_HPP

namespace communication {

namespace ethernet {

static constexpr int CONNECTION_DEFAULT_TIMEOUT_MS = 500;
static constexpr int CONNECTION_MAX_WRITE_TIME_MS = 500;
static constexpr int CONNECTION_MAX_READ_TIME_MS = 500;

static constexpr unsigned short CONNECTION_DEFAULT_PORT = 5100;
static constexpr char CONNECTION_DEFAULT_IP[] = "127.0.0.1";

}
    
}

#endif // IPG_CONSTANTS_HPP
