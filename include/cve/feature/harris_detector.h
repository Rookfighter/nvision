/* harris_detector.h
 *
 * Author: Fabian Meyer
 * Created On: 18 Jun 2019
 */

#ifndef CVE_HARRIS_DETECTOR_H_
#define CVE_HARRIS_DETECTOR_H_

#include "cve/math.h"
#include "cve/filter/gauss_filter.h"
#include "cve/filter/sobel_filter.h"

namespace cve
{
    /** Class for harris corner detection. */
    template<typename Scalar,
        typename SmoothFilter=GaussFilter<Scalar, 3>,
        typename GradientFilter=SobelFilter<Scalar>>
    class HarrisDetector
    {
    private:
        typedef Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> Matrix;
        typedef Eigen::Matrix<Scalar, 2, 2> Matrix2;
        typedef Eigen::Matrix<Scalar, 2, 1> Vector2;

        Scalar traceFactor_;
        BorderHandling handling_;

        SmoothFilter smoothFilter_;
        GradientFilter gradientFilter_;

        /** Determines of given pixel is a corner.
          * If the harris response of this pixel is greater than all response
          * values in its local neighbourhood then the pixel is a corner. */
        bool isCorner(const Eigen::Tensor<Scalar, 3> &response,
            const Index row,
            const Index col) const
        {
            Scalar val = response(row, col, 0);
            for(Index x = -1; x < 2; ++x)
            {
                Index c2 = col + x;
                if(c2 < 0 || c2 >= response.dimension(1))
                    continue;
                for(Index y = -1; y < 2; ++y)
                {
                    Index r2 = row + y;
                    if(r2 < 0 || r2 >= response.dimension(0))
                        continue;

                    if(val < response(r2, c2, 0))
                        return false;
                }
            }

            return true;
        }

    public:
        HarrisDetector()
            : HarrisDetector(0.05)
        {

        }

        HarrisDetector(const Scalar traceFactor)
            : traceFactor_(traceFactor), handling_(BorderHandling::Reflect)
        {

        }

        void setBorderHandling(const BorderHandling handling)
        {
            handling_ = handling;
        }

        void setSmoothFilter(const SmoothFilter &filter)
        {
            smoothFilter_ = filter;
        }

        void setGradientFilter(const GradientFilter &gradient)
        {
            gradientFilter_ = gradient;
        }


        /** Compute Harris corner features in the given Image.
          * The keypoints are returned in a matrix, where each column
          * represents a single 2D point. Each keypoint is stored in (x y), i.e.
          * (column row), format.
          * @param srcImg input image
          * @param keypoints 2xN matrix with N keypoints
          */
        template<typename ScalarA>
        void apply(const Eigen::Tensor<ScalarA, 3> &srcImg,
            Matrix &keypoints) const
        {
            Eigen::Tensor<Scalar, 3> gradX(srcImg.dimensions());
            Eigen::Tensor<Scalar, 3> gradY(srcImg.dimensions());
            Eigen::Tensor<Scalar, 3> gradXX(srcImg.dimensions());
            Eigen::Tensor<Scalar, 3> gradYY(srcImg.dimensions());
            Eigen::Tensor<Scalar, 3> gradXY(srcImg.dimensions());
            Eigen::Tensor<Scalar, 3> tmpImg(srcImg.dimensions());
            Eigen::Tensor<Scalar, 3> response(srcImg.dimension(0), srcImg.dimension(1), 1);

            // calculate gradients
            gradientFilter_.applyX(srcImg, gradX);
            gradientFilter_.applyY(srcImg, gradY);

            // calculate gradients magnitudes
            gradXX = gradX * gradX;
            gradYY = gradY * gradY;
            gradXY = gradX * gradY;

            // accumulate values from local neighbourhood with smooth filter
            smoothFilter_.apply(gradXX, tmpImg);
            gradXX = tmpImg;
            smoothFilter_.apply(gradYY, tmpImg);
            gradYY = tmpImg;
            smoothFilter_.apply(gradXY, tmpImg);
            gradXY = tmpImg;

            // compute the harris response
            response.setZero();
            for(Index d = 0; d < gradXX.dimension(2); ++d)
            {
                for(Index c = 0; c < gradXX.dimension(1); ++c)
                {
                    for(Index r = 0; r < gradXX.dimension(0); ++r)
                    {
                        Matrix2 M;
                        M << gradXX(r, c, d), gradXY(r, c, d),
                            gradXY(r, c, d), gradYY(r, c, d);
                        Scalar trace = M(0, 0) + M(1, 1);
                        Scalar det = M.determinant();
                        response(r, c, 0) += det - traceFactor_ * trace;
                    }
                }
            }

            size_t cnt = 0;
            for(Index c = 0; c < response.dimension(1); ++c)
                for(Index r = 0; r < response.dimension(0); ++r)
                    if(isCorner(response, r, c))
                        ++cnt;
            keypoints.resize(2, cnt);

            cnt = 0;
            for(Index c = 0; c < response.dimension(1); ++c)
            {
                for(Index r = 0; r < response.dimension(0); ++r)
                {
                    if(isCorner(response, r, c))
                    {
                        keypoints.col(cnt) << c, r;
                        ++cnt;
                    }
                }
            }
        }
    };
}

#endif
