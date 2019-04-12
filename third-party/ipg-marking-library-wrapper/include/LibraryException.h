#pragma once

#include <exception>
#include <iostream>

namespace ipg_marking_library_wrapper {

    class LibraryException : public std::exception {
    private:
        std::string descr;

    public:
        explicit LibraryException(const std::string& descr) :
            descr(descr) { }

        const char* what() const {
            return descr.c_str();
        }

    };

}