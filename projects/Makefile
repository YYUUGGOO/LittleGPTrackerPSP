# PSP-only LittleGPTracker build
.SUFFIXES:

PLATFORM := PSP
FONT := original.bmp

include $(PWD)/Makefile.PSP

TITLE := lgpt
BUILD := buildPSP

COMMONDIRS := \
	../sources/System/Console \
	../sources/System/io \
	../sources/Services/Midi \
	../sources/Services/Time \
	../sources/Services/Controllers \
	../sources/System/FileSystem \
	../sources/System/Timer \
	../sources/System/Errors \
	../sources/UIFramework/Framework \
	../sources/UIFramework/BasicDatas \
	../sources/UIFramework/SimpleBaseClasses \
	../sources/UIFramework/Interfaces \
	../sources/Application \
	../sources/Application/Controllers \
	../sources/Application/Commands \
	../sources/Application/Audio \
	../sources/Application/FX \
	../sources/Application/Views \
	../sources/Application/Views/ModalDialogs \
	../sources/Application/Views/BaseClasses \
	../sources/Application/Model \
	../sources/Application/Utils \
	../sources/Application/Player \
	../sources/Application/Mixer \
	../sources/Application/Instruments \
	../sources/Application/Persistency \
	../sources/Foundation \
	../sources/Services/Audio \
	../sources/Foundation/Services \
	../sources/Foundation/Variables \
	../sources/Externals/Compression \
	../sources/Externals/Soundfont \
	../sources/Externals/TinyXML

PSPDIRS := \
	. \
	../sources/Adapters/PSP/Main \
	../sources/Adapters/PSP/FileSystem \
	../sources/Adapters/PSP/Midi \
	../sources/Adapters/PSP/System \
	../sources/System/Process \
	../sources/Adapters/SDL/Process \
	../sources/Adapters/SDL/Audio \
	../sources/Adapters/SDL/GUI \
	../sources/Adapters/SDL/Timer

COMMONFILES := \
	Logger.o Trace.o Result.o Status.o Config.o \
	Timer.o FileSystem.o FontConfig.o \
	SysMutex.o TimeService.o \
	MidiOutDevice.o MidiInDevice.o MidiService.o Groove.o \
	MidiChannel.o MidiInMerger.o \
	GUIEvent.o GUIRect.o EventManager.o GUIWindow.o \
	Channel.o Mixer.o ControlRoom.o MultiChannelAdapter.o \
	CommandDispatcher.o ApplicationCommandDispatcher.o EventDispatcher.o \
	ControllerService.o ButtonControllerSource.o KeyboardControllerSource.o \
	JoystickControllerSource.o HatControllerSource.o ControllerSource.o ControlNode.o \
	I_GUIWindowImp.o \
	Application.o AppWindow.o SelectProjectDialog.o NewProjectDialog.o \
	ImportSampleDialog.o MixerView.o NullView.o SongView.o ChainView.o \
	PhraseView.o ViewData.o ConsoleView.o ProjectView.o InstrumentView.o UIController.o \
	View.o ModalView.o FieldView.o UIField.o UIIntField.o UIIntVarOffField.o \
	UIIntVarField.o ViewEvent.o I_Action.o UITempoField.o UIActionField.o \
	MessageBox.o CommandSelectorModal.o GrooveView.o UINoteVarField.o \
	UIBigHexVarField.o SRPUpdaters.o UIStaticField.o \
	Song.o Chain.o Phrase.o Project.o Scale.o char.o n_assert.o fixed.o wildcard.o \
	SyncMaster.o TablePlayback.o Player.o Table.o TableView.o \
	InstrumentBank.o WavFileWriter.o WavFile.o MidiInstrument.o Filters.o \
	SampleVariable.o SampleInstrument.o SamplePool.o CommandList.o FxPrinter.o \
	PersistencyService.o Persistent.o Observable.o SingletonRegistry.o \
	Audio.o AudioMixer.o AudioOutDriver.o AudioDriver.o AudioOut.o DummyAudioOut.o \
	PlayerChannel.o AudioFileStreamer.o MixBus.o MixerService.o PlayerMixer.o \
	Service.o ServiceRegistry.o SubService.o Variable.o VariableContainer.o WatchedVariable.o \
	SoundFontPreset.o SoundFontManager.o SoundFontSample.o \
	ENAB.o HYDRA.o OMEGA.o RIFF.o SFDETECT.o SFLOOKUP.o SFNAV.o SFREADER.o WIN_MEM.o \
	HexBuffers.o lz.o tinyxmlparser.o tinyxml.o tinyxmlerror.o tinystr.o Tiny2NosStub.o

PSPFILES := \
	PSPmain.o PSPFileSystem.o Process.o PSPMidiService.o PSPMidiOutDevice.o \
	GUIFactory.o SDLGUIWindowImp.o SDLEventManager.o SDLAudioDriver.o SDLAudio.o \
	SDLProcess.o SDLTimer.o PSPSystem.o

TARGET := $(TITLE)
SOURCES := $(PSPDIRS) $(COMMONDIRS)
INCLUDES :=

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT := $(CURDIR)/$(TARGET)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)
export LD := $(CXX)
export OFILES := $(COMMONFILES) $(PSPFILES)
export OBJS := $(OFILES)
export INCLUDE := -I$(CURDIR)/$(BUILD)

.PHONY: $(BUILD) clean

$(BUILD): ../sources/Resources/font.h
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(PWD)/Makefile

../sources/Resources/font.h:
	python3 ../sources/Resources/mkfont.py ../sources/Resources/$(FONT) $@

clean:
	@echo clean ...
	@rm -fr $(OUTPUT).$(EXTENSION) $(BUILD) *.elf

else

DEPENDS := $(OFILES:.o=.d)
include $(PSPSDK)/lib/build.mak

.PHONY: UsbMidiDriver.prx
UsbMidiDriver.prx: $(PWD)/../psp-usb-midi/dist/PspUsbMidi/kernel/UsbMidiDriver.prx
	cp $< $@

$(OUTPUT).elf: $(OFILES)

-include $(DEPENDS)

endif
