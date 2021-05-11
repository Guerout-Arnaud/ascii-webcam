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

    #include <string>
    #include <cstdint>

    class VideoCapture {
        public:
            VideoCapture(std::string videoFilepath, unsigned int width, unsigned int height);
            ~VideoCapture();
            const unsigned int getVideoWidth() const;
            const unsigned int getVideoHeight() const;
            uint8_t const *getVideoBuffer() const;

        protected:

        private:
            int fd;
            unsigned int width;
            unsigned int height;
            unsigned int buffSize;
            uint8_t *videoBuffer;
    };

#endif