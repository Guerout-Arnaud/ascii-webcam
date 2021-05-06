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

    #include <stdint.h>
    #include <string>

    class VideoCapture {
        public:
            VideoCapture(std::string filepath);
            VideoCapture(const VideoCapture &inital);
            ~VideoCapture();
            const VideoCapture &operator=(const VideoCapture &rhs);    

        protected:

        private:
            uint8_t *videoBuffer;
            int fd;
            int width;
            int height;

            int xioctl(int request, void *arg);
            int print_caps(int fd);

    };

#endif