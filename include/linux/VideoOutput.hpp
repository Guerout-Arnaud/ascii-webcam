/*
** ascii-webcam PROJECT, 2021
** VideoOutput.hpp
** File description:
** Video 4 Linux 2 LoopBack implementation prototypes
** 
** This file is licensed under the 
** GNU General Public License v2.0
** It's original content made by letmaik can be found here :
** https://github.com/letmaik/pyvirtualcam/blob/master/pyvirtualcam/native_linux_v4l2loopback/virtual_output.h
** 
** Author:
** Arnaud Guerout
** https://github.com/Guerout-Arnaud
** Contributors:
**
*/

#ifndef VIDEO_OUTPUT_H_
    #define VIDEO_OUTPUT_H_

    #include <string>
    #include <vector>
    #include <stdint.h>

    class VideoOutput {
        public:
            VideoOutput(unsigned int width, unsigned int height);
            VideoOutput(unsigned int width, unsigned int height, std::string v4l2LbFilePath);
            ~VideoOutput();

            void sendOutput(const uint8_t *frame);

        protected:

        private:
            int fd;
            unsigned int frameSize;

            void openStream(std::string v4l2LbFilePath);

    };


#endif