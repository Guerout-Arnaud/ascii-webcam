/*
** ascii-webcam PROJECT, 2021
** VideoOutput.cpp
** File description:
** Video 4 Linux 2 LoopBack implementation to have a virtual webcam
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

#include <iostream>

#include <string>
#include <filesystem>
#include <stdexcept>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

#include "linux/VideoOutput.hpp"

VideoOutput::VideoOutput(unsigned int width, unsigned int height)
{
    fd = -1;
    frameSize = width * height;

    size_t outputId = 0;
    v4l2_format v4l2_fmt = {0};
    v4l2_pix_format &pix = v4l2_fmt.fmt.pix;

    v4l2_fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    pix.width = width;
    pix.height = height;
    pix.pixelformat = V4L2_PIX_FMT_GREY;

    for (outputId = 0; fd == -1; outputId++) {
        std::ostringstream v4l2LbFilePath;
        v4l2LbFilePath << "/dev/video" << outputId;
        if (std::filesystem::exists(v4l2LbFilePath.str()) == false)
            throw std::runtime_error(
                "No v4l2 loopback device found.\n"
                "Did you run 'modprobe v4l2loopback'?\n");
        try {
            openStream(v4l2LbFilePath.str());
        } catch (std::exception &ex) {
            continue;
        }
    }

    if (ioctl(fd, VIDIOC_S_FMT, &v4l2_fmt) == -1) {
        close(fd);
        throw std::runtime_error(
            "Virtual camera /dev/video" + std::to_string(outputId) +
            " device could not be configured:\n" + std::string(strerror(errno))
        );
    }
}

VideoOutput::VideoOutput(unsigned int width, unsigned int height, std::string v4l2LbFilePath)
{
    fd = -1;
    frameSize = width * height;

    size_t outputId = 0;
    v4l2_format v4l2_fmt = {0};
    v4l2_pix_format &pix = v4l2_fmt.fmt.pix;

    v4l2_fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    pix.width = width;
    pix.height = height;
    pix.pixelformat = V4L2_PIX_FMT_GREY;

    try {
        openStream(v4l2LbFilePath);
    } catch (std::exception &ex) {
        throw ex;
    }

    if (ioctl(fd, VIDIOC_S_FMT, &v4l2_fmt) == -1) {
        close(fd);
        throw std::runtime_error(
            "Virtual camera /dev/video" + std::to_string(outputId) +
            " device could not be configured:\n" + std::string(strerror(errno))
        );
    }
}

VideoOutput::~VideoOutput()
{
    if (fd != -1)
        close(fd);
}

void VideoOutput::openStream(std::string v4l2LbFilePath)
{
    struct v4l2_capability camera_cap = {0};

    fd = open(v4l2LbFilePath.c_str(), O_WRONLY | O_SYNC);
    if (fd == -1) {
        if (errno == EACCES) {
            throw std::runtime_error(
                "Missing permissions while openning " + v4l2LbFilePath + "\n"
                "Did you add your user to the 'video' group?\n"
                "Run 'usermod -a -G video myusername' and log out and in again.\n"
            );
        } else if (errno == ENOENT) {
            throw std::invalid_argument(
                "Device " + v4l2LbFilePath + " does not exist."
            );
        } else {
            throw std::invalid_argument(
                "Device " + v4l2LbFilePath + " could not be opened: " +
                std::string(std::strerror(errno))
            );
        }
    }

    try {
        if (ioctl(fd, VIDIOC_QUERYCAP, &camera_cap) == -1) {
            throw std::invalid_argument(
                "Device capabilities of " + v4l2LbFilePath + " could not be queried."
            );
        }
        if (camera_cap.capabilities & V4L2_CAP_VIDEO_OUTPUT == 0) {
            throw std::invalid_argument(
                "Device " + v4l2LbFilePath + " is not a video output device."
            );
        }
        if (strcmp((const char*)camera_cap.driver, "v4l2 loopback") != 0) {
            throw std::invalid_argument(
                "Device " + v4l2LbFilePath + " is not a V4L2 device."
            );
        }
    } catch (std::exception &ex) {
        close(fd);
        fd = -1;
        throw ex;
    }
}

void VideoOutput::sendOutput(const uint8_t *frame)
{
    ssize_t n = write(fd, const_cast<uint8_t*>(frame), frameSize);
    if (n == -1) {
        // not an exception, in case it is temporary
        fprintf(stderr, "error writing frame: %s", strerror(errno));
    }
}
