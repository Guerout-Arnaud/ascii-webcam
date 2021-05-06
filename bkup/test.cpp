#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "bitmask_16.hpp"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdlib.h>
#include <iostream>
#include <stdio.h>


const char *CHAR_LIST = " .:-=+*#%@";

static int MASK_HEIGHT = 16;
static int MASK_WIDTH = 16;

static int IMGS_HEIGHT = 2;
static int IMGS_WIDTH = 2;

int main() {
    cv::Mat frame = cv::Mat(IMGS_HEIGHT * MASK_HEIGHT, IMGS_WIDTH * MASK_WIDTH, CV_8UC1);
    
    for (int y = 0; y < IMGS_HEIGHT * MASK_HEIGHT; y++) {
        for (int x = 0; x < IMGS_WIDTH * MASK_WIDTH; x++) {
            frame.data[(y * IMGS_WIDTH * MASK_WIDTH) + x] = 0;
        }
    }

    const char *mask = bitmask[9];

    // printf("%s\n", mask);


    for (int i = 0; i < IMGS_HEIGHT; i++) {
        for (int j = 0; j < IMGS_WIDTH; j++) {
            // getmask

            for (int y = 0; y < MASK_HEIGHT; y++) {
                for (int x = 0; x < MASK_WIDTH; x++) {
                    if (mask[(y * MASK_WIDTH) + x] == '0')
                        frame.data[(i * MASK_HEIGHT * MASK_WIDTH * IMGS_WIDTH) + (j * MASK_WIDTH) + (y * MASK_WIDTH * IMGS_WIDTH) + x] = 0;
                    else 
                        frame.data[(i * MASK_HEIGHT * MASK_WIDTH * IMGS_WIDTH) + (j * MASK_WIDTH) + (y * MASK_WIDTH * IMGS_WIDTH) + x] = 255;
                }
            }
            // break; 
        }
        // break; 
    }


// // int i = 0; 
//     for (int x = 0, y = 0; (x + (y * MASK_WIDTH)) < (MASK_HEIGHT * MASK_WIDTH); x++) {
//         // i++;
//         if (mask[x + (y * (MASK_WIDTH + 1))] == '\n') {
//             // printf("x = %d | y = %d | i = %d\n", x, y, i);
//             x = 0;
//             y = y + 1;
//         }
//         if (mask[x + (y * (MASK_WIDTH + 1))] == '0')
//             frame.data[x + (y * MASK_WIDTH)] = 0;
//         else
//             frame.data[x + (y * MASK_WIDTH)] = 255;

//     }



    // cv::putText(frame,
    //             ".",
    //             cv::Point(0, 10),
    //             cv::FONT_HERSHEY_PLAIN,
    //             1,
    //             255,
    //             1);

    // for (int i = 0; i < MASK_HEIGHT; i++) {
    //     for (int j = 0; j < MASK_WIDTH; j++) {
    //         if (frame.data[(i*MASK_HEIGHT) + j])
    //             printf("1");
    //         else 
    //             printf("0");
    //     }
    //     printf("\n");
    // }
    for (;;) {
        cv::imshow("Frame", frame);
        if (cv::waitKey(0))
            break;
    }
    return (0);
}