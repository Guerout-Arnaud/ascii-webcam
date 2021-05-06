#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <iostream>
#include "output.hpp"

uint8_t *buffer;
int IMAGEWIDTH;
int IMAGEHEIGHT;
uint8_t *mat;
uint8_t *out;


static int xioctl(int fd, int request, void *arg)
{
        int r;
 
        do r = ioctl (fd, request, arg);
        while (-1 == r && EINTR == errno);
 
        return r;
}
 
int print_caps(int fd)
{
        struct v4l2_capability caps = {0};
        if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &caps))
        {
                perror("Querying Capabilities");
                return 1;
        }
 
        printf( "Driver Caps:\n"
                "  Driver: \"%s\"\n"
                "  Card: \"%s\"\n"
                "  Bus: \"%s\"\n"
                "  Version: %d.%d\n"
                "  Capabilities: %08x\n",
                caps.driver,
                caps.card,
                caps.bus_info,
                (caps.version>>16)&&0xff,
                (caps.version>>24)&&0xff,
                caps.capabilities);
 
 
        struct v4l2_cropcap cropcap = {0};
        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (-1 == xioctl (fd, VIDIOC_CROPCAP, &cropcap))
        {
                perror("Querying Cropping Capabilities");
                return 1;
        }

        printf( "Camera Cropping:\n"
                "  Bounds: %dx%d+%d+%d\n"
                "  Default: %dx%d+%d+%d\n"
                "  Aspect: %d/%d\n",
                cropcap.bounds.width, cropcap.bounds.height, cropcap.bounds.left, cropcap.bounds.top,
                cropcap.defrect.width, cropcap.defrect.height, cropcap.defrect.left, cropcap.defrect.top,
                cropcap.pixelaspect.numerator, cropcap.pixelaspect.denominator);
 
        int support_grbg10 = 0;
 
        struct v4l2_fmtdesc fmtdesc = {0};
        fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        char fourcc[5] = {0};
        char c, e;
        printf("  FMT : CE Desc\n--------------------\n");
        while (0 == xioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc))
        {
                strncpy(fourcc, (char *)&fmtdesc.pixelformat, 4);
                if (fmtdesc.pixelformat == V4L2_PIX_FMT_SGRBG10)
                    support_grbg10 = 1;
                c = fmtdesc.flags & 1? 'C' : ' ';
                e = fmtdesc.flags & 2? 'E' : ' ';
                printf("  %s: %c%c %s\n", fourcc, c, e, fmtdesc.description);
                fmtdesc.index++;
        }
        /*
        if (!support_grbg10)
        {
            printf("Doesn't support GRBG10.\n");
            return 1;
        }*/
 
        struct v4l2_format fmt = {0};
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width = 1920;
        fmt.fmt.pix.height = 1080;
        //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_GREY;
        // fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
        // fmt.fmt.pix.field = V4L2_FIELD_NONE;
        
        if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
        {
            perror("Setting Pixel Format");
            return 1;
        }
 
        strncpy(fourcc, (char *)&fmt.fmt.pix.pixelformat, 4);
        printf( "Selected Camera Mode:\n"
                "  Width: %d\n"
                "  Height: %d\n"
                "  PixFmt: %s\n"
                "  Field: %d\n",
                fmt.fmt.pix.width,
                fmt.fmt.pix.height,
                fourcc,
                fmt.fmt.pix.field);

        IMAGEWIDTH = fmt.fmt.pix.width;
        IMAGEHEIGHT = fmt.fmt.pix.height;
        return 0;
}

int init_mmap(int fd)
{
    struct v4l2_requestbuffers req = {0};
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
 
    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req))
    {
        perror("Requesting Buffer");
        return 1;
    }
 
    struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if(-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
    {
        perror("Querying Buffer");
        return 1;
    }
 
    buffer = (uint8_t *)mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
    printf("Length: %d\nAddress: %p\n", buf.length, buffer);
    printf("Image Length: %d\n", buf.bytesused);
 
    return 0;
}

void print_img(uint8_t *img)
{
    int size = IMAGEWIDTH * IMAGEHEIGHT * 3;
    printf("Size = %d\n", sizeof(buffer)/sizeof(*buffer));
    
    for (int i = 0; i < size; i++) {
        printf("%d", img[i]);
        if ((i % 3) == 2) {
            printf("\n");
        } else {
            printf(", ");
        }
    }
    exit(0);
}

uint8_t *rgb2grayscale(uint8_t *rgb)
{
    double pixel_value = 0;
    int pixels = IMAGEWIDTH * IMAGEHEIGHT;
    uint8_t *grayscale = (uint8_t *)malloc(sizeof(*grayscale) * pixels);

    static int test = 0;
    if (test == 50) {
        print_img(rgb);
    }
    test++;
    // std::cout<< test++ << std::endl;
    return NULL;

    for (int i = 0; i < pixels; i++) {
        // printf("R:%d | G:%d | B:%d\n", rgb[i*3], rgb[(i * 3) + 1], rgb[(i * 3) + 2]);
        
        pixel_value = ((double)rgb[(i * 3)] * 0.2126  + (double)rgb[(i * 3) + 1] * 0.7152 + (double)rgb[(i * 3) + 2] * 0.0722) / 255;
        // printf("GrayScale (linar) = %f\n", pixel_value);
        
        if (pixel_value <= 0.0031308)
            pixel_value = 12.92 * pixel_value;
        else 
            pixel_value = 1.055 * pow(pixel_value, (double)(1 / 2.4)) - 0.055;
        grayscale[i] = (uint8_t)(pixel_value * 255);
        // printf("GrayScale = %d\n", (uint8_t)(pixel_value * 255));
    }
    // exit(0);
    return (grayscale);

}

 
void capture_image(int fd, VirtualOutput vo)
{
    struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if(-1 == xioctl(fd, VIDIOC_QBUF, &buf))
    {
        perror("Query Buffer");
        return;
    }
 
    if(-1 == xioctl(fd, VIDIOC_STREAMON, &buf.type))
    {
        perror("Start Capture");
        return;
    }
 
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    struct timeval tv = {0};
    tv.tv_sec = 2;
    int r = select(fd+1, &fds, NULL, NULL, &tv);
    if(-1 == r)
    {
        perror("Waiting for Frame");
        return;
    }
 
    if(-1 == xioctl(fd, VIDIOC_DQBUF, &buf))
    {
        perror("Retrieving Frame");
        return;
    }
    // printf ("saving image\n");

    // uint8_t *grayscale = rgb2grayscale(buffer);
    // printf("%p\n", grayscale);
    
    cv::_InputArray pic_arr(buffer, IMAGEWIDTH * IMAGEHEIGHT * 3);
    cv::Mat mat = cv::imdecode(pic_arr, cv::IMREAD_GRAYSCALE);
    imshow("view", mat);
    // vo.send(buffer);
    // cv::_OutputArray out_img;
    // mat.convertTo(out_img, sizeof(uint8_t *));
    // VirtualOutput vo = VirtualOutput(64, 64, libyuv::FOURCC_RAW, "/dev/video2");
    // imwrite("output.jpg",out_img);
    cv::waitKey(16);

    // return mat;
}

 
int main()
{
    
    cv::Mat mat;

        int fd;
 
        fd = open("/dev/video0", O_RDWR);
        if (fd == -1)
        {
                perror("Opening video device");
                return 1;
        }
        if (print_caps(fd))
            return 1;
        printf("%d\t%d\n", IMAGEWIDTH, IMAGEHEIGHT);
        VirtualOutput vo = VirtualOutput(IMAGEWIDTH, IMAGEHEIGHT, libyuv::FOURCC_J400, "/dev/video2");
        if(init_mmap(fd))
            return 1;
        int i;
        for(;;)
        {
            capture_image(fd, vo);            
            
            // vo.send(mat.data);
            
        }
        close(fd);
        return 0;
}
