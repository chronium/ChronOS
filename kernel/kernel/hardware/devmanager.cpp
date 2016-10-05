#include <stdio.h>

#include <kernel/devmanager.h>

namespace DeviceManager {

DeviceManager *devman = new DeviceManager ();   

void DeviceManager::ListDevices () {
    for (int i = 0; i < devices->Count (); i++)
        printf ("Dev: %s\n", devices->get (i)->GetName ());
}

void AddDevice (Device *dev) {
    devman->AddDevice (dev);
}

}
