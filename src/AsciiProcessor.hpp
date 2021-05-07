/*
** ascii-webcam PROJECT, 2021
** AsciiProcessor.hpp
** File description:
** 
** Author:
** Arnaud Guerout
** https://github.com/Guerout-Arnaud
** Contributors:
**
*/

#ifndef ASCII_PROCESSOR_HPP_
    #define ASCII_PROCESSOR_HPP_

    class AsciiProcessor {
        public:
            AsciiProcessor(unsigned int videoWidth, unsigned int videoHeight, unsigned int outputWidth, unsigned int outputHeight);
            ~AsciiProcessor();
            void transformToAscii(uint8_t *videoBuffer, bool *isRunning);

        protected:

        private:
            uint8_t *tranformedBuffer;
            unsigned int videoWidth;
            unsigned int videoHeight;
            unsigned int outputWidth;
            unsigned int outputHeight;


            uint8_t const *getTransformedBuffer() const;
            void transformToAscii(uint8_t *videoBuffer, bool *isRunning);
            uint8_t avg(uint8_t *videoBuffer, int x, int y);

    };

#endif