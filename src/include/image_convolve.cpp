#include "image_convolve.h"

ofImage convertFromFloat(const ofFloatImage& floatImage) {
    const ofPixels_<float> floatPixels = floatImage.getPixels();

    ofPixels output;
    output.allocate(floatPixels.getWidth(), floatPixels.getHeight(),
                    OF_IMAGE_COLOR);

    for (size_t row = 0; row < floatPixels.getHeight(); row++)
        for (size_t col = 0; col < floatPixels.getWidth(); col++)
            output.setColor(col, row, 
                            ofColor(floatPixels.getColor(col, row)));

    ofImage outputImg;
    outputImg.setFromPixels(output.getData(), 
                            output.getWidth(), 
                            output.getHeight(), 
                            OF_IMAGE_COLOR);
                
    return outputImg;
}

template <typename PixelType>
ofFloatImage float_convolve(const ofImage_<PixelType> &input, 
                            const Kernel &kernel) {
    const ofPixels_<PixelType> pixels = input.getPixels();

    ofPixels_<float> output;
    output.allocate(pixels.getWidth(), pixels.getHeight(),
                    OF_IMAGE_COLOR);

    // Avoid size_t to int conversions.
    int kernelHeight = kernel.getHeight();
    int kernelWidth = kernel.getWidth();

    for (size_t row = 0; row < pixels.getHeight(); row++)
        for (size_t col = 0; col < pixels.getWidth(); col++) {
            float red = 0;
            float green = 0;
            float blue = 0;

            for (int kernelRow = -kernelHeight / 2; 
                     kernelRow <= kernelHeight / 2; 
                     kernelRow++)

                for (int kernelCol = -kernelWidth / 2;
                         kernelCol <= kernelWidth / 2;
                         kernelCol++) {

                    int usedRow = row + kernelRow;
                    int usedCol = col + kernelCol;

                    if (usedRow < 0)
                        usedRow = pixels.getHeight() + kernelRow;
                    else if (usedRow >= (int)pixels.getHeight())
                        usedRow = kernelRow - 1;

                    if (usedCol < 0) 
                        usedCol = pixels.getWidth() + kernelCol;
                    else if (usedCol >= (int)pixels.getWidth())
                        usedCol = kernelCol - 1;

                    const double multiplier = kernel[kernelRow + kernelHeight / 2]
                                                    [kernelCol + kernelWidth / 2];

                    const ofColor color = pixels.getColor(usedCol, usedRow);

                    red   += static_cast<float>(color.r) * multiplier;
                    green += static_cast<float>(color.g) * multiplier;
                    blue  += static_cast<float>(color.b) * multiplier;
                }

            output.setColor(col, row, ofColor(red, green, blue));
        }

    ofImage_<float> outputImg;
    outputImg.setFromPixels(output.getData(), 
                            output.getWidth(), 
                            output.getHeight(), 
                            OF_IMAGE_COLOR);
                
    return outputImg;
}

ofImage convolve(const ofImage& input, const Kernel &kernel) {
    ofFloatImage result = float_convolve<unsigned char>(input, kernel);

    return convertFromFloat(result);
}

ofImage iterativeConvolve(const ofImage& image, const Kernel &kernel,
                          const int times) {
    ofFloatImage result = float_convolve<unsigned char>(image, kernel);

    for (int index = 1; index < times; index++)
        result = float_convolve(result, kernel);

    return convertFromFloat(result);
}