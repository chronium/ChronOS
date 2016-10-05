#ifndef _KERNEL_DEVMANAGER_H
#define _KERNEL_DEVMANAGER_H

#include <kernel/list.h>
#include <kernel/device.h>

using namespace Driver;

namespace DeviceManager {

class DeviceManager {
public:
    DeviceManager ():
        devices (new List<Device> ()) { }

    inline void AddDevice (Device *dev) { this->devices->add (dev); };
    inline List<Device> *GetDevices () const { return this->devices; };

    void ListDevices ();

private:
    List<Device> *devices;
};

extern "C" void AddDevice (Device *dev);

extern DeviceManager *devman;

}

#endif
