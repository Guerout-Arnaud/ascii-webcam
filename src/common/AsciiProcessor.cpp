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

#include <cstdint>
#include <cstdlib>

#include "common/bitmask_16.hpp"
#include "common/AsciiProcessor.hpp"

AsciiProcessor::AsciiProcessor(unsigned int videoWidth, unsigned int videoHeight, unsigned int outputWidth, unsigned int outputHeight)
{
    this->tranformedBuffer = (uint8_t *)malloc(sizeof(*(this->tranformedBuffer)) * (outputWidth * BITMASK_WIDTH) * (outputHeight * BITMASK_HEIGHT));
    this->videoWidth = videoWidth;
    this->videoHeight = videoHeight;
    this->outputWidth = outputWidth;
    this->outputHeight = outputHeight;
}

AsciiProcessor::~AsciiProcessor()
{
    free(tranformedBuffer);
}

uint8_t const *AsciiProcessor::getTransformedBuffer() const {
    return (this->tranformedBuffer);
}

uint8_t AsciiProcessor::avg(uint8_t *videoBuffer, int x, int y)
{
    static int cell_width = videoWidth / outputWidth;
    static int cell_height = videoHeight / outputHeight;

    int x_init = x * cell_width;
    int y_init = y * cell_height;
    
    
    uint8_t grayscale = 0;

    for (int i = 0; i < cell_height; i++) {
        for (int j = 0; j < cell_width; j++) {
            grayscale = grayscale + videoBuffer[(y_init * 2 * cell_width) + x_init + (i * 2 * cell_width) + j];
                
                // frame.at<uchar>(
                // ((y_init + i) < videoHeight) ? (y_init + i) : videoHeight,
                // ((x_init + j) < videoWidth) ? (x_init + j) : videoWidth
                // );
        }
    }
    return (grayscale / (cell_width * cell_height));
}

const uint8_t const *AsciiProcessor::grayToChar(uint8_t grayscale) const {

    int idx = (grayscale * BITMASK_LEN) / 255;

    return (idx < (BITMASK_LEN - 1) ? BITMASKS[idx] : BITMASKS[BITMASK_LEN - 1]);
}

void AsciiProcessor::transformToAscii(uint8_t *videoBuffer, bool *isRunning)
{
    const uint8_t *mask = NULL;
    
    for (; *isRunning == true;) {
        for (int i = 0; i < this->outputHeight; i++) {
            for (int j = 0; j < this->outputWidth; j++) {
                mask = grayToChar(avg(videoBuffer, j, i));

                // ToDo Transform this into a REAL bitmask vith bitwise operators /!\ Will be way more difficult to read
                for (int y = 0; y < BITMASK_HEIGHT; y++) {
                    for (int x = 0; x < BITMASK_WIDTH; x++) {
                        if (mask[(y * BITMASK_WIDTH) + x] == '0')
                            this->tranformedBuffer[(i * BITMASK_HEIGHT * BITMASK_WIDTH * this->outputWidth) + (j * BITMASK_WIDTH) + (y * BITMASK_WIDTH * this->outputWidth) + x] = 0;
                        else 
                            this->tranformedBuffer[(i * BITMASK_HEIGHT * BITMASK_WIDTH * this->outputWidth) + (j * BITMASK_WIDTH) + (y * BITMASK_WIDTH * this->outputWidth) + x] = 255;
                    }
                }
            }
        }
    }
}
