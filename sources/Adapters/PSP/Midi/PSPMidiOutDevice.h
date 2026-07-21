#ifndef _PSP_MIDI_OUT_DEVICE_H_
#define _PSP_MIDI_OUT_DEVICE_H_

#include "Services/Midi/MidiOutDevice.h"

#include <string>

class PSPMidiOutDevice : public MidiOutDevice {
  public:
    PSPMidiOutDevice(const char *name, const char *driverPath);
    virtual ~PSPMidiOutDevice();

    virtual bool Init();
    virtual void Close();
    virtual bool Start();
    virtual void Stop();
    virtual void SendQueue(T_SimpleList<MidiMessage> &queue);
    virtual void SendMessage(MidiMessage &message);

  private:
    static int ClockThread(unsigned int args, void *argp);
    int RunClock();
    bool StartClock();
    void StopClock();
    int WriteMessage(MidiMessage &message);
    int WriteEvents(void *events, int count);

    std::string driverPath_;
    bool initialized_;
    volatile bool running_;
    volatile bool clockRunning_;
    int clockThread_;
    unsigned long long usbStartedAt_;
};

#endif
