# PSP build

With `PSPDEV` installed and on `PATH`, run:

```sh
make -j4
```

The build creates `buildPSP/EBOOT.PBP` and copies the matching USB MIDI kernel driver to `buildPSP/UsbMidiDriver.prx`.

To force a clean rebuild:

```sh
make clean
make -j4
```
