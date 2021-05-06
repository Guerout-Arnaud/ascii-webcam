/*
** ascii-webcam PROJECT, 2021
** video_capture.cpp
** File description:
** 
** Author:
** Arnaud Guerout
** https://github.com/Guerout-Arnaud
** Contributors:
**
*/

#include <iostream>
#include <stdexcept>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include "VideoCapture.hpp"

VideoCapture::VideoCapture(std::string filepath)
{
    struct v4l2_capability caps = {0};
    struct v4l2_cropcap cropcap = {0};
    struct v4l2_format fmt = {0};
    struct v4l2_fmtdesc fmtdesc = {0};
    char fourcc[5] = {0};
    char c = '\0';
    char e = '\0';

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 1280;
    fmt.fmt.pix.height = 720;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (ioctl(fd, VIDIOC_QUERYCAP, &caps) == -1)
        throw new std::runtime_error("Querying Capabilities. Video device may not be compatible");

    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl (fd, VIDIOC_CROPCAP, &cropcap) == -1)
        throw new std::runtime_error("Querying Cropping Capabilities. Video input cannot be resized");

    if (-1 == ioctl(fd, VIDIOC_S_FMT, &fmt))
        throw new std::runtime_error("Setting Pixel Format. Unable to set video format");
    
    strncpy(fourcc, (char *)&fmt.fmt.pix.pixelformat, 4);

    std::cout << "Driver Caps:" << std::endl;
    std::cout << "\tDriver:\t\"" << caps.driver << "\"" << std::endl;
    std::cout << "\tCard:\t\"" << caps.card << "\"" << std::endl;
    std::cout << "\tBus:\t\"" << caps.bus_info << "\"" << std::endl;
    std::cout << "\tVersion:\t" << ((caps.version >> 16) && 0xff) << "." << ((caps.version >> 24) && 0xff) << std::endl;
    std::cout << "\tCapabilities:\t%08x" << std::endl;
            caps.capabilities);

    printf( "Camera Cropping:\n"
            "\tBounds:\t%dx%d+%d+%d\n"
            "\tDefault:\t%dx%d+%d+%d\n"
            "\tAspect:\t%d/%d\n",
            cropcap.bounds.width, cropcap.bounds.height, cropcap.bounds.left, cropcap.bounds.top,
            cropcap.defrect.width, cropcap.defrect.height, cropcap.defrect.left, cropcap.defrect.top,
            cropcap.pixelaspect.numerator, cropcap.pixelaspect.denominator);
    printf("  FMT : CE Desc\n--------------------\n");
    
    while (ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc) == 0)
    {
            strncpy(fourcc, (char *)&fmtdesc.pixelformat, 4);
            c = fmtdesc.flags & 0b01 ? 'C' : ' ';
            e = fmtdesc.flags & 0b10 ? 'E' : ' ';
            std::cout << "  %s: %c%c %s\n", , c, e, fmtdesc.description);
            fmtdesc.index++;
    }

    
    printf( "Selected Camera Mode:\n"
            "\tWidth:\t%d\n"
            "\tHeight:\t%d\n"
            "\tPixFmt:\t%s\n"
            "\tField:\t%d\n",
            fmt.fmt.pix.width,
            fmt.fmt.pix.height,
            fourcc,
            fmt.fmt.pix.field);

}

VideoCapture::VideoCapture(const VideoCapture &initial)
{

    
}

VideoCapture::~VideoCapture()
{
}

const VideoCapture &VideoCapture::operator=(const VideoCapture &rhs)
{
    return *this;
}

int VideoCapture::xioctl(int request, void *arg)
{
        int result = ioctl (fd, request, arg);

        while (result == -1 && errno == EINTR) {
            result = ioctl (fd, request, arg);
        }
        return (result);
}

int VideoCapture::print_caps(int fd)
{
    
 
    

    

    
    int support_grbg10 = 0;

    
        /*
    if (!support_grbg10)
    {
        printf("Doesn't support GRBG10.\n");
        return 1;
    }*/

    

    

    return 0;
}