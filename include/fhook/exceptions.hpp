/**
 * This file is part of the fhook library.
 * 
 * SPDX-License-Identifier: 0BSD
 * 
 * Copyright (C) 2022
 * Author: Yaroslav Yasynytskyi <yaroslav.yasynytskyi@gmail.com>
*/

/**
 * This file contains the declaration of all exceptions thrown by the library.
*/

#ifndef FHOOK_EXCEPTIONS_HPP
#define FHOOK_EXCEPTIONS_HPP

#include <stdexcept>

namespace fhook
{

    /**
     * The base virtual class of the exception. All other exceptions are inherited from it.
    */
    class BasicException : public std::exception
    {
        public:
            BasicException(ErrorCode errCode, char* description) : errCode(errCode), description(description) {}

            const char* what() const throw() { return description; }

            ErrorCode getErrorCode() const { return errCode; }

        private:
            ErrorCode errCode;
            const char* description;
    };


    /**
     * The exception that is thrown when an attempt to allocate memory fails.
    */
    class MemoryAllocateException : public BasicException
    {
        public:
            using BasicException::BasicException;
    };

    class MemoryDeallocateException : public BasicException
    {
        public:
            using BasicException::BasicException;
    };


    /**
     * The exception that is thrown when an attempt to change the memory access parameters fails.
    */
    class MemoryProtectException : public BasicException
    {
        public:
            using BasicException::BasicException;
    };

    /**
     * The exception that is thrown if there is not enough memory in the function bytecode to install the hook.
    */
    class NotEnoughMemoryException : std::exception
    {
        public:
            const char* what() const throw()
            {
                return "fhook: not enought memory to install the hook";
            }
    };
};

#endif // FHOOK_EXCEPTIONS_HPP