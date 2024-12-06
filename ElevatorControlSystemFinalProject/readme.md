# Elevator Control System

Link to the YOUTUBE Video for the demo
https://www.youtube.com/watch?v=QGCwXBBeATc

## Overview

##### The Elevator Control System is a software project implemented on an STM32 microcontroller using FreeRTOS v1. This system provides a Command-Line Interface (CLI) to simulate and control an elevator with the following features:

#####    1. Supports up to 5 floors.
#####    2. Accepts user commands via UART for operations like opening/closing doors, handling emergencies, and moving between floors.
#####    3. Implements tasks for floor selection, elevator movement, and real-time system updates.
#####    4. Uses FreeRTOS for multitasking and efficient resource management.
#####    5. Provides a visual, interactive CLI with ANSI escape sequences for enhanced user feedback.
#####    6. Emergency Button Integration: Includes a button for emergency stops.
#####    7. Maintenance Mode: Allows the elevator to be put into and released from maintenance mode via CLI.
#####    8. Elevator Call: Users can call the elevator to their current floor.

##  How It Works

###    Startup:
####        The system initializes and displays the elevator's status in the CLI.

##    Elevator Status Prompt:

###        The status includes the current floor, door status, and elevator state, as shown below:

```sh
Elevator Status:
Current Floor: 1
Door Status: Closed
Elevator State: Idle
```
#### Commands: Please enter open to open the door
#### To call the elevator to different floor Please enter the floor number you on
#### Please enter M for maintenance
####    Enter a command:

## User Commands:

### The user enters commands via the CLI:
| Input                               |                                                                                        |
|-------------------------------------|----------------------------------------------------------------------------------------|
| open                                | Opens the elevator door. If the door is already open, the system notifies the user.    |
| close                               | Closes the elevator door. If the door is already closed, the system notifies the user. |
| pressing the external button        | Triggers an emergency stop, halting the elevator's operation.                          |
| M                                   | Engages maintenance mode.                                                              |
| entering number from 1 - 5          | Calls the elevator to the specified floor.                                             |

## Elevator Call Feature

### The elevator call feature allows users to summon the elevator to their current floor. Here's how it works:

###  1. Call the Elevator:
####      If the elevator is idle at a different floor, users can call it by entering their floor number.
####  example

```sh
Enter a command: 5
Elevator Status: Moving Up
```

### 2. Elevator Movement:

####    The elevator moves to the requested floor.
####    Real-time updates are displayed in the CLI, including the current floor and elevator state 

### 3. Elevator Movement:

####    The elevator moves to the requested floor.
####    Real-time updates are displayed in the CLI, including the current floor and elevator state 

```sh
Door Status: Open
Elevator Status: Idle
```

## Emergency Stop Button

### An external emergency stop button is connected to pin PB0 on the STM32 microcontroller. The button is used to:

###  1. Activate Emergency Stop:
####      Pressing the button causes the elevator to immediately stop at the nearest floor or the floor it was passing by.
####      The elevator doors automatically open, allowing passengers to exit quickly.
####  The CLI status updates:

```sh
Door Status: Open
Elevator Status: Emergency Stop
```

### 2. Release Emergency Stop:

####    Pressing the button again releases the elevator from the emergency stop status.
####    The elevator remains idle at the current floor, with the doors remaining open.
####  The CLI status updates:

```sh
Door Status: Open
Elevator Status: Idle
```

### 3. Reset Required:

####    After being released from emergency stop, the elevator cannot be used until it is reset. By pressing on the black button on the STM32.
####    This ensures proper maintenance or verification before resuming normal operations.



## Maintenance Mode

### The maintenance mode can be activated via the CLI by entering M. Here's how it works:

###  1. Activate Maintenance Mode:
####      The elevator status changes to Out of Order (Maintenance).
####      The system displays a prompt for the maintenance personnel:

```sh
Door Status: Open
Elevator Status: Emergency Stop
```

### 2. Release Maintenance Mode:

####    Maintenance personnel must enter exit to release the elevator from maintenance mode.
####    Upon exiting, the elevator will need to be reset by pressing on the black button on the STM32. for it becomes ready for normal operation.
####  CLI Example:

```sh
Enter a command (open/close/E/M): M
Elevator Status: Out of Order (Maintenance)
Enter 'exit' when done with the maintenance:
exit
Exiting maintenance mode. Returning to normal operation.
Elevator Status: Idle
Door Status: Closed
```

## Floor Selection

### Once the door is open, the system prompts the user to select a floor (1–5). For example:

```sh
Please enter the floor (1-5):
```

###  The elevator moves to the requested floor, updating the status in real time. In case of emergencies, the elevator halts and follows the Emergency Stop process.


## Future Work:


### 1. Multi-Floor Stops

#### Future iterations will enable the elevator to process multiple floor requests simultaneously. For example:

##### User inputs: 2 4 5
##### The elevator will stop sequentially at each floor, displaying real-time status updates for current floor, door status, and elevator state at each stop.

### 2. Code Enhancements

#### Further refactoring for improved modularity and error handling.



