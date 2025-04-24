# LoRa_stm8l

## LoRa Technology

LoRa adopts a star network topology, which, compared to mesh networks, features lower latency and a simpler network structure. In real-world applications, IoT devices using the LoRa protocol can achieve wireless communication distances of over 15 kilometers (in suburban environments), with battery life extending beyond 10 years. LoRa can connect millions of wireless sensor nodes to a LoRa gateway—an advantage that traditional communication standards cannot match. A comparison of transmission distance, data rate, and power consumption among common wireless communication technologies is as follows:

| Technology | Max Transmission Distance | Max Data Rate | Min Receive Power Consumption |
| :--------: | :-----------------------: | :-----------: | :---------------------------: |
| Bluetooth | 15 m | 2 MB/s | 6 mA |
| WiFi | 100 m | 54 MB/s | 105 mA |
| ZigBee | 75 m | 250 KB/s | 2 mA |
| LoRa | 15 km | 600 KB/s | 3 mA |

## SX1278 Chip

The SX1278 transceiver uses LoRa modulation for ultra-long-range spread spectrum communication. It features strong anti-interference capabilities and minimizes power consumption. With Semtech’s patented LoRa modulation technology, the SX1278 achieves high sensitivity over -148 dBm using low-cost crystals and components.  
This design uses the Ra-02 module from AI-Thinker. For technical specifications, refer to: <https://docs.ai-thinker.com/lora>

## STM8L Microcontroller

This design uses the STM8L051F3 microcontroller. Technical documentation can be found at: <https://www.st.com/en/microcontrollers-microprocessors/stm8l051f3.html>. The STM8L051F3 supports five low-power modes: Wait mode, Low-power run mode, Low-power wait mode, Active-halt mode, and Halt mode.

1. **Wait mode**: The CPU clock stops, but selected modules continue to operate. Internal/external interrupts or a reset can wake the CPU from this mode.
2. **Low-power run mode**: The CPU and selected peripherals operate normally using a low-speed oscillator (LSI or LSE). This mode is entered and exited via software or reset.
3. **Low-power wait mode**: Similar to Low-power run mode but with the CPU clock stopped. The system can exit via reset, internal, or external events (such as timer, serial interface, DMA1, or I/O port).
4. **Active-halt mode**: All clocks except for the RTC are stopped. The system can be woken up by an RTC interrupt, external interrupt, or reset.
5. **Halt mode**: Both CPU and peripheral clocks stop, while the device remains powered. RAM contents are retained. Wake-up can occur via external interrupt or reset. Some peripherals can also wake the MCU from halt. Wake-up time is as fast as 5 μs.

The current consumption in each of the five modes is listed below:

| Mode | Current |
| :--: | :-----: |
| Wait mode | — |
| Low-power run mode | 5.1 μA |
| Low-power wait mode | 3 μA |
| Active-halt mode | 1.3 μA |
| Halt mode | 350 nA |

The minimum drive current of the STM8L051F3 I/O pins can reach as low as 5 μA, supporting its low-power design.
