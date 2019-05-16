/* box_filter.h
 *
 * Author: Fabian Meyer
 * Created On: 04 Mar 2019
 */

#ifndef CVE_BOX_FILTER_H_
#define CVE_BOX_FILTER_H_

#include "cve/border_handling.h"

namespace cve
{
    /** Filter class to apply an iterated box blur.
     *
     *  It applies a 2D average operation on each pixel with kernel:
     *        | 1 1 1 |
     *  1/9 * | 1 1 1 |
     *        | 1 1 1 |
     *
     *  The iterated box filter becomes a Gaussian filter in the limit
     *  (iterations -> inf).
     * @tparam Scalar value type of the underlying kernel
     * @tparam Dim size of the underlying kernel */
    template<typename Scalar, unsigned int Dim = 3>
    class BoxFilter
    {
    public:
        static_assert(Dim % 2 == 1, "BoxFilter dimension must be odd");
    private:
        size_t iterations_;
        BorderHandling handling_;

        Eigen::Matrix<Scalar, 1, Dim> kernelX_;
        Eigen::Matrix<Scalar, Dim, 1> kernelY_;
    public:

        BoxFilter()
            : BoxFilter(1)
        {
        }

        BoxFilter(const size_t iterations)
            : iterations_(iterations), handling_(BorderHandling::Reflect),
            kernelX_(), kernelY_()
        {
            kernelX_.setOnes();
            kernelX_ /= Dim;

            kernelY_.setOnes();
            kernelY_ /= Dim;
        }

        void setBorderHandling(const BorderHandling handling)
        {
            handling_ = handling;
        }

        void setIterations(const size_t iterations)
        {
            iterations_ = iterations;
        }

        template<typename ImageA, typename ImageB>
        void apply(const ImageA &srcImg, ImageB &destImg) const
        {
            static_assert(ImageA::Depth == ImageB::Depth,
                "ImageA and ImageB must have same depth.");

            ImageB tmpImg;
            destImg = srcImg;

            for(size_t i = 0; i < iterations_; ++i)
            {
                kernel::apply(destImg, tmpImg, kernelX_, handling_);
                kernel::apply(tmpImg, destImg, kernelY_, handling_);
            }
        }
    };
}

#endif
