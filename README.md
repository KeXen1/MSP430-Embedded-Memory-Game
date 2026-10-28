
# MSP430 Embedded Memory Game

An interactive memory game built in Embedded C using the MSP430G2553 microcontroller. Players repeat an LED sequence by pressing the corresponding buttons. Each successful round increases the sequence length, making the game progressively more challenging.

## Features

- Four LEDs and four push buttons for gameplay
- Pseudo-random LED sequence generation
- Progressive difficulty with increasingly longer sequences
- Player input validation and game-over detection
- Win animation and game restart functionality
- Finite state machine to organize game logic

## Hardware

- MSP-EXP430G2ET LaunchPad
- MSP430G2553 microcontroller
- 4 external LEDs
- 4 push buttons
- 100Ω resistors
- Breadboard and jumper wires

## Software and Tools

- Embedded C
- Code Composer Studio (CCS)
- MSP430 GPIO and Timer_A peripherals

## How It Works

1. The game generates a sequence of LED flashes.
2. The player repeats the sequence using the four buttons.
3. The program checks each input against the stored sequence.
4. Correct answers advance to a longer sequence.
5. An incorrect answer triggers the game-over state.
6. Completing the maximum sequence triggers the win animation.

## Skills Demonstrated

Embedded C, microcontroller programming, GPIO configuration, timer peripherals, finite state machines, input handling, and hardware debugging.

## Project Status

Completed — October 2026.
