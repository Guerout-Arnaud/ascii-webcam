/*
** ascii-webcam PROJECT, 2021
** main.cpp
** File description:
** 
** Author:
** Arnaud Guerout
** https://github.com/Guerout-Arnaud
** Contributors:
**
*/

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <stdexcept>


#include "linux/VideoCapture.hpp"
#include "linux/VideoOutput.hpp"
#include "common/AsciiProcessor.hpp"
#include "common/ArgParser.hpp"

static const int SECOND = 1000000;
#ifndef FPS 
    #define FPS 60
#endif

#ifndef COLS
    // #define COLS 64
    #define COLS 50
#endif

#ifndef ROWS
    // #define ROWS 48
    #define ROWS 37
#endif


static bool isRunning = true;

static void fetch_video(VideoCapture &camera)
{
    // for (;isRunning == true;) {
        camera.captureVideo();
        // std::this_thread::sleep_for(std::chrono::microseconds(SECOND / static_cast<int>(FPS)));
    // }
}

static void process_video(AsciiProcessor &processor, uint8_t const *videoBuffer)
{
    // for (;isRunning == true;) {
        processor.transformToAscii(videoBuffer);
    // }
}

static void output_video(VideoOutput &camera, uint8_t const *frame)
{
    // for (;isRunning == true;) {
        camera.sendOutput(frame);
        // std::this_thread::sleep_for(std::chrono::microseconds(SECOND / static_cast<int>(FPS)));
    // }
}


int main (int argc, char **argv)
{
    try {
        // ArgParser args = ArgParser(argc, argv);
        std::cout << "Input: ";
        // VideoCapture vid_input = VideoCapture(args.getArgument("video_input_file"), atoi(args.getArgument("video_width").c_str()), atoi(args.getArgument("video_height").c_str()));
        VideoCapture vid_input = VideoCapture("/dev/video0", 640, 480);
        std::cout << "Done" << std::endl;


        std::cout << "Width:" << std::to_string(vid_input.getVideoWidth()) << " | " << "Height: " << std::to_string(vid_input.getVideoHeight()) << std::endl; 

        std::cout << "Processor: ";
        // AsciiProcessor ascii_prcsr = AsciiProcessor(vid_input.getVideoWidth(), vid_input.getVideoHeight(), atoi(args.getArgument("char_width").c_str()), atoi(args.getArgument("char_height").c_str()));
        AsciiProcessor ascii_prcsr = AsciiProcessor(vid_input.getVideoWidth(), vid_input.getVideoHeight(), COLS, ROWS);
        // AsciiProcessor ascii_prcsr = AsciiProcessor(vid_input.getVideoWidth(), vid_input.getVideoHeight(), 640, 480);
        // AsciiProcessor ascii_prcsr = AsciiProcessor(64, 64, 4, 4);
        std::cout << "Done" << std::endl;

        std::cout << "Output: " << std::endl;
        // if (args.getArgument("video_output_file").empty())
            // VideoOutput vid_output = VideoOutput(vid_input.getVideoWidth(), vid_input.getVideoHeight());
            // VideoOutput vid_output = VideoOutput(48*16, 64*16);

            int nb = 50 * 16;

            std::cout << "Width: " << nb << std::endl;
            std::cout << "Height: " << (37  * 16) << std::endl;

            std::cout << "Width: " << ((int) COLS) * 16 << std::endl;
            std::cout << "Height: " << ((int) ROWS)  * 16 << std::endl;

            VideoOutput vid_output = VideoOutput(COLS*16, ROWS*16);
            // VideoOutput vid_output = VideoOutput(64, 64);
        // else
            // VideoOutput vid_output = VideoOutput(vid_input.getVideoWidth(), vid_input.getVideoHeight(), args.getArgument("video_output_file"));
        std::cout << "Done" << std::endl;

        // std::thread input_thrd = std::thread(fetch_video, std::ref(vid_input));
        // std::thread prcsr_thrd = std::thread(process_video, std::ref(ascii_prcsr), vid_input.getVideoBuffer());
        // std::thread output_thrd = std::thread(output_video, std::ref(vid_output), ascii_prcsr.getTransformedBuffer());

        for(int i = 0;; i++) {
            fetch_video(vid_input);
            cv::Mat output = cv::Mat(ROWS*16, COLS*16, CV_8UC1, cv::Scalar(0, 0, 0));
            // cv::Mat output = cv::Mat(vid_input.getVideoWidth(), vid_input.getVideoHeight(), CV_8UC1, cv::Scalar(0, 0, 0));
            // cv::Mat output = cv::Mat(480 * 16, 640 * 16, CV_8UC1, cv::Scalar(0, 0, 0));

            // cv::Mat test = cv::Mat(64, 64, CV_8UC1, cv::Scalar(255, 255, 255));

            // std::cout << "BUFFER" << std::endl;
            
            // for (int i = 0; i < (1080 * 720); i++) {
            //     std::cout << std::to_string(tmp.data[i]) << "|";
            //     if ((i % 720) == 0)
            //         std::cout << std::endl;
            // }
            // std::cout << "END BUFFER" << std::endl;


            process_video(ascii_prcsr, vid_input.getVideoBuffer());
            // process_video(ascii_prcsr, test.data);

            output.data = (uchar *)ascii_prcsr.getTransformedBuffer();

            output_video(vid_output, output.data);
            // output_video(vid_output, vid_input.getVideoBuffer());


            // for (int i = 0; i < (1080 * 720); i++) {
            //     printf("%d", tmp.data[i]);
            //     if ((i % 720) == 0)
            //         printf("\n");
            // }

            // exit(0);
            // cv::imshow("view", vid_input.getMat());
            // if (i == 0)
            // std::cout << std::to_string(vid_input.getMat().rows) << std::to_string(vid_input.getMat().cols) << std::endl; 
            // cv::imshow("Input", vid_input.getMat());
            // cv::imshow("Input", test);
            cv::imshow("Output", output);
            cv::waitKey(16);
            // std::this_thread::sleep_for(std::chrono::microseconds(SECOND / static_cast<int>(FPS)));

        }

        // input_thrd.join();
        // prcsr_thrd.join();
        // output_thrd.join();
    } catch (std::exception &ex) {
        std::cout << std::endl << ex.what() << std::endl;
        return (-1);
    }
    return (0);
}