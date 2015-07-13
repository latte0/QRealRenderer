#ifndef ENVIRONMENTREADER_H
#define ENVIRONMENTREADER_H

#include <iostream>

namespace QRR{
namespace Environment{
    static std::string asset_path = "./resource/";
    static std::string shader_path = asset_path + "Shader/";
    static std::string model_path = asset_path + "model/";
    static std::string distance_ip = "192.168.100.23";
}

namespace WindowInfo{
    int window_width = 1280;
    int window_height = 720;
}
}
#endif // ENVIRONMENTREADER_H
