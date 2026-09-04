# UART_RX

Bare-metal UART receive on the TI Tiva C LaunchPad (TM4C123GH6PM), with the
received byte shown on the on-board RGB LED. No TivaWare, no HAL — the peripherals
are brought up by writing their registers directly.

Written in 2018 while learning the part. It stays up because the register-level
version is the one that actually explains what a UART driver does.

## What it does

Brings up UART0 at **9600 baud, 8N1, FIFO off** from the 16 MHz system clock — the
same configuration as the transmit side:

```c
UART0_IBRD_R = 104;   // 16 MHz / (16 x 9600) = 104.17, integer part
UART0_FBRD_R = 11;    // 0.17 x 64 + 0.5, rounded -> fractional part
UART0_LCRH_R = 0x60;  // 8-bit word, no parity, 1 stop bit, FIFO off
UART0_CTL_R  = 0x301; // enable UART0, TXE, RXE
```

It then also enables Port F and configures PF1–PF3 as outputs, which are the
LaunchPad's red, blue and green LEDs:

```c
GPIO_PORTF_DIR_R = 0x0E;
GPIO_PORTF_DEN_R = 0x0E;
```

The main loop reads a byte and shifts it one place left onto Port F, so the low
three bits of whatever you send light the RGB LED:

```c
character = UART0Rx();
GPIO_PORTF_DATA_R = character << 1;   // bits 0-2 land on PF1-PF3
```

Receiving is a busy-wait on the flag register — spin while the receive FIFO is
empty, then read the data register:

```c
while((UART0_FR_R & 0x10) != 0);   // RXFE: receive FIFO empty
character = UART0_DR_R;
```

## Build and run

A Code Composer Studio project — import the directory into CCS, build, flash. Then
send it bytes over the virtual COM port and watch the LED:

```sh
screen /dev/ttyACM0 9600
```

Sending `1` lights red, `2` blue, `4` green; combinations mix them.

## See also

[UART_TX](https://github.com/islash8/UART_TX) — the transmit side.
