#include "ovrcondition.h"

#include "Kernel/OVR_Threads.h"
#include "Util/Util_SystemGUI.h"
#include "eigenutil.h"


#include "external/external_opengl/glm/glm.hpp"
#include "external/external_opengl/glm/gtc/type_ptr.hpp"



OVRCondition::OVRCondition()
{
    OnStartup();

    initOVR();
}

OVRCondition::~OVRCondition()
{

}

void OVRCondition::initOVR(){

}



static void OVR_CDECL LogCallback(int level, const char* message)
{
    OVR_UNUSED2(level, message);
}

int OVRCondition::OnStartup()
{


    ovrInitParams params = {0, 0, nullptr, 0};
    params.LogCallback = LogCallback;
    ovrBool result = ovr_Initialize(&params);

    ovr_TraceMessage(ovrLogLevel_Info, "Oculus World Demo OnStartup");

    if (!result)
    {
        qDebug() << "oculusworlddemo", "unabel to initialize libovr";

    }

    Hmd = ovrHmd_Create(0);
    if (!Hmd || !ovrHmd_ConfigureTracking(Hmd,
        ovrTrackingCap_Orientation, 0))
    {
        qDebug() << "Unable to create hmd" << ovrHmd_GetLastError(NULL);
    }

    for (int i = 0; i < 10; ++i) {
      ovrTrackingState state = ovrHmd_GetTrackingState(Hmd, 0);

      ovrQuatf orientation = state.HeadPose.ThePose.Orientation;
      qDebug () << orientation.x << orientation.y << orientation.z << orientation.w;
      glm::quat q = glm::make_quat(&orientation.x);
      glm::vec3 euler = glm::eulerAngles(q);

      qDebug() << euler.x << euler.y << euler.z;

    }


    return 0;

}


void OVRCondition::send(){
    ovrTrackingState state = ovrHmd_GetTrackingState(Hmd, 0);

    ovrQuatf orientation = state.HeadPose.ThePose.Orientation;
    qDebug () << orientation.x << orientation.y << orientation.z << orientation.w;
    Eigen::Matrix4f testmat = QRRUtil::MakeMatrixfromQuat( orientation.x , orientation.y , orientation.z , orientation.w);
    std::cout << testmat << std::endl;
    glm::quat q = glm::make_quat(&orientation.x);
    glm::vec3 euler = glm::eulerAngles(q);

 //   qDebug() << euler.x * 180/3.1415<< euler.y * 180/3.1415<< euler.z* 180/3.1415;

    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_1);
    out << euler.x * 180/3.1415<< euler.y * 180/3.1415<< euler.z* 180/3.1415;
    QString send = QString("%1/%2/%3").arg(euler.x * 180/3.1415).arg(euler.y * 180/3.1415).arg(euler.z* 180/3.1415);

    qDebug () << send;
    udpsocket.writeDatagram(send.toLocal8Bit(),send.toLocal8Bit().length(), QHostAddress(QString("192.168.23.201")), 1500);
     udpsocket.writeDatagram(send.toLocal8Bit(),send.toLocal8Bit().length(), QHostAddress::LocalHost, 1500);
}
