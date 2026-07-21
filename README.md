# LittleGPTracker PSP

A PSP-focused fork of [LittleGPTracker](https://github.com/djdiskmachine/LittleGPTracker), maintained at [YYUUGGOO/LittleGPTrackerPSP](https://github.com/YYUUGGOO/LittleGPTrackerPSP).

This fork keeps the shared tracker engine and only the platform code needed for PSP. Desktop and other handheld adapters, build targets, bundled libraries, and packaging resources have been removed.

## PSP-specific features

- Class-compliant USB MIDI output through the bundled `psp-usb-midi` driver
- MIDI notes, CC, program changes, pitch bend, Start, 24 PPQN Clock, and Stop
- USB MIDI initialized automatically at startup
- Dedicated monotonic MIDI clock thread, independent of sample and file loading
- Auditioning a non-MIDI instrument does not start external MIDI transport
- Optimized long-sample loading with validated WAV parsing and large sequential reads

The audio engine supports PCM WAV files with one or two channels and 8-bit or 16-bit samples. Samples are held in RAM during playback, so the total decoded sample set must still fit in the PSP's available memory.

## Build

Install the [PSPDEV toolchain](https://pspdev.github.io/), then:

```sh
export PSPDEV="/path/to/pspdev"
export PATH="$PSPDEV/bin:$PATH"
cd projects
make -j4
```

Build outputs are written to `projects/buildPSP/`:

- `EBOOT.PBP`
- `UsbMidiDriver.prx`

Copy both files plus `projects/resources/PSP/config.xml` and `mapping.xml` into the same folder under `ms0:/PSP/GAME/`.

## Repository layout

- `sources/Application`, `sources/Services`, `sources/System`: shared tracker engine
- `sources/Adapters/PSP`: PSP filesystem, system, main, and USB MIDI integration
- `sources/Adapters/SDL`: SDL components required by the PSP build
- `psp-usb-midi`: PSP USB MIDI library and kernel PRX
- `projects`: PSP-only build and install resources

General tracker documentation remains under [`docs`](docs). PSP installation details are in [`projects/resources/PSP/INSTALL_HOW_TO.txt`](projects/resources/PSP/INSTALL_HOW_TO.txt).

## Upstream and license

This project derives from LittleGPTracker by Marc Nostromo and later contributors. See the repository history and `CHANGELOG` for attribution. Distributed under the terms in [`LICENSE`](LICENSE).
