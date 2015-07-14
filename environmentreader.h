#ifndef ENVIRONMENTREADER_H
#define ENVIRONMENTREADER_H

#include <iostream>



namespace QRR{
namespace Environment{
    static std::string asset_path = "./resource/";
    static std::string shader_path = asset_path + "Shader/";
    static std::string model_path = asset_path + "model/";
    static std::string distance_ip = "192.168.100.23";
    static float mmDiv = 7.8;
}

namespace WindowInfo{
    static int Width = 1280;
    static int Height = 720;
}


/* mm */
namespace RealInfo{
    static float EyeEye = 63.0;
    static float EyeHand = 400.0;
}
}
#endif // ENVIRONMENTREADER_H
