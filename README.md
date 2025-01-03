
# Sonic SDR

A real-time audio spectrum visualizer using ALSA and ncurses, designed to process audio input and display its frequency spectrum as a waterfall plot.

## Features

- Captures audio from ALSA-supported devices.
- Configurable sample rate, buffer size, and skip buffers.
- Interactive ncurses-based user interface.
- Real-time visualization of audio frequency spectrum.
- Adjustable frequency range and gain settings.
- Keyboard controls for intuitive interaction.

## Prerequisites

- **ALSA**: Ensure ALSA is installed on your system.
- Ensure your terminal supports RGB colors

## Building the Project

1. Clone the repository:

   ```bash
   git clone https://github.com/radiopushka/SonicSDR
   cd SonicSDR
   ```

2. Build the project:

   ```bash
   make
   ```

## Running the Application

Run the program with the following syntax:

```bash
./ftaudio
./ftaudio <alsa_device> <sample_rate> <buffer_size> <skip_buffer>
```

### Arguments (optional)

- `<alsa_device>`: ALSA audio device (default: `"default"`).
- `<sample_rate>`: Audio sample rate in Hz (default: `48000`).
- `<buffer_size>`: Receive buffer size (default: `4000`).
- `<skip_buffer>`: Number of buffers to skip before visualization (default: `0`).

### Example

```bash
./ftaudio
./ftaudio hw:0,0 44100 2048 1
```

## Keyboard Controls

- **↑**: Increase gain.
- **↓**: Decrease gain.
- **→**: Move frequency cursor right.
- **←**: Move frequency cursor left.
- **Enter**: Set the waterfall's maximum frequency.
- **Tab**: Set the waterfall's minimum frequency.
- **r**: Reset the frequency range to default.
- **h**: Display help for controls.
- **Space**: Pause or resume visualization.
- **q**: Quit the application.

## Output

- Displays the real-time frequency spectrum with adjustable gain and frequency ranges.
- Highlights frequency markers and allows precise control over visualization parameters.

## Screenshots

![image on sdr](https://github.com/radiopushka/SonicSDR/blob/main/Screenshots/2025-01-02_17-18.png)

![image on sdr](https://github.com/radiopushka/SonicSDR/blob/main/Screenshots/2025-01-01_23-01.png)

![digital ultrasonic signal](https://github.com/radiopushka/SonicSDR/blob/main/Screenshots/2025-01-01_23-07.png)

![music in DSB](https://github.com/radiopushka/SonicSDR/blob/main/Screenshots/2025-01-01_23-12.png)

![music in DSB](https://github.com/radiopushka/SonicSDR/blob/main/Screenshots/2025-01-01_23-14.png)

![music in DSB](https://github.com/radiopushka/SonicSDR/blob/main/Screenshots/2025-01-01_23-16.png)

## Issues and Contributions

Feel free to report issues or contribute improvements to the project. Open a pull request or file an issue in the [issues section](https://github.com/radiopushka/SonicSDR/issues).
