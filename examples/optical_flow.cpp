/* optical_flow.cpp
 *
 * Author: Fabian Meyer
 * Created On: 16 Mar 2019
 */

#include <iostream>
#include <nvision/optflow/lucas_kanade_detector.h>
#include <nvision/optflow/horn_schunck_detector.h>
#include <nvision/optflow/robust_flow_detector.h>
#include <nvision/optflow/color_map.h>
#include <nvision/imageio/pgm.h>
#include <nvision/imageio/ppm.h>

using namespace nvision;

int main(int argc, const char **argv)
{
    if(argc != 3)
    {
        std::cerr << "usage: optical_flow <filename> <filename>" << std::endl;
        return -1;
    }

    Image8 imgA;
    Image8 imgB;
    Image8 imgASmooth;
    Image8 imgBSmooth;
    Image8 oimg;
    Eigen::Tensor<float, 3> flowImg;
    optflow::ColorMap<float> cmap;

    std::cout << "Load \"" << argv[1] << "\"" << std::endl;
    nvision::pgm::load(argv[1], imgA);
    std::cout << "-- size " << imgA.dimension(1) << "x" << imgA.dimension(0) << std::endl;

    std::cout << "Load \"" << argv[2] << "\"" << std::endl;
    nvision::pgm::load(argv[2], imgB);
    std::cout << "-- size " << imgB.dimension(1) << "x" << imgB.dimension(0) << std::endl;

    GaussFilter<float> preSmooth(3);
    preSmooth(imgA, imgASmooth);
    preSmooth(imgB, imgBSmooth);

    std::cout << "Apply Lucas Kanade detector" << std::endl;

    LucasKanadeDetector<float> lkDetector;
    lkDetector.setSmoothFilter({3});
    lkDetector(imgASmooth, imgBSmooth, flowImg);
    cmap(flowImg, oimg);

    nvision::ppm::save("lucas_kanade.ppm", oimg);

    std::cout << "Apply Horn Schunck detector" << std::endl;

    HornSchunckDetector<float> hsDetector;
    hsDetector(imgASmooth, imgBSmooth, flowImg);
    cmap(flowImg, oimg);

    nvision::ppm::save("horn_schunck.ppm", oimg);

    std::cout << "Apply Robust Flow detector" << std::endl;

    RobustFlowDetector<float> rfDetector(300, 20, 1);
    rfDetector(imgA, imgB, flowImg);
    cmap(flowImg, oimg);

    nvision::ppm::save("robust_flow.ppm", oimg);

    optflow::ColorWheel<float> cwheel;
    cwheel(50, oimg);
    nvision::ppm::save("color_wheel.ppm", oimg);

    return 0;
}
