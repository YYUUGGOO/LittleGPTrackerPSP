#ifndef _PSP_MIDI_SERVICE_H_
#define _PSP_MIDI_SERVICE_H_

#include "Services/Midi/MidiService.h"

#include <string>

class PSPMidiService : public MidiService {
  public:
    PSPMidiService(const char *driverPath);
    virtual ~PSPMidiService();
    virtual void buildDriverList();

  private:
    std::string driverPath_;
};

#endif
