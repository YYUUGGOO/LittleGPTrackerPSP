#include "PSPMidiService.h"

#include "PSPMidiOutDevice.h"

static const char *PSP_USB_MIDI_DEVICE_NAME = "PSP USB MIDI";

PSPMidiService::PSPMidiService(const char *driverPath)
    : driverPath_(driverPath) {}

PSPMidiService::~PSPMidiService() {}

void PSPMidiService::buildDriverList() {
    Insert(new PSPMidiOutDevice(PSP_USB_MIDI_DEVICE_NAME,
                                driverPath_.c_str()));
    SelectDevice(PSP_USB_MIDI_DEVICE_NAME);

    // Enumerate while LGPT is loading, rather than on the first Play press.
    // The USB transport has a conservative host-attach settle period, so this
    // keeps the initial MIDI Start and clock ticks out of that window.
    startDevice();
}
