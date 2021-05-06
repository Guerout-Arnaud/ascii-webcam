#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdlib.h>
#include <iostream>
#include <stdio.h>

int main() {
    cv::Mat tmp = cv::Mat(64, 64, CV_8UC1, cv::Scalar(0, 0, 0));


    // " .:-=+*#%@"
    cv::putText(tmp,
    "%", 
    cv::Point(0, 0), 
    cv::FONT_HERSHEY_PLAIN,
    5, 
    CV_RGB(255, 255, 255),
    1,
    8,
    true
    );

    for (int y = 0; y < tmp.size().height; y++) {
        for (int x = 0; x < tmp.size().width; x++) {
            uchar val = tmp.at<uchar>(y,x);
            if (val == 0)
                printf("0");
                // printf(" ");
            else
                printf("1");
                // printf("\e[47m \e[0m");
            // printf("%d,", val);
        }
        // printf("|\n");
        printf("\n");
    }

    // for (uchar *it = tmp.data; it < tmp.dataend; it++) {
    //     if (((long int)it % ((tmp.dataend - tmp.data) / 64)) == 0 )
    //         printf("\n");
    //     printf("%d,");
    // };


    for (;;) {
        cv::imshow("Dot", tmp);
        if (cv::waitKey(16) >= 0)
            break;
    };

}