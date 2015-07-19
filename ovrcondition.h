#ifndef OVRCONDITION_H
#define OVRCONDITION_H


#include <QFile>
#include <QUdpSocket>
#include <QDataStream>
#include <QByteArray>


#include "OVR.h"
#include "OVR_Kernel.h"
#include "Kernel/OVR_Types.h"
#include "Kernel/OVR_Allocator.h"
#include "Kernel/OVR_RefCount.h"
#include "Kernel/OVR_Log.h"
#include "Kernel/OVR_System.h"
#include "Kernel/OVR_Nullptr.h"
#include "Kernel/OVR_String.h"
#include "Kernel/OVR_Array.h"
#include "Kernel/OVR_Timer.h"
#include "Kernel/OVR_DebugHelp.h"
#include "Extras/OVR_Math.h"
#include "Render/Render_Device.h"
#include "OVR_CAPI.h"

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "environmentreader.h"


class OVRCondition
{
public:
    OVRCondition();
    ~OVRCondition();

    ovrHmd              Hmd;
    bool isHmdInitialized = false;
    ovrHmd ovrhmd;

     void initOVR();
     void send();
     int OnStartup();

     QUdpSocket udpsocket;

     QUdpSocket *videoudpSocket;

};

#endif // OVRCONDITION_H
