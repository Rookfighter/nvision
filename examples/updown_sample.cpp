/* updown_sample.cpp
 *
 * Author: Fabian Meyer
 * Created On: 29 Apr 2019
 */

#include <iostream>
#include <cve/image.h>
#include <cve/imageio/pgm.h>

int main(int argc, const char **argv)
{
    if(argc != 2)
    {
        std::cerr << "usage: updown_sample <filename>" << std::endl;
        return -1;
    }

    std::cout << "Load \"" << argv[1] << "\"" << std::endl;
    cve::ImageGray img;
    cve::pgm::load(argv[1], img);

    std::cout << "Downsample" << std::endl;;
    cve::ImageGray oimg;
    cve::downsample<float, cve::ImageGray>(0.3, img, oimg);

    cve::pgm::save("downsample.pgm", oimg);

    return 0;
}