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
    class BasicException
    {
        public:
            BasicException(ErrorCode errCode) : errCode(errCode) {}

            virtual const char* what() const throw() = 0;

            ErrorCode getErrorCode() const
            {
                return errCode;
            }

        private:
            ErrorCode errCode;
    };


    /**
     * The exception that is thrown when an attempt to allocate memory fails.
    */
    class MemoryAllocateException : public BasicException
    {
        public:
            using BasicException::BasicException;

            const char* what() const throw()
            {
                return "fhook: cannot allocate memory";
            }
    };

    class MemoryDeallocateException : public BasicException
    {
        public:
            using BasicException::BasicException;

            const char* what() const throw()
            {
                return "fhook: cannot deallocate memory";
            }
    };


    /**
     * The exception that is thrown when an attempt to change the memory access parameters fails.
    */
    class MemoryProtectException : public BasicException
    {
        public:
            using BasicException::BasicException;

            const char* what() const throw()
            {
                return "fhook: memory region cannot be (un)protected";
            }
    };

    /**
     * The exception that is thrown if there is not enough memory in the function bytecode to install the hook.
    */
    class NotEnoughMemoryException : public BasicException
    {
        public:
            NotEnoughMemoryException() : BasicException(getErrorCode()) {}

            const char* what() const throw()
            {
                return "fhook: not enought memory to install the hook";
            }
    };
};

#endif // FHOOK_EXCEPTIONS_HPP