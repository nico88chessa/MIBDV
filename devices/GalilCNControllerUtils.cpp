#include "GalilCNControllerUtils.hpp"

using namespace PROGRAM_NAMESPACE;

GalilCNControllerUtils::GalilCNControllerUtils() { }

QString GalilCNControllerUtils::getErrorDescription(int error) {

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

QString GalilCNControllerUtils::getTCDescription(int rcCode) {

    QString value = "";
    switch (rcCode) {
        case 1: value = "Unrecognized command"; break;
        case 2: value = "Command only valid from program"; break;
        case 3: value = "Command not valid in program"; break;
        case 4: value = "Operand error"; break;
        case 5: value = "Input buffer full"; break;
        case 6: value = "Number out of range"; break;
        case 7: value = "Command not valid while running"; break;
        case 8: value = "Command not valid while not running"; break;
        case 9: value = "Variable error"; break;
        case 10: value = "Empty program line or undefined label"; break;
        case 11: value = "Invalid label or line number"; break;
        case 12: value = "Subroutine more than 16 deep"; break;
        case 13: value = "JG only valid when running in jog mode"; break;
        case 14: value = "EEPROM check sum error"; break;
        case 15: value = "EEPROM write error"; break;
        case 16: value = "IP incorrect sign during position move or IP given during forced deceleration"; break;
        case 17: value = "ED, BN and DL not valid while program running"; break;
        case 18: value = "Command not valid when contouring"; break;
        case 19: value = "Application strand already executing"; break;
        case 20: value = "Begin not valid with motor off"; break;
        case 21: value = "Begin not valid while running"; break;
        case 22: value = "Begin not possible due to Limit Switch"; break;
        case 24: value = "Begin not valid because no sequence defined"; break;
        case 28: value = "S operand not valid"; break;
        case 29: value = "Not valid during coordinated move"; break;
        case 30: value = "Sequenct Segment Too Short"; break;
        case 31: value = "Total move distance in a sequence > 2 billion"; break;
        case 32: value = "Segment buffer full"; break;
        case 33: value = "VP or CR commands cannot be mixed with LI commands"; break;
        case 39: value = "No time specified"; break;
        case 41: value = "Contouring record range error"; break;
        case 42: value = "Contour data being sent too slowly"; break;
        case 46: value = "Gear axis both master and follower"; break;
        case 50: value = "Not enough fields"; break;
        case 51: value = "Question mark not valid"; break;
        case 52: value = "Missing \" or string too long"; break;
        case 53: value = "Error in {}"; break;
        case 54: value = "Question mark part of string"; break;
        case 55: value = "Missing [ or []"; break;
        case 56: value = "Array index invalid or out of range"; break;
        case 57: value = "Bad function or array"; break;
        case 58: value = "Bad command response"; break;
        case 59: value = "Mismatched parentheses"; break;
        case 60: value = "Download error - line too long or too many lines"; break;
        case 61: value = "Duplicate or bad label"; break;
        case 62: value = "Too many labels"; break;
        case 63: value = "IF statement without ENDIF"; break;
        case 66: value = "Array space full"; break;
        case 67: value = "Too many arrays or variables"; break;
        case 80: value = "Record mode already running"; break;
        case 81: value = "No array or source specified"; break;
        case 82: value = "Undefined Array"; break;
        case 83: value = "Not a valid number"; break;
        case 84: value = "Too many elements"; break;
        case 90: value = "Only A B C D valid operand"; break;
        case 96: value = "SM jumper needs to be installed for stepper motor operation (no Accelera, no RIO)"; break;
        case 97: value = "Bad Binary Command Format"; break;
        case 98: value = "Binary Commands not valid in application program"; break;
        case 99: value = "Bad binary command number"; break;
        case 100: value = "Not valid when running ECAM"; break;
        case 101: value = "Improper index into ET"; break;
        case 102: value = "No master axis defined for ECAM"; break;
        case 103: value = "Master axis modulus greater than 256 EP value"; break;
        case 104: value = "Not valid when axis performing ECAM"; break;
        case 105: value = "EB1 command must be given first"; break;
        case 106: value = "Privilege Violation"; break;
        case 110: value = "No hall effect sensors detected"; break;
        case 111: value = "Must be made brushless by BA command"; break;
        case 112: value = "BZ command timeout"; break;
        case 113: value = "No movement in BZ command"; break;
        case 114: value = "BZ command runaway"; break;
        case 118: value = "Controller has GL1600 not GL1800"; break;
        case 119: value = "Not valid for axis configured as stepper"; break;
        case 120: value = "Bad Ethernet transmit"; break;
        case 121: value = "Bad Ethernet packet received"; break;
        case 122: value = "Ethernet input buffer overrun"; break;
        case 123: value = "TCP lost sync"; break;
        case 124: value = "Ethernet handle already in use"; break;
        case 125: value = "No ARP response from IP address"; break;
        case 126: value = "Closed Ethernet handle"; break;
        case 127: value = "Illegal Modbus function code"; break;
        case 128: value = "IP address not valid"; break;
        case 130: value = "Remote IO command error"; break;
        case 131: value = "Serial Port Timeout"; break;
        case 132: value = "Analog inputs not present"; break;
        case 133: value = "Command not valid when locked / Handle must be UDP"; break;
        case 134: value = "All motors must be in MO for this command"; break;
        case 135: value = "Motor must be in MO"; break;
        case 164: value = "Exceeded maximum sequence length, BGS or BGT is required"; break;
        default: value = "Errore sconosciuto"; break;
    }

    return value;

}


