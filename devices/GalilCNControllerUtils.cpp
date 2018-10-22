#include "GalilCNControllerUtils.hpp"

using namespace PROGRAM_NAMESPACE;

GalilCNControllerUtils::GalilCNControllerUtils() { }

QString GalilCNControllerUtils::getErrorString(int error) {

    constexpr GSize size = G_SMALL_BUFFER;
    char description[size];

    GError(error, description, size);

    return QString(description);

//    switch(error) {
//        case G_NO_ERROR: return "no error";
//        case G_GCLIB_ERROR: return "gclib unexpected error";
//        case G_GCLIB_UTILITY_ERROR: return "invalid request value or bad arguments were specified to GUtility()";
//        case G_GCLIB_UTILITY_IP_TAKEN: return "ip address is already taken by a device on the network";
//        case G_GCLIB_NON_BLOCKING_READ_EMPTY: return "data was not waiting for a zero-timeout read";
//        case G_TIMEOUT: return "device timed out";
//        case G_OPEN_ERROR: return "device failed to open";
//        case G_READ_ERROR: return "device read error";
//        case G_WRITE_ERROR: return "device write error";
//        case G_INVALID_PREPROCESSOR_OPTIONS: return "preprocessor did not recognize options";
//        case G_COMMAND_CALLED_WITH_ILLEGAL_COMMAND: return "illegal command passed to command call";
//        case G_DATA_RECORD_ERROR: return "data record error";
//        case G_UNSUPPORTED_FUNCTION: return "function not supported on this communication bus";
//        case G_FIRMWARE_LOAD_NOT_SUPPORTED: return "firmware cannot be loaded on this communication bus to this hardware";
//        case G_ARRAY_NOT_DIMENSIONED: return "array not dimensioned on controller or wrong size";
//        case G_ILLEGAL_DATA_IN_PROGRAM: return "illegal ASCII character in program";
//        case G_UNABLE_TO_COMPRESS_PROGRAM_TO_FIT: return "program cannot be compressed to fit on the controller";
//        case G_BAD_RESPONSE_QUESTION_MARK: return "question mark returned by controller";
//        case G_BAD_VALUE_RANGE: return "value passed to function was bad or out of range";
//        case G_BAD_FULL_MEMORY: return "operation could not complete because of a memory error";
//        case G_BAD_LOST_DATA: return "data was lost due to buffer or fifo limitations";
//        case G_BAD_FILE: return "file was not found, contents are invalid, or write failed";
//        case G_BAD_ADDRESS: return "a bad address was specified in open";
//        case G_BAD_FIRMWARE_LOAD: return "Firmware upgrade failed";
//        case G_GCAPS_OPEN_ERROR: return "gcaps connection could not be opened";
//        case G_GCAPS_SUBSCRIPTION_ERROR: return "function requires subscription not specified in GOpen()";
//    }

//    return "";

}


