/* sobel_filter.h
 *
 * Author: Fabian Meyer
 * Created On: 30 Apr 2019
 */

#ifndef CVE_SOBEL_FILTER_H_
#define CVE_SOBEL_FILTER_H_

#include "cve/src/core/image.h"
#include "cve/src/filter/gradient_filter.h"

namespace cve
{
    /** Filter functor, which computes Sobel gradients of an image.
     *  The Sobel operator computes the first derivative of the image. */
    template<typename _KernelScalar>
    class SobelFilter
    {
    public:
        using KernelScalar = _KernelScalar;
        static constexpr Index Dimension = 3;
        using KernelMatrix = Eigen::Matrix<KernelScalar, Dimension, Dimension>;

        static_assert(Eigen::NumTraits<KernelScalar>::IsInteger == 0, "Kernel scalar must be floating point");
        static_assert(Dimension % 2 == 1, "Kernel must have odd dimension");

        SobelFilter()
        {
            _kernelX << -1, 0, 1,
                        -2, 0, 2,
                        -1, 0, 1;
            _kernelY << -1, -2, -1,
                         0,  0,  0,
                         1,  2,  1;
        }

        /** Applies sobel filter in horizontal direction to the given image and returns a expression of the computation.
          * @param img the image on which the box filter should be applied.
          * @param handling the border handling mode; defaults to BorderReflect
          * @return expression of the convolution operation */
        template<typename ColorSpace, typename BorderHandling=BorderReflect>
        auto operator()(const Image<ColorSpace> &img,
            const GradientMode::X,
            const BorderHandling &handling = BorderHandling{}) const
        {
            return image::correlate(img, _kernelX, handling);
        }

        /** Applies sobel filter in vertical direction to the given image and returns a expression of the computation.
          * @param img the image on which the box filter should be applied.
          * @param handling the border handling mode; defaults to BorderReflect
          * @return expression of the convolution operation */
        template<typename ColorSpace, typename BorderHandling=BorderReflect>
        auto operator()(const Image<ColorSpace> &img,
            const GradientMode::Y,
            const BorderHandling &handling = BorderHandling{}) const
        {
            return image::correlate(img, _kernelY, handling);
        }
    private:
        KernelMatrix _kernelX = {};
        KernelMatrix _kernelY = {};
    };
}

#endif