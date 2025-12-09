# Flipper Zero Ruler App

A digital ruler application for Flipper Zero that displays a 1:1 scale ruler on the device's screen.

## Features

- 1:1 scale ruler display (calibrated for Flipper Zero screen)
- Centimeter and millimeter graduations
- Labeled centimeter marks (0, 1cm, 2cm, 3cm)
- Bold 5mm mid-marks for easier reading
- 1mm fine graduations
- Simple and intuitive interface

## Screenshots

The ruler displays on Flipper Zero's 128x64 pixel screen, providing measurements up to approximately 3 cm.

## Technical Details

Flipper Zero screen specifications:
- Resolution: 128x64 pixels
- Display size: 1.4 inches
- Active area width: ~30mm
- Controller: ST7567

Calibration used in this app:
- ~4.1 pixels = 1mm
- 41 pixels = 1cm

**Important:** Due to screen size limitations (~30mm width), the ruler can measure objects up to approximately 3cm. Minor inaccuracies may occur due to pixel rounding.

## Installation

### Method 1: Via qFlipper (Recommended)

1. Install [qFlipper](https://flipperzero.one/update) on your computer
2. Connect Flipper Zero via USB
3. Open qFlipper and navigate to "File Manager"
4. Navigate to `apps/Tools/` folder
5. Copy the compiled `ruler.fap` file to this folder

### Method 2: Via SD Card

1. Remove the microSD card from your Flipper Zero
2. Connect it to your computer
3. Copy `ruler.fap` to `/ext/apps/Tools/` folder
4. Insert the card back into Flipper Zero

### Method 3: Direct Installation

Download the latest `ruler.fap` from the [Releases](https://github.com/misht-world/Flipper-ruler/releases) page.

## Building from Source

### Using uFBT (Recommended)

```bash
# Install uFBT
pip install ufbt

# Clone the repository
git clone https://github.com/misht-world/Flipper-ruler.git
cd Flipper-ruler

# Build the application
ufbt

# The ruler.fap file will be created in dist/ folder
```

### Using Full Flipper Build Tool

```bash
# Clone the firmware repository
git clone https://github.com/flipperdevices/flipperzero-firmware.git
cd flipperzero-firmware

# Copy the app folder
cp -r /path/to/Flipper-ruler applications_user/ruler

# Build the application
./fbt fap_ruler

# Output: dist/f7-D/apps/ruler/ruler.fap
```

## Usage

1. On Flipper Zero, open the "Apps" menu
2. Navigate to "Tools" category
3. Select the "Ruler" application
4. Place an object against the screen (align to the left edge at 0)
5. Read the measurement from the ruler markings
6. Press "Back" button to exit

## Calibration

If you find the ruler inaccurate on your specific device, you can adjust the calibration constant `pixels_per_cm` in `ruler.c` (line 31):

```c
const uint8_t pixels_per_cm = 41; // Adjust this value for calibration
```

After changing, recompile the application.

## Limitations

- Maximum measurement length: ~3cm
- Accuracy limited by screen resolution (~0.24mm per pixel)
- Individual device calibration may be required for highest accuracy
- Approximate measurements only - not suitable for precision work

## Disclaimer

This application provides approximate measurements. Due to screen size, pixel density, and manufacturing variations, some inaccuracy is expected. This tool is intended for quick measurements and educational purposes only. For precise measurements, please use a physical ruler or measuring tool.

## License

MIT License - See LICENSE file for details

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## Credits

Created with assistance from Claude AI
Developed for the Flipper Zero community

## Version History

### v1.0 (2024)
- Initial release
- 1:1 scale ruler with cm and mm markings
- Calibrated for Flipper Zero screen
- Clean, minimal interface
