/*
** ascii-webcam PROJECT, 2021
** VideoCapture.cpp
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
#include <ios>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>

#include "linux/VideoCapture.hpp"

VideoCapture::VideoCapture(std::string videoFilepath, unsigned int width = 1920, unsigned int height = 1080)
{
    this->buff = NULL;
    this->buffSize = 0;
    this->fd = -1;
    this->width = width;
    this->height = height;

    std::cout << "Width:" << width << std::endl; 
    std::cout << "Height:" << height << std::endl; 
    
    std::cout << "this->Width:" << this->width << std::endl;
    std::cout << "this->Height:" << this->height << std::endl;

    struct v4l2_capability caps = {0};
    struct v4l2_cropcap cropcap = {0};
    struct v4l2_format fmt = {0};
    struct v4l2_fmtdesc fmtdesc = {0};
    struct v4l2_requestbuffers req = {0};
    struct v4l2_buffer buf = {0};
    char fourcc[5] = {0};
    char c = '\0';
    char e = '\0';

    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = this->width;
    // fmt.fmt.pix.width = 1080;
    fmt.fmt.pix.height = this->height;
    // fmt.fmt.pix.height = 720;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;

    this->fd = open(videoFilepath.c_str(), O_RDWR);
    if (fd == -1)
        throw new std::runtime_error("Open File. Unable to open ");

    if (ioctl(fd, VIDIOC_QUERYCAP, &caps) == -1)
        throw new std::runtime_error("Querying Capabilities. Video device may not be compatible");

    if (ioctl (fd, VIDIOC_CROPCAP, &cropcap) == -1)
        throw new std::runtime_error("Querying Cropping Capabilities. Video input cannot be resized");

    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
        throw new std::runtime_error("Setting Pixel Format. Unable to set video format");
    
    if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1)
        throw new std::runtime_error("Requesting Buffer. Unable to set memory segment");

    if(ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1)
        throw new std::runtime_error("Querying Buffer. Unable to set memory segment");

    std::cout << "Driver Caps:" << std::endl;
    std::cout << "\tDriver:\t\"" << caps.driver << "\"" << std::endl;
    std::cout << "\tCard:\t\"" << caps.card << "\"" << std::endl;
    std::cout << "\tBus:\t\"" << caps.bus_info << "\"" << std::endl;
    std::cout << "\tVersion:\t" << ((caps.version >> 16) && 0xff) << "." << ((caps.version >> 24) && 0xff) << std::endl;
    std::cout << "\tCapabilities:\t" << std::hex << caps.capabilities << std::endl;

    std::cout << "Camera Cropping:" << std::endl;
    std::cout << "\tBounds:\t" << cropcap.bounds.width << "x" << cropcap.bounds.height << "+" << cropcap.bounds.left << "+" << cropcap.bounds.top << std::endl;
    std::cout << "\tDefault:\t" << cropcap.defrect.width << "x" << cropcap.defrect.height << "+" << cropcap.defrect.left << "+" << cropcap.defrect.top << std::endl;
    std::cout << "\tAspect:\t" << cropcap.pixelaspect.numerator << "/" << cropcap.pixelaspect.denominator << std::endl;
    std::cout << "\tFMT : CE Desc\n--------------------" << std::endl;
    
    while (ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc) == 0)
    {
            strncpy(fourcc, (char *)&fmtdesc.pixelformat, 4);
            c = fmtdesc.flags & 0b01 ? 'C' : ' ';
            e = fmtdesc.flags & 0b10 ? 'E' : ' ';
            std::cout << "\t" << fourcc << ": " << c << e << " " << fmtdesc.description << std::endl;
            fmtdesc.index++;
    }
    
    strncpy(fourcc, (char *)&fmt.fmt.pix.pixelformat, 4);
    std::cout << "Selected Camera Mode:" << std::endl;
    std::cout << "\tWidth:\t" << std::to_string(fmt.fmt.pix.width) << std::endl;
    std::cout << "\tHeight:\t" << std::to_string(fmt.fmt.pix.height) << std::endl;
    std::cout << "\tPixFmt:\t" << fourcc << std::endl;
    std::cout << "\tField:\t" << fmt.fmt.pix.field << std::endl;

    this->width = fmt.fmt.pix.width;
    this->height = fmt.fmt.pix.height;
    this->videoBuffer = cv::Mat(this->height, this->width, CV_8UC1, cv::Scalar(0, 0, 0));

    this->buffSize = buf.length;
    this->buff = (uint8_t *)mmap (NULL, this->buffSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
    std::cout << "Length: " << buf.length << "\nAddress: " << buff << std::endl;
    std::cout << "Image Length: " << buf.bytesused << std::endl;
    std::cout << "buffSize: " << buffSize << std::endl;
}

VideoCapture::~VideoCapture()
{
    close(this->fd);
    munmap(this->buff, this->buffSize);
}

const unsigned int VideoCapture::getVideoWidth() const {
    return (this->width);
}

const unsigned int VideoCapture::getVideoHeight() const {
    return (this->height);
}

uint8_t const *VideoCapture::getVideoBuffer() const {
    return (this->videoBuffer.data);
}

void VideoCapture::captureVideo()
{
    struct v4l2_buffer buf = {0};
    fd_set fds = {0};
    struct timeval tv = {0};
    int r = -1;
    
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;

    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) {
        perror("Query Buffer");
        return;
    }
 
    if (ioctl(fd, VIDIOC_STREAMON, &buf.type) == -1) {
        perror("Start Capture");
        return;
    }
    
    r = select(fd + 1, &fds, NULL, NULL, &tv);
    if (r == -1) {
        perror("Waiting for Frame");
        return;
    }
 
    if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1) {
        perror("Retrieving Frame");
        return;
    }

    cv::_InputArray pic_arr(buff, width * height * 3);
    cv::imdecode(pic_arr, cv::IMREAD_GRAYSCALE, &videoBuffer);
}

cv::Mat VideoCapture::getMat()
{
    return (this->videoBuffer);
}
