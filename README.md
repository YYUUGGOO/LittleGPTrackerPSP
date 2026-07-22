# LittleGPTracker PSP

A PSP-focused fork of [LittleGPTracker](https://github.com/djdiskmachine/LittleGPTracker), maintained at [YYUUGGOO/LittleGPTrackerPSP](https://github.com/YYUUGGOO/LittleGPTrackerPSP).

This fork keeps the shared tracker engine and only the platform code needed for PSP. Desktop and other handheld adapters, build targets, bundled libraries, and packaging resources have been removed.

## PSP-specific features

- Class-compliant USB MIDI output through the bundled `psp-usb-midi` driver
- MIDI notes, CC, program changes, pitch bend, Start, 24 PPQN Clock, and Stop
- USB MIDI initialized automatically at startup and can be turned off in the project settings
- Dedicated monotonic MIDI clock thread, independent of sample and file loading
- Optimized long-sample loading with validated WAV parsing and large sequential reads

The audio engine supports PCM WAV files with one or two channels and 8-bit or 16-bit samples. Samples are held in RAM during playback, so the total decoded sample set must still fit in the PSP's available memory. Which is 32MB for the PSP-1000 series and 64MB for the 2000/3000/Go/E1000 series. Most efficient file format for samples is mono, 16-bit PCM WAV@22.05 kHz.

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

## Releases and versioning

Every push to `master` builds the PSP package and publishes a GitHub Release using strict SemVer tags such as `v1.0.0`.

Automatic version bumps follow Conventional Commit messages:

- `fix:` and other changes bump the patch version.
- `feat:` bumps the minor version.
- `type!:` or a `BREAKING CHANGE:` footer bumps the major version.

The first automated release is `v1.0.0`. The release workflow can also be run manually with an explicit patch, minor, or major bump.

## Repository layout

- `sources/Application`, `sources/Services`, `sources/System`: shared tracker engine
- `sources/Adapters/PSP`: PSP filesystem, system, main, and USB MIDI integration
- `sources/Adapters/SDL`: SDL components required by the PSP build
- `psp-usb-midi`: PSP USB MIDI library and kernel PRX
- `projects`: PSP-only build and install resources

General tracker documentation remains under [`docs`](docs). PSP installation details are in [`projects/resources/PSP/INSTALL_HOW_TO.txt`](projects/resources/PSP/INSTALL_HOW_TO.txt).

## Upstream and license

This project derives from LittleGPTracker by Marc Nostromo and later contributors. See the repository history and `CHANGELOG` for attribution. Distributed under the terms in [`LICENSE`](LICENSE).
