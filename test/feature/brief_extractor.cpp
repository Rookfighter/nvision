/* brief_extractor.cpp
 *
 * Author: Fabian Meyer
 * Created On: 06 Aug 2019
 */

#include "assert/eigen_require.h"
#include <cve/feature/brief_extractor.h>

using namespace cve;

typedef float Scalar;
typedef Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> Matrix;

TEST_CASE("brief_extractor")
{
    BRIEFExtractor<Scalar> brief;

    Image8 img(5, 5, 1);
    img.setZero();
    img(0, 2, 0) = 255; img(0, 4, 0) = 255;
    img(1, 1, 0) = 255; img(1, 3, 0) = 255;
    img(2, 0, 0) = 255; img(2, 2, 0) = 255;
    img(3, 0, 0) = 255; img(3, 4, 0) = 255;
    img(4, 3, 0) = 255; img(4, 4, 0) = 255;

    SECTION("multi bytes, single keypoint")
    {
        Matrixi pattern(4, 32);
        pattern <<  4, 4, 1, 2, 4, 1, 2, 4,  4, 1, 2, 4, 4, 1, 2, 4,  4, 4, 1, 2, 4, 1, 2, 4,  4, 1, 2, 4, 4, 1, 2, 4,
                    2, 3, 4, 2, 3, 4, 2, 2,  2, 4, 2, 3, 3, 4, 2, 2,  2, 3, 4, 2, 3, 4, 2, 2,  2, 4, 2, 3, 3, 4, 2, 2,
                    0, 3, 1, 3, 3, 1, 3, 0,  0, 1, 3, 3, 3, 1, 3, 0,  0, 3, 1, 3, 3, 1, 3, 0,  0, 1, 3, 3, 3, 1, 3, 0,
                    3, 2, 3, 0, 2, 3, 0, 3,  3, 3, 0, 2, 2, 3, 0, 3,  3, 2, 3, 0, 2, 3, 0, 3,  3, 3, 0, 2, 2, 3, 0, 3;

       Matrix keypoints(2, 1);
       keypoints << 0, 0;

       Matrixi32 descriptorsExp(1, 1);
       descriptorsExp << 0x5a3a5a3a;

       Matrixi32 descriptorsAct;

       brief.setPattern(pattern);
       REQUIRE_MATRIX(pattern, brief.pattern());

       brief(img, keypoints, descriptorsAct);

       REQUIRE_MATRIX(descriptorsExp, descriptorsAct);
    }

    SECTION("multibytes, multi keypoints")
    {
        Matrixi pattern(4, 32);
        pattern <<  4, 4, 1, 2, 4, 1, 2, 4,  4, 1, 2, 4, 4, 1, 2, 4,  4, 4, 1, 2, 4, 1, 2, 4,  4, 1, 2, 4, 4, 1, 2, 4,
                    2, 3, 4, 2, 3, 4, 2, 2,  2, 4, 2, 3, 3, 4, 2, 2,  2, 3, 4, 2, 3, 4, 2, 2,  2, 4, 2, 3, 3, 4, 2, 2,
                    0, 3, 1, 3, 3, 1, 3, 0,  0, 1, 3, 3, 3, 1, 3, 0,  0, 3, 1, 3, 3, 1, 3, 0,  0, 1, 3, 3, 3, 1, 3, 0,
                    3, 2, 3, 0, 2, 3, 0, 3,  3, 3, 0, 2, 2, 3, 0, 3,  3, 2, 3, 0, 2, 3, 0, 3,  3, 3, 0, 2, 2, 3, 0, 3;

       Matrix keypoints(2, 2);
       keypoints << 0, 2,
                    0, 0;

       Matrixi32 descriptorsExp(1, 2);
       descriptorsExp << 0x5a3a5a3a,
                         0x24442444;

       Matrixi32 descriptorsAct;

       brief.setPattern(pattern);
       REQUIRE_MATRIX(pattern, brief.pattern());

       brief(img, keypoints, descriptorsAct);

       REQUIRE_MATRIX(descriptorsExp, descriptorsAct);
    }
}
