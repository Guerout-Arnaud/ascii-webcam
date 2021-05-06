// #include <iostream>
// #include <fcntl.h>
// #include <sys/ioctl.h>
// #include <linux/videodev2.h>



// static int xioctl(int fd, int request, void *arg)
// {
//     int result = ioctl (fd, request, arg);
 
//     // do r = ioctl (fd, request, arg);
//     while (result == -1 && EINTR == errno) {
//         result = ioctl (fd, request, arg);
//     }
 
//     return result;
// }

// int print_caps(int fd)
// {
//     struct v4l2_capability caps = {0};
//     if (xioctl(fd, VIDIOC_QUERYCAP, &caps) == -1)
//     {
//         std::cerr << "Error Querying Capabilities" << std::endl;
//         return (-1);
//     }

//     printf( "Driver Caps:\n"
//             "  Driver: \"%s\"\n"
//             "  Card: \"%s\"\n"
//             "  Bus: \"%s\"\n"
//             "  Version: %d.%d\n"
//             "  Capabilities: %08x\n",
//             caps.driver,
//             caps.card,
//             caps.bus_info,
//             (caps.version>>16)&&0xff,
//             (caps.version>>24)&&0xff,
//             caps.capabilities
//     );


//     return (0);
// }

// int main(int argc, char **argv)
// {
//     int fd = -1;
 
//     fd = open("/dev/video0", O_RDWR);
//     if (fd == -1) {
//         std::cerr << "Error while opening video device" << std::endl;
//         return (-1);
//     }
//     if (print_caps(fd) == -1)
//         return (-1);
//     return (0);
// }


#include <opencv2/highgui.hpp>
#include <iostream>

int main(int argc, char **argv)
{
    int deviceID = 0;    
    int apiID = cv::CAP_ANY;
    cv::Mat frame = cv::Mat();
    cv::VideoCapture cap = cv::VideoCapture();

    cap.open(deviceID, apiID);

    if (cap.isOpened() == false) {
        std::cerr << "ERROR! Unable to open camera" << std::endl;
        return (-1);
    }

    for (;;) {
        cap.read(frame);
        if (frame.empty() == true) {
            std::cerr << "ERROR! blank frame grabbed" << std::endl;
            break;
        }



        if (cv::waitKey(16) >= 0)
            break;
        cv::imshow("Hello World !", frame);
    }
    return (0);
}