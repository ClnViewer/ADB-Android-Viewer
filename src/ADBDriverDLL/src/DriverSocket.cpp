
#include "DriverInternal.h"
#include "DriverConst.h"
#include "lz4/lz4.h"
//#include "..\Dialog\AdbMgrDialog/AdbMgrDialog.h"

#define BMZ_MAGIC 0x315a4d42

namespace GameDev
{

bool ADBDriver::AdbCheck()
{
    return m_net.Check();
}

#include "DriverSocket/DriverSocketMake.cpp"
#include "DriverSocket/DriverSocketSend.cpp"
#include "DriverSocket/DriverSocketReceive.cpp"
#include "DriverSocket/DriverSocketRawCmd.cpp"
#include "DriverSocket/DriverSocketCapture.cpp"

}
