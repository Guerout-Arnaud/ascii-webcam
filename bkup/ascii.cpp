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

const char *CHAR_LIST = " .:-=+*#%@";
// char *CHAR_LIST = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
// char *CHAR_LIST = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrcnuvczXYUJCLQ0OZmwqpdnkhao*#MW&8%B@$";

Mat **imgs = (Mat **)malloc(sizeof(*imgs) * strlen(CHAR_LIST));
std::vector<cv::Mat> imgsVector = std::vector<cv::Mat>();


char grayToChar(int gray) {

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


uchar *toAscii(Mat frame)
{
    // Mat grayscale = Mat();
    cv::cvtColor(frame, frame, COLOR_BGR2GRAY);

    int height = frame.size().height;
    int width = frame.size().width;
    static uchar *buffer = (uchar *) malloc(sizeof(*buffer) * ((ROWS * COLS) + (COLS + 1)));
    char *character = NULL;
    uint8_t *out = (uint8_t *) malloc(sizeof(*out) * ((ROWS * 16) * (COLS * 16)));
    
    if (COLS > width || ROWS > height)
        return;
            
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            character = NULL; //grayToChar(avg(frame, j, i));
            for (int y = 0; y < 16; y++) {
                for (int x = 0; x < 16; x++) {
                    out 
                }
            }

            buffer[(i * COLS) + i + j] = 
            
        }
        buffer[(i * COLS) + i + COLS] = '\n';
        // buffer[(i * COLS) + i + COLS] = '\0';
    }
    return (buffer);
    // for (int i = 0; i < ROWS; i++) {
    //     for (int j = 0; j < COLS; j++) {
    //         std::cout << buffer[(i * COLS) + j];
    //     }
    //     std::cout << buffer[(i * COLS) + COLS];
    // }
}