/*
** ascii-webcam PROJECT, 2021
** VideoCapture.hpp
** File description:
** 
** Author:
** Arnaud Guerout
** https://github.com/Guerout-Arnaud
** Contributors:
**
*/

#ifndef VIDEO_CAPTURE_HPP_
    #define VIDEO_CAPTURE_HPP_

    // #include <stdint.h>
    // #include <string>

    class VideoCapture {
        public:
            VideoCapture(std::string filepath);
            ~VideoCapture();
            const unsigned int getVideoWidth() const;
            const unsigned int getVideoHeight() const;
            uint8_t const *getVideoBuffer() const;

        protected:

        private:
            uint8_t *videoBuffer;
            unsigned int buffSize;
            int fd;
            unsigned int width;
            unsigned int height;

    };

#endif