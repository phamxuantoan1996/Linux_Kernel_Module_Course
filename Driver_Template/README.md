# Driver Template
A driver template is not an official subsytem. It's a code design pattern (or framwork) used to write a specific type of driver. So, there are several common templates you can start from when writing a Linux driver - each one targets a specific subsytem or I/O type

| Template Name                    | Description                                                | Registers As             | Example Device Node                 |
| -------------------------------- | ---------------------------------------------------------- | ------------------------ | ----------------------------------- |
| **Character Device Template**    | Byte-stream device, accessed via read/write                | `/dev/<name>`            | `/dev/ttyS0`, `/dev/null`           |
| **Misc Device Template**         | Simplified wrapper around char device                      | `/dev/<name>`            | `/dev/random`, `/dev/loop-control`  |
| **Block Device Template**        | Devices that handle fixed-size data blocks                 | `/dev/<name>`            | `/dev/sda`, `/dev/mmcblk0`          |
| **Network Device Template**      | For network interfaces (packets, not bytes)                | `ifconfig` / `ip link`   | `eth0`, `wlan0`                     |
| **Platform Device Template**     | For SoC (on-chip) devices described in Device Tree or ACPI | Kernel internal          | Often combined with char or misc    |
| **PCI Driver Template**          | For PCI/PCIe hardware                                      | Kernel internal          | May register as char, block, or net |
| **USB Driver Template**          | For USB devices (host or gadget)                           | Kernel internal          | `/dev/ttyUSB0`, `/dev/video0`       |
| **I2C Driver Template**          | For I2C-attached chips (sensors, EEPROM, etc.)             | Kernel internal          | Often char device                   |
| **SPI Driver Template**          | For SPI-attached chips                                     | Kernel internal          | Often char device                   |
| **Input Driver Template**        | For keyboard, mouse, touch, etc.                           | `/dev/input/*`           | `/dev/input/event0`                 |
| **Sound Driver Template**        | For audio devices                                          | `/dev/snd/*`             | `/dev/snd/pcmC0D0p`                 |
| **Framebuffer / DRM Template**   | For display and GPU devices                                | `/dev/fb0`, `/dev/dri/*` | `/dev/fb0`                          |
| **TTY / Serial Driver Template** | For serial ports                                           | `/dev/tty*`              | `/dev/ttyS0`, `/dev/ttyUSB0`        |
