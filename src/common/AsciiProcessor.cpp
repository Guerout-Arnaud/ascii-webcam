/*
** ascii-webcam PROJECT, 2021
** AsciiProcessor.cpp
** File description:
** 
** Author:
** Arnaud Guerout
** https://github.com/Guerout-Arnaud
** Contributors:
**
*/

#include <iostream>

#include <cstdint>
#include <cstdlib>

#include "common/bitmask_16.hpp"
#include "common/AsciiProcessor.hpp"

AsciiProcessor::AsciiProcessor(unsigned int videoWidth, unsigned int videoHeight, unsigned int outputWidth, unsigned int outputHeight)
{
    this->tranformedBuffer = (uint8_t *)malloc(sizeof(*(this->tranformedBuffer)) * (outputWidth * BITMASK_WIDTH) * (outputHeight * BITMASK_HEIGHT));
    // this->tranformedBuffer = (uint8_t *)malloc(sizeof(*(this->tranformedBuffer)) * (outputWidth * BITMASK_WIDTH) * (outputHeight * BITMASK_HEIGHT) * 3);
    this->videoWidth = videoWidth;
    this->videoHeight = videoHeight;
    this->outputWidth = outputWidth;
    this->outputHeight = outputHeight;

    std::cout << "=========== Processor ==========" << std::endl;
    std::cout << "Width: " << outputWidth << std::endl;
    std::cout << "Height: " << outputHeight << std::endl; 
}

AsciiProcessor::~AsciiProcessor()
{
    free(tranformedBuffer);
}

uint8_t const *AsciiProcessor::getTransformedBuffer() const {
    return (this->tranformedBuffer);
}

uint8_t AsciiProcessor::avg(uint8_t const *videoBuffer, int x, int y)
{
    static int cell_width = videoWidth / outputWidth;
    static int cell_height = videoHeight / outputHeight;

    int x_init = x * cell_width;
    int y_init = y * cell_height;
    
    // std::cout << "cell_width: " << std::to_string(cell_width) << std::endl;
    // std::cout << "cell_height: " << std::to_string(cell_height) << std::endl;
    
    // std::cout << "x_init: " << std::to_string(x_init) << std::endl;
    // std::cout << "y_init: " << std::to_string(y_init) << std::endl << std::endl;

    
    int grayscale = 0;

    for (int i = 0; i < cell_height; i++) {
        for (int j = 0; j < cell_width; j++) {
            grayscale = grayscale + videoBuffer[(y_init * videoWidth) + x_init + (i * videoWidth) + j];
            // std::cout << "added: " << std::to_string(videoBuffer[(y_init * 2 * cell_width) + x_init + (i * 2 * cell_width) + j]) << std::endl;
            // std::cout << "grayscale: " << std::to_string(grayscale) << std::endl;
                
                // frame.at<uchar>(
                // ((y_init + i) < videoHeight) ? (y_init + i) : videoHeight,
                // ((x_init + j) < videoWidth) ? (x_init + j) : videoWidth
                // );
        }
    }
    // std::cout << "Grayscale: " << std::to_string(grayscale) << std::endl;
    // std::cout << "grayscale / (cell_width * cell_height): " << std::to_string(grayscale / (cell_width * cell_height)) << std::endl;

    // std::cout << std::endl << "grayscale: " << std::to_string(grayscale) << std::endl << std::endl;
    
    return ((uint8_t)(grayscale / (cell_width * cell_height)));
}

const uint8_t *AsciiProcessor::grayToChar(uint8_t grayscale) const {

    static const char *CHAR_LIST = " .:-=+*#%@";
    
    int idx = (grayscale * BITMASK_LEN) / 255;

    // if (idx < (BITMASK_LEN - 1))
    //     std::cout << CHAR_LIST[idx];
    // else
    //     std::cout << CHAR_LIST[BITMASK_LEN - 1];
    
    return (idx < (BITMASK_LEN - 1) ? BITMASKS[idx] : BITMASKS[BITMASK_LEN - 1]);
}

void AsciiProcessor::transformToAscii(uint8_t const *videoBuffer)
{
    const uint8_t *mask = NULL;

    // std::cout << "transformToAscii" << std::endl;
    // std::cout << "Heignt: " << std::to_string(outputHeight) << std::endl;
    // std::cout << "Width: " << std::to_string(outputWidth) << std::endl;
    
    
    for (int i = 0; i < this->outputHeight; i++) {
        for (int j = 0; j < this->outputWidth; j++) {

            mask = grayToChar(avg(videoBuffer, j, i));
            // continue;
            // exit(0);
    
            // ToDo Transform this into a REAL bitmask vith bitwise operators /!\ Will be way more difficult to read
            for (int y = 0; y < BITMASK_HEIGHT; y++) {
                for (int x = 0; x < BITMASK_WIDTH; x++) {
                    if (mask[(y * BITMASK_WIDTH) + x] == '0') {
                        // std::cout<< "LA" << std::endl;
                        // ToDo rework to not overflow
                        this->tranformedBuffer[(i * BITMASK_HEIGHT * BITMASK_WIDTH * this->outputWidth) + (j * BITMASK_WIDTH) + (y * BITMASK_WIDTH * this->outputWidth) + x] = 0;
                        // this->tranformedBuffer[(i * BITMASK_HEIGHT * BITMASK_WIDTH * this->outputWidth) + (j * BITMASK_WIDTH) + (y * BITMASK_WIDTH * this->outputWidth) + x + 1] = 0;
                        // this->tranformedBuffer[(i * BITMASK_HEIGHT * BITMASK_WIDTH * this->outputWidth) + (j * BITMASK_WIDTH) + (y * BITMASK_WIDTH * this->outputWidth) + x + 2] = 0;
                    } else {
                        // std::cout<< "ICI" << std::endl;
                        // ToDo rework to not overflow
                        this->tranformedBuffer[(i * BITMASK_HEIGHT * BITMASK_WIDTH * this->outputWidth) + (j * BITMASK_WIDTH) + (y * BITMASK_WIDTH * this->outputWidth) + x] = 255;
                        // this->tranformedBuffer[(i * BITMASK_HEIGHT * BITMASK_WIDTH * this->outputWidth) + (j * BITMASK_WIDTH) + (y * BITMASK_WIDTH * this->outputWidth) + x + 1] = 255;
                        // this->tranformedBuffer[(i * BITMASK_HEIGHT * BITMASK_WIDTH * this->outputWidth) + (j * BITMASK_WIDTH) + (y * BITMASK_WIDTH * this->outputWidth) + x + 2] = 255;
                    }
                }
            }
        }
        // std::cout << std::endl;
    }
    
    // std::cout << std::endl;
    // std::cout << std::endl;
    // std::cout << std::endl;
}
