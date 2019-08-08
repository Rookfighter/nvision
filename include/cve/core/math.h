/* math.h
 *
 * Author: Fabian Meyer
 * Created On: 15 May 2019
 */

#ifndef CVE_MATH_H_
#define CVE_MATH_H_

namespace cve
{
    template<typename Scalar>
    Scalar pi()
    {
        return static_cast<Scalar>(3.1415926535897932);
    }

    namespace angle
    {
        template<typename Scalar>
        Scalar normalize(const Scalar angle)
        {
            Scalar result = angle;
            while(result >= 2 * pi<Scalar>())
                result -= 2 * pi<Scalar>();
            while(result < 0)
                result += 2 * pi<Scalar>();
            return result;
        }
    }
}

#endif
