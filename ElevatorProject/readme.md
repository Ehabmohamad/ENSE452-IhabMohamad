# Elevator Control System

Link to the YOUTUBE Vieo for the demo
https://youtu.be/uDELq09hyK0

## Overview

##### The Elevator Control System is a software project implemented on an STM32 microcontroller using FreeRTOS v1. This system provides a command-line interface (CLI) to simulate and control an elevator with the following features:

#####    1. Supports up to 5 floors.
#####    2. Allows users to send commands via UART to open/close doors, handle emergencies, and move between floors.
#####    3. Implements tasks for floor selection, elevator movement, and real-time system updates.
#####    4. Utilizes FreeRTOS for multitasking and efficient resource management.
#####    5. Includes visual CLI feedback using ANSI escape sequences for a clear and interactive user interface.

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
#### Please enter M for maintenance
####    Enter a command (open/close/E/M):

## User Commands:

### The user enters commands via the CLI:
| Input   |                                                                                        |
|---------|----------------------------------------------------------------------------------------|
| open    | Opens the elevator door. If the door is already open, the system notifies the user.    |
| close   | Closes the elevator door. If the door is already closed, the system notifies the user. |
| E       | Triggers an emergency stop.                                                            |
| M       | Engages maintenance mode.                                                              |

## Floor Selection:

### Once the door is open, the user is prompted to enter a floor number (1–5). For example:

#### Please enter the floor (1-5): 


## Future Work:


##### One of the major improvements will involve enabling the elevator to handle multi-floor stops. Currently, the system processes one floor request at a time, but future iterations will allow users to enter multiple floor numbers in a single prompt. For instance, if the user inputs 2 4 5, the elevator will stop sequentially at each floor, displaying real-time status updates such as the current floor, door status, and elevator state at each stop. The system will automatically handle the sequence of stops, pausing briefly at each floor before proceeding to the next. 

##### Hardware integration will include adding five LEDs to represent floors, lighting up when the elevator reaches the respective floor. Additionally, three  physical buttons will be introduced: an emergency button to stop the elevator, an open-door button for manual door control, and a maintenance button to engage maintenance mode.

##### Code improvements will focus on fixing the door status issue, ensuring dynamic updates for all states, and refactoring the system for better modularity and error handling. 




