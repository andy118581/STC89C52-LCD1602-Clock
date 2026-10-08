# 8051 Digital Clock

A simple digital clock implemented with an **STC89C52RC (8051)** microcontroller using **Keil C51**, **Timer0 interrupt**, and an **LCD1602** display.

This project was developed as an embedded C practice project to learn MCU timer interrupts, time/date counting, and LCD1602 control.

## Features

* Digital clock display: `HH:MM:SS`
* Date display: `YYYY/MM/DD`
* AM/PM display
* Timer0 interrupt-based time counting
* Automatic seconds, minutes, and hours carry
* Automatic day/month/year carry
* Different month lengths
* Leap-year detection
* LCD1602 display

## Hardware

* STC89C52RC / 8051 MCU
* 11.0592 MHz crystal oscillator
* LCD1602
* Minimum system circuit
* 5V power supply

## Software

* C language
* Keil C51
* 8051 Timer0
* Interrupt
* LCD1602 driver

## Timer0 Configuration

The MCU uses an **11.0592 MHz crystal** in 12T mode.

The timer tick period is approximately:

```text
12 / 11.0592 MHz ≈ 1.085 μs
```

For approximately 1 ms timer overflow:

```text
1 ms / 1.085 μs ≈ 922 counts
```

Therefore, the Timer0 initial value is:

```text
65536 - 922 = 64614
```

The timer is configured in 16-bit mode:

```c
TMOD &= 0xF0;
TMOD |= 0x01;
```

Timer0 generates an interrupt approximately every 1 ms.

A software counter is then used to accumulate 1000 timer interrupts:

```c
T0Count++;

if(T0Count >= 1000)
{
    T0Count = 0;
    sec++;
}
```

This produces an approximately 1-second time base.

## Time and Date Counting

The program handles the following time carry operations:

```text
60 seconds → 1 minute
60 minutes → 1 hour
12 hours   → next AM/PM period
End of month → next month
December   → next year
```

The program also determines whether February has 28 or 29 days based on the Gregorian leap-year rule.

## Project Structure

```text
8051-Digital-Clock/
│
├── main.c
├── LCD1602.c
├── LCD1602.h
├── Delay.c
├── Delay.h
└── README.md
```

## Example Display

```text
PM: 00:54:04
      2026/10/09
```

## Learning Goals

This project was created to practice:

1. 8051 MCU programming
2. Timer0 configuration
3. Interrupt handling
4. Timer overflow calculation
5. Embedded C programming
6. LCD1602 interfacing
7. Time and date state management
8. Conditional logic and modulo operations

## Future Improvements

Possible improvements for this project include:

* Add physical buttons for time/date adjustment
* Improve timer accuracy using a more precise timer calculation
* Add 12/24-hour display mode
* Refactor time/date handling into independent functions

## Author

This project is part of my embedded C / MCU learning portfolio.
