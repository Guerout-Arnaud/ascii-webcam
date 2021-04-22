#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
using namespace cv;
using namespace std;

static int COLS = 210;
static int ROWS = 52;

uchar *buffer = (uchar *) malloc(sizeof(*buffer) * ((ROWS * COLS) + (COLS + 1 )));

  
char grayToChar(int gray) {
    char *CHAR_LIST = " .:-=+*#%@";
    // char *CHAR_LIST = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
    // char *CHAR_LIST = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrcnuvczXYUJCLQ0OZmwqpdnkhao*#MW&8%B@$";
    int num_chars = strlen(CHAR_LIST);
    int grayScale = (gray * num_chars) / 255;

    return (grayScale < (num_chars - 1) ? CHAR_LIST[grayScale] : CHAR_LIST[num_chars - 1]);
}


uchar avg(Mat frame, int x, int y)
{
    int cell_width = frame.size().width / COLS;
    int cell_height = frame.size().height / ROWS;

    int x_init = x * cell_width;
    int y_init = y * cell_height;
    
    
    uint grayscale = 0;

    for (int i = 0; i < cell_height; i++) {
        for (int j = 0; j < cell_width; j++) {
            grayscale = grayscale + frame.at<uchar>(
                ((y_init + i) < frame.size().height) ? (y_init + i) : frame.size().height,
                ((x_init + j) < frame.size().width) ? (x_init + j) : frame.size().width
            );
        }
    }
    return (grayscale / (cell_width * cell_height));
}


void toAscii(Mat frame)
{
    // Mat grayscale = Mat();
    cv::cvtColor(frame, frame, COLOR_BGR2GRAY);

    int height = frame.size().height;
    int width = frame.size().width;
    
    if (COLS > width || ROWS > height)
        return;
            
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            buffer[(i * COLS) + i + j] = grayToChar(avg(frame, j, i));
            
        }
        buffer[(i * COLS) + i + COLS] = '\0';
        
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            std::cout << buffer[(i * COLS) + j];
        }
        std::cout << std::endl; // << buffer[(i * COLS) + COLS];
    }
}


int main(int argc, char **argv)
{
    Mat frame = Mat();
    VideoCapture cap = VideoCapture();

    int deviceID = 0;    
    int apiID = cv::CAP_ANY;
    cap.open(deviceID, apiID);

    if (cap.isOpened() == false) {
        std::cerr << "ERROR! Unable to open camera" << std::endl;
        return (-1);
    }
    
    for (;;)
    {
        cap.read(frame);
        if (frame.empty() == true) {
            cerr << "ERROR! blank frame grabbed" << std::endl;
            break;
        }

        // cv::putText(frame, //target image
        //     "Hello, OpenCV!", //text
        //     cv::Point(10, frame.rows / 2), //top-left position
        //     cv::FONT_HERSHEY_DUPLEX,
        //     1.0,
        //     CV_RGB(118, 185, 0), //font color
        //     2);



        if (cv::waitKey(16) >= 0)
            break;
        // frame = 
        toAscii(frame);
        
        // int v = frame.size().height / (ROWS + 1);
        // int w = frame.size().width / (COLS + 1);


        // for (int i = 0; i < ROWS; i++) {
        //     cv::putText(frame,                  //target image
        //         (char *) &(buffer[(i * COLS) + i]),               //text
        //         cv::Point((w / 2), ((v / 2) + (i * v))),                    //top-left position
        //         cv::FONT_HERSHEY_SIMPLEX,
        //         1.0,
        //         CV_RGB(255, 255, 255),          //font color
        //         1);
        // }
        cv::imshow("Hello!", frame);

    }
    return (0);
}