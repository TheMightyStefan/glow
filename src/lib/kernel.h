#pragma once

#include <vector>

/** @brief Small matrix used for image processing. **/
class Kernel {
    private:
        /** @brief The values of the Kernel. **/
        std::vector<std::vector<double>> kernelData;

    public:
        /** 
         * @brief Constructs the Kernel. 
         *
         * @warning The input matrix must not be empty, have even dimensions, or
         *          different row-dimensions. Otherwise, std::invalid_argument is thrown.
         */
        Kernel(const std::vector<std::vector<double>>& kernelData);

        /** @brief Copy-construct the Kernel. **/
        Kernel(const Kernel& kernel);

        /** 
         * @brief Normalizes the Kernel values. 
         *
         * @returns The factor used for the normalization.
         */
        double normalize();

        /** @brief Returns the specified row of the Kernel. **/
        std::vector<double> operator[](const size_t index) const;
        
        /** @brief Returns the height of the Kernel. **/
        size_t getHeight() const;
        
        /** @brief Returns the width of the Kernel. **/
        size_t getWidth() const;
};

class GaussianKernel : public Kernel {
    private:
        /**
         * @brief Extracts the necessary data from a normal distribution with 
         *        mean 0 and a given variance to create a Kernel.
         *
         * @param width -> The width of the Kernel.
         * @param height -> The height of the Kernel.
         *
         * @param variance -> The variance of the distribution used to create
         *                    the Kernel.
         *
         * @warning width and height must be odd and the variance positive. 
         *          std::invalid_argument is thrown otherwise.
         */
        std::vector<std::vector<double>> getGaussianData(const size_t width, 
                                                         const size_t height, 
                                                         const double variance) const;

    public:
        /**
         * @brief Creates a kernel from a normal distribution 
         *        with mean 0 and a given variance.
         *
         * Calls getGaussianData(). All requirements apply.
         */
        GaussianKernel(const size_t width, const size_t height, const double variance);
};
