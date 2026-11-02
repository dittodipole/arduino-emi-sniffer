# Arduino EMI Sniffer

**A simple near-field probe that makes electromagnetic interference visible with an Arduino and four LEDs**

I built this while teaching myself the basics of EMC (electromagnetic compatibility), antenna theory and radio signals. The goal was to see electromagnetic interference with my own hardware: which everyday devices radiate noise, and how strongly.

<!-- Add a photo of the setup here -->
<!-- Add the demo video here (drag an MP4 under ~10 MB into the GitHub editor, or link an unlisted YouTube video) -->

## How it works

```
[Wire probe] → [A0, with 1 MΩ to GND] → [Arduino ADC] → [4-LED bar graph]
```

- **Probe:** a piece of breadboard jumper wire acting as a simple, untuned near-field probe. It isn't designed for any specific frequency band.
- **1 MΩ pull-down resistor:** connects A0 to ground. Without it, the input would float and drift with static charge. With it, the pin rests near 0 V, and only voltages induced in the wire by nearby fields show up.
- **Arduino:** reads the induced voltage with its 10-bit ADC (`analogRead`, 0–1023) and maps it to a bar graph of four LEDs on digital pins.

No radio modules are used. The wire itself is the sensor.

## Experiments

I compared three sources against a quiet baseline. The video shows each test.

| Test | Result | Interpretation |
|---|---|---|
| **Baseline** (nothing nearby) | LEDs off | The 1 MΩ resistor keeps the input stable at rest |
| **Laptop charger** | Barely any response | Little detectable emission with this probe (see limitations) |
| **Touching the wire** | Strong spike | My body is capacitively coupled to the mains wiring in the room and picks up 50 Hz; touching the wire passes that straight into the input |
| **Running DC motor (unshielded)** | All LEDs lit | Brushed motors are a classic noise source: sparking at the brushes and switching of the coil currents create strong electromagnetic interference |

## Limitations

This is a qualitative demonstration, not a measurement instrument. Understanding why was one of the most useful parts of the project:

- **Sampling rate:** `analogRead` takes at most about 10,000 samples per second. Switching power supplies typically operate at tens to hundreds of kHz, far above what the ADC can resolve. Such signals are aliased, so the probe can indicate *that* there is noise, but not its frequency.
- **No calibration:** the readings are relative. They can compare sources in the same setup, but not show absolute field strength or whether a device meets EMC limits.
- **Negative voltages are lost:** the ADC only measures 0–5 V, so the negative half of an AC signal is cut off.
- **Proper EMC testing** uses calibrated near-field probes with a spectrum analyzer, which shows the signal strength at each frequency.

## What I learned

- The basics of how antennas and near-field probes pick up electric and magnetic fields
- Why floating inputs need a defined reference (pull-down resistor)
- How common noise sources like brushed motors and capacitive body coupling behave
- What separates a simple demonstration from a real EMC measurement: bandwidth, sampling rate, calibration

## Hardware

- Arduino (Elegoo starter kit)
- Jumper wire as the probe
- 1 MΩ resistor
- 4 LEDs with current-limiting resistors

### Pin assignment

| Component | Pin |
|---|---|
| Probe wire (with 1 MΩ to GND) | A0 |
| LED bar graph (lowest to highest) | D2, D3, D4, D5 |

## Code

The sketch is in [`firmware/emi_sniffer/emi_sniffer.ino`](firmware/emi_sniffer/emi_sniffer.ino). Every 20 ms it reads A0, prints the raw value for the Serial Plotter, and lights the LEDs as a bar graph: each LED turns on when the reading passes its threshold (2, 5, 10 and 20 on the 0–1023 scale).

## Possible improvements

- **Peak-to-peak measurement:** the sketch takes one reading every 20 ms, so it only catches a random moment of a fast-changing signal. Sampling continuously and measuring the range between the highest and lowest value over a short window would give a much more stable noise level. I later used this method for the sound sensor in my desk companion project, Maru.
- **Better probes:** a shielded loop probe for magnetic fields and a small stub probe for electric fields, to tell the two apart.
- **Faster sampling:** an ESP32 or a dedicated ADC to capture higher frequencies, plus an FFT to show which frequencies are present.
