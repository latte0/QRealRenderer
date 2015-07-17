#ifndef OCULUSINFO_H
#define OCULUSINFO_H


class OculusInfo
{
public:
    static constexpr float eyeProjection = 90;
    static constexpr float eyeAspect = 960.0/1080.0;
    static constexpr float width = 40;
    static constexpr float height = 57.1;
    static float getEyeProjection()
    {
        return eyeProjection;
    }
    static float getEyeAspect()
    {
        return eyeAspect;
    }
};

class MorpheusInfo
{
public:
    static constexpr float eyeProjection = 90;
    static constexpr float eyeAspect = 40.0/57.1;
    static constexpr float width = 40;
    static constexpr float height = 57.1;
    static float getEyeProjection()
    {
        return eyeProjection;
    }
    static float getEyeAspect()
    {
        return eyeAspect;
    }
};


#endif // OCULUSINFO_H
