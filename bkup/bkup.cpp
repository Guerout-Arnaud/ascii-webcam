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

uchar *buffer = (uchar *) malloc(sizeof(*buffer) * ((ROWS * COLS) + (COLS + 1 )));
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
        buffer[(i * COLS) + i + COLS] = '\n';
        // buffer[(i * COLS) + i + COLS] = '\0';
        
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            std::cout << buffer[(i * COLS) + j];
        }
        std::cout << buffer[(i * COLS) + COLS];
    }
}


// def generate_ascii_letters():
//     images = []
//     #letters = "# $%&\\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~"
//     letters = " \\ '(),-./:;[]_`{|}~"
//     for letter in letters:
//         img = np.zeros((12, 16), np.uint8)
//         img = cv2.putText(img, letter, (0, 11), cv2.FONT_HERSHEY_SIMPLEX, 0.5, 255)
//         images.append(img)
//     return np.stack(images)


void create_ascii_imgs() {
    char c[2] = {0};
    for (int i = 0; i < strlen(CHAR_LIST); i++ ) {
        c[0] = CHAR_LIST[i];
        // printf("%s\n", c);
        imgs[i] = new cv::Mat(64, 64, CV_8UC3);
        cv::putText(*(imgs[i]), c, cv::Point(31, 31), FONT_HERSHEY_PLAIN, 0.5, 
        CV_RGB(255, 255, 255), 
        1
        // 300
        );

        cv::cvtColor(*(imgs[i]), *(imgs[i]), COLOR_BGR2GRAY);


        // cv::Mat tmp = Mat(64, 64, CV_8UC3);
        // cv::putText(tmp, c, cv::Point(31, 31), FONT_HERSHEY_PLAIN, 1, 
        // CV_RGB(255, 255, 255), 
        // 1
        // // 300
        // );

        // cv::cvtColor(tmp, tmp, COLOR_BGR2GRAY);

        // imgsVector.push_back(tmp);
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

    create_ascii_imgs();
    
    // for (int idx = 0; idx < strlen(CHAR_LIST); idx++) {
    //     printf("Char : '%c'\n", CHAR_LIST[idx]);
    //     for (int y = 0; y < (*(imgs[idx])).size().height; y++) {
    //         for (int x = 0; x < (*(imgs[idx])).size().width; x++) {
    //             int val = (*(imgs[idx])).at<uchar>(y, x);
    //             if (val == 0)
    //                 printf(" ");
    //             else
    //                 printf("\e[47m \e[0m");
    //         }
    //         printf("|\n");
    //     }
    //     printf("\n\n\n");

    // }

    // for (auto it = imgsVector.cbegin(); it < imgsVector.cend(); it++) {
    //     for (int y = 0; y < (*it).size().height; y++) {
    //         for (int x = 0; x < (*it).size().width; x++) {
    //             int val = (*it).data[(y * (*it).size().width) + x ];
    //             // if (val == 0)
    //             //     printf(" ");
    //             // else
    //             //     printf("\e[47m \e[0m");
    //             printf("%d,", val);
    //         }
    //         // printf("|\n");
    //         printf("\n");
    //     }
    //     printf("\n");
    //     printf("\n");
    //     printf("\n");
    // }
    // exit(0);

    // for (;;) {
    //     cv::imshow("Space",     *(imgs[0]));
    //     cv::imshow("Dot",       *(imgs[1]));
    //     cv::imshow("DDot",      *(imgs[2]));
    //     cv::imshow("Dash",      *(imgs[3]));
    //     cv::imshow("Equals",    *(imgs[4]));
    //     cv::imshow("Add",       *(imgs[5]));
    //     cv::imshow("Staar",     *(imgs[6]));
    //     cv::imshow("Sharp",     *(imgs[7]));
    //     cv::imshow("Percent",   *(imgs[8]));
    //     cv::imshow("At",        *(imgs[9]));
    //     if (cv::waitKey(16) >= 0)
    //         break;
    // };


    for (;;)
    {
        cap.read(frame);
        if (frame.empty() == true) {
            cerr << "ERROR! blank frame grabbed" << std::endl;
            break;
        }

        // // cv::putText(frame, //target image
        // //     c, //text
        // //     cv::Point(0, 11),
        // //     FONT_HERSHEY_SIMPLEX, 
        // //     0.5,
        // //     // CV_RGB(133, 133, 133),
        // //     255);

        // // std::cout << typeToString(frame.type());
        // // exit(0);

        if (cv::waitKey(16) >= 0)
            break;
        // // frame = 
        toAscii(frame);
        
        // int v = frame.size().height / (ROWS + 1);
        // int w = frame.size().width / (COLS + 1);

        // int height = frame.size().height;
        // int width = frame.size().width;

        // // cv::Mat testFrame = cv::Mat((ROWS * 12), (COLS * 16), CV_8UC3);
        cv::Mat testFrame = cv::Mat(720, 1080, CV_8UC3);
        

        // // .:-=+*#%@
        // char c[COLS] = {0};
        cv::HersheyFonts font = FONT_HERSHEY_PLAIN;

        // for (int i = 0; i < ROWS; i++) {
        //     for (int j = 0; j < COLS; j++) {
        //         cv::Mat ascii = select_letter(buffer[(i * COLS) + j]);
        //         for (int y = 0; y < ascii.size().height; y++) {
        //             for (int x = 0; x < ascii.size().width; x++) {
        //                 video_buffer[(i * 12) + (j * 16) + (y * ascii.size().width) + x];
        //             }                
        //         }
        //     }

        // }


        // for (int idx = 0; idx < strlen(CHAR_LIST); idx++) {
        //     printf("Char : '%c'\n", CHAR_LIST[idx]);
        //     for (int y = 0; y < (*(imgs[idx])).size().height; y++) {
        //         for (int x = 0; x < (*(imgs[idx])).size().width; x++) {
        //             printf("%d,", (*(imgs[idx])).at<uchar>(y, x));
        //         }
        //         printf("\n");
        //     }
        //     printf("\n\n\n");
        // }


        // for (int i = 0; i < ROWS; i++) {
            // std::cout << i;
        //     cv::putText(testFrame,                  //target image
        //         ".",               //text
        //         cv::Point(0, 11),
        //         // (char *) &(buffer[(i * COLS) + i]),               //text
        //         // cv::Point((w / 2), ((v / 2) + (i * v))),                    //top-left position
        //         font,
        //         0.5,
        //         CV_RGB(255, 255, 255),          //font color
        //         // (0, 0, 0), 
        //         1);
        //     cv::putText(testFrame,                  //target image
        //         ":",               //text
        //         cv::Point(0, 22),
        //         // (char *) &(buffer[(i * COLS) + i]),               //text
        //         // cv::Point((w / 2), ((v / 2) + (i * v))),                    //top-left position
        //         font,
        //         0.5,
        //         CV_RGB(255, 255, 255),          //font color
        //         // (0, 0, 0), 
        //         1);
        //     cv::putText(testFrame,                  //target image
        //         "-",               //text
        //         cv::Point(0, 33),
        //         // (char *) &(buffer[(i * COLS) + i]),               //text
        //         // cv::Point((w / 2), ((v / 2) + (i * v))),                    //top-left position
        //         font,
        //         0.5,
        //         CV_RGB(255, 255, 255),          //font color
        //         // (0, 0, 0), 
        //         1);
        //     cv::putText(testFrame,                  //target image
        //         "+",               //text
        //         cv::Point(0, 44),
        //         // (char *) &(buffer[(i * COLS) + i]),               //text
        //         // cv::Point((w / 2), ((v / 2) + (i * v))),                    //top-left position
        //         font,
        //         0.5,
        //         CV_RGB(255, 255, 255),          //font color
        //         // (0, 0, 0), 
        //         1);
        //     cv::putText(testFrame,                  //target image
        //         "*",               //text
        //         cv::Point(0, 55),
        //         // (char *) &(buffer[(i * COLS) + i]),               //text
        //         // cv::Point((w / 2), ((v / 2) + (i * v))),                    //top-left position
        //         font,
        //         0.5,
        //         CV_RGB(255, 255, 255),          //font color
        //         // (0, 0, 0), 
        //         1);
        //     cv::putText(testFrame,                  //target image
        //         "#",               //text
        //         cv::Point(0, 66),
        //         // (char *) &(buffer[(i * COLS) + i]),               //text
        //         // cv::Point((w / 2), ((v / 2) + (i * v))),                    //top-left position
        //         font,
        //         0.5,
        //         CV_RGB(255, 255, 255),          //font color
        //         // (0, 0, 0), 
        //         1);
        //     cv::putText(testFrame,                  //target image
        //         "%",               //text
        //         cv::Point(0, 77),
        //         // (char *) &(buffer[(i * COLS) + i]),               //text
        //         // cv::Point((w / 2), ((v / 2) + (i * v))),                    //top-left position
        //         font,
        //         0.5,
        //         CV_RGB(255, 255, 255),          //font color
        //         // (0, 0, 0), 
        //         1);
        //     cv::putText(testFrame,                  //target image
        //         "@",               //text
        //         cv::Point(0, 88),
        //         // (char *) &(buffer[(i * COLS) + i]),               //text
        //         // cv::Point((w / 2), ((v / 2) + (i * v))),                    //top-left position
        //         font,
        //         0.5,
        //         CV_RGB(255, 255, 255),          //font color
        //         // (0, 0, 0), 
        //         1);
        // // }
        // cv::imshow("Char", *(imgs[1]));
        cv::imshow("Frame", frame);
        // cv::imshow("TestFrame", testFrame);


    }
    return (0);
}