# Automated Dough Flattening Machine

## Project Overview
An Arduino-based automated dough flattening system designed for consistent and efficient dough processing. The machine automates the entire process from dough extrusion to flattening, making it ideal for small to medium-scale bakery operations.

## Key Features
- Automated dough extrusion system
- Precision iris-based cutting mechanism
- Rotating base for even flattening
- Height-adjustable roller system
- Serial command interface
- Safety-first design with emergency stop
- Cycle-based operation for consistent results

## Hardware Components
### Control System
- Arduino Mega 2560 microcontroller
- 7 relay modules for motor control

### Mechanical Components
- DC motor (3.5RPM) for roller height adjustment
- DC motors for hopper movement
- Iris mechanism with servo control
- Base rotation motor
- Custom-designed hopper with rack and pinion

### Pin Configuration
```
Hopper Control:
- Forward: Pin 22
- Backward: Pin 24

Iris Mechanism:
- Open: Pin 28
- Close: Pin 26

Motor Control:
- Base Rotation: Pin 30
- Roller Up: Pin 34
- Roller Down: Pin 32
```

## Operation Guide
### Command Interface
The machine accepts three basic commands via Serial Monitor:
1. `start` - Initiates a complete processing cycle
2. `stop` - Emergency stop and system reset
3. `refill` - Activates hopper refill sequence

### Processing Cycle
Each cycle follows a precise sequence:
1. System initialization
2. Roller positioning
3. Dough extrusion (1 second)
4. Precision cutting
5. Base rotation and flattening
6. 10-second rest period
7. Return to home position

### Safety Features
- Complete cycle protection
- Emergency stop capability
- Automatic roller homing
- Motor overload protection
- Controlled refill operation

## Technical Specifications
- Operating Voltage: 12V DC
- Control Logic: 5V DC
- Roller Speed: 3.5 RPM
- Cycle Time: ~15 seconds
- Maximum Dough Capacity: Based on hopper size
- Communication: Serial (9600 baud)

## Installation
1. Clone the repository
```bash
git clone https://github.com/yourusername/dough-flattening-machine.git
```

2. Install Arduino IDE
3. Install required libraries
4. Upload code to Arduino Mega 2560

## Setup Instructions
1. Connect all motors according to pin configuration
2. Power up the system
3. Open Serial Monitor (9600 baud rate)
4. Ensure line ending is set to "Newline"
5. System is ready for commands

## Usage Example
```
1. Open Serial Monitor
2. Type "start" and press enter
3. Machine will complete one cycle
4. Wait for "Cycle complete" message
5. Repeat for next batch
```

## Project Structure
```
├── src/
│   ├── main.cpp          # Main control logic
│   └── config.h          # Configuration settings
├── docs/
│   ├── images/           # System diagrams
│   └── manual.pdf        # Detailed manual
└── README.md             # Project documentation
```

## Contributing
1. Fork the repository
2. Create feature branch
3. Commit changes
4. Push to branch
5. Create Pull Request

## Future Enhancements
- [ ] LCD display interface
- [ ] Variable speed control
- [ ] Dough thickness adjustment
- [ ] Temperature monitoring
- [ ] WiFi connectivity
- [ ] Mobile app control

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments
- Project Supervisor: Nandish Hambi 
- Department of E&C
- KLE Technological University


## Project Status
 Core functionality complete
 Testing phase
 Ongoing improvements
