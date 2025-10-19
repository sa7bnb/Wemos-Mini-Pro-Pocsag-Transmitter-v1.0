# POCSAG Transmitter for CC1101

A RadioLib-based POCSAG transmitter implementation for the CC1101 radio module with configurable frequency offset capability.

**Author**: Anders Isaksson (SA7BNB)  
**Hardware Design**: Anders Isaksson  
**Software Development**: Anders Isaksson (based on RadioLib)

## Overview

This project enables transmission of POCSAG (Post Office Code Standardisation Advisory Group) pager messages using a CC1101 radio transceiver module connected to a Wemos D1 Mini Pro. The implementation includes a frequency offset feature for precise frequency calibration.

## Hardware Requirements

- **Wemos D1 Mini Pro** (or compatible ESP8266 board)
- **CC1101 transceiver module**
- **LED** (optional, for transmission indicator)

### Wiring Configuration

| CC1101 Pin | Wemos D1 Mini Pro Pin |
|------------|----------------------|
| CS         | GPIO 15              |
| GDO0       | GPIO 4               |
| GDO2       | GPIO 2               |
| RST        | Not connected        |
| SCK        | GPIO 14 (automatic)  |
| MISO       | GPIO 12 (automatic)  |
| MOSI       | GPIO 13 (automatic)  |
| VCC        | 3.3V                 |
| GND        | GND                  |

**LED:** Connect to GPIO 5 (optional)

## Features

- **Interactive Serial Interface**: Send messages via serial monitor with real-time input
- **Flexible Message Format**: Support for both default and custom RIC (Radio Identity Code) addressing
- **Frequency Offset Calibration**: Adjustable frequency offset for precise tuning
- **Maximum Power Output**: Configured for +10 dBm output power
- **Visual Feedback**: LED indicator for successful transmissions
- **Input Validation**: Built-in checks for RIC and message validity

## Software Requirements

- Arduino IDE or PlatformIO
- [RadioLib library](https://github.com/jgromes/RadioLib)

## Installation

1. Install the RadioLib library in your Arduino IDE:
   - Go to **Sketch → Include Library → Manage Libraries**
   - Search for "RadioLib"
   - Install the latest version

2. Clone or download this repository

3. Open the `.ino` file in Arduino IDE

4. Configure your board settings:
   - Board: "LOLIN(WEMOS) D1 mini Pro"
   - Upload Speed: 921600
   - CPU Frequency: 80 MHz

5. Upload to your Wemos D1 Mini Pro

## Configuration

### Frequency Settings

Adjust these parameters at the top of the code:

```cpp
float BASE_FREQUENCY = 433.92;      // Nominal frequency (MHz)
float FREQUENCY_OFFSET = 0.015;     // Offset in MHz (15 kHz)
```

- **BASE_FREQUENCY**: The nominal operating frequency
- **FREQUENCY_OFFSET**: Fine-tuning offset in MHz
  - Positive values increase frequency
  - Negative values decrease frequency

### Power Output

The transmitter is configured for maximum power (+10 dBm, approximately 10 mW). Modify the power setting in the code if needed:

```cpp
radio.setOutputPower(10);  // Range: -30 to +10 dBm
```

## Usage

### Message Format

The transmitter supports two input formats:

1. **With custom RIC**: `RIC::Message`
   - Example: `123456::Hello World`
   - RIC must be between 1 and 2,097,151

2. **Without RIC**: `Message`
   - Example: `Test message`
   - Uses default RIC 123456

### Sending Messages

1. Open the Serial Monitor (115200 baud)
2. Wait for the prompt: `Enter command:`
3. Type your message in one of the supported formats
4. Press Enter to transmit
5. Watch for transmission confirmation

### Message Limitations

- Maximum message length: 40 characters
- Longer messages are automatically truncated
- Only ASCII characters are supported

## Example Session

```
RadioLib POCSAG Transmitter Starting...
Base frequency: 433.92000 MHz
Frequency offset: 0.01500 MHz
Actual transmit frequency: 433.93500 MHz
[CC1101] Initializing ... success!
[Pager] Initializing ... success!
[CC1101] Setting maximum power ... MAX POWER SET!

=== POCSAG Message Sender ===
Format: RIC::Message (e.g., '123456::Hello World')
Or just: Message (uses default RIC 123456)
TX Frequency: 433.93500 MHz
Enter command: 123456::Test message
Sending: 'Test message' to RIC 123456
[Pager] Transmitting ... SUCCESS!
Message sent successfully!
```

## Technical Details

- **Protocol**: POCSAG
- **Baud Rate**: 1200 bps
- **Modulation**: FSK (Frequency Shift Keying)
- **Encoding**: ASCII
- **Default Frequency**: 433.92 MHz (adjustable)
- **RIC Range**: 1 to 2,097,151 (21-bit addressing)

## Legal & Safety Notice

⚠️ **Important**: 
- Ensure you have the appropriate license to transmit on your chosen frequency
- Respect local regulations regarding radio transmissions
- Be aware of frequency allocations in your country
- This project is intended for educational and amateur radio use only
- The author assumes no liability for misuse or regulatory violations

## Troubleshooting

### No transmission
- Check wiring connections
- Verify power supply (3.3V for CC1101)
- Confirm antenna is connected
- Check serial monitor for error codes

### Frequency offset issues
- Use an SDR or spectrum analyzer to verify actual transmission frequency
- Adjust `FREQUENCY_OFFSET` in small increments (0.001 MHz)
- Temperature can affect frequency stability

### Message not received
- Verify RIC matches receiver configuration
- Confirm receiver is tuned to the correct frequency
- Check transmission power and antenna placement

## License

This project is provided as-is for educational purposes. Please check RadioLib's license for library-specific terms.

## Acknowledgments

- Hardware design and software implementation by **Anders Isaksson (SA7BNB)**
- Built using the excellent [RadioLib](https://github.com/jgromes/RadioLib) by Jan Gromeš
- POCSAG protocol implementation by RadioLib contributors

## References

- [POCSAG Protocol Information](https://en.wikipedia.org/wiki/POCSAG)
- [CC1101 Datasheet](https://www.ti.com/product/CC1101)
- [RadioLib Documentation](https://jgromes.github.io/RadioLib/)
