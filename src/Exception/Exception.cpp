/*
** ascii-webcam PROJECT, 2021
** Exception.cpp
** File description:
** 
** Author:
** Arnaud Guerout
** https://github.com/Guerout-Arnaud
** Contributors:
**
*/

#ifndef EXCEPTION_H_
    #define EXCEPTION_H_

    #include <exception>
    #include <string>

    class Exception : public std::exception {
        public:
            Exception(std::string message);
            Exception(const Exception &inital);
            ~Exception();
            const Exception &operator=(const Exception &rhs);

        protected:

        private:
    };


#endif