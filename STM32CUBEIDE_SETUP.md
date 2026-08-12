# STM32CubeIDE Setup

## Target

The screenshot indicates an STM32F303VCTx-family MCU.

Suggested project name:

```text
STM32F3_MPU6050_USB_CDC_Monitor
```

## 1. Create project

1. Open STM32CubeIDE.
2. Select **File → New → STM32 Project**.
3. Search for the exact board or `STM32F303VCTx`.
4. Select C language and create the project.

## 2. Debug interface

In **System Core → SYS**:

```text
Debug: Serial Wire
```

## 3. I2C1

Enable:

```text
Connectivity → I2C1 → I2C
```

Recommended:

```text
I2C speed: 400 kHz
Addressing: 7-bit
Analog filter: Enabled
```

Use 100 kHz during initial troubleshooting if necessary.

## 4. USB CDC

Enable the target's USB peripheral in Device mode, then:

```text
Middleware and Software Packs
→ USB_DEVICE
→ Communication Device Class (Virtual Port Com)
```

Generated folders should include:

```text
USB_DEVICE/App
USB_DEVICE/Target
```

## 5. Clock

USB requires a valid 48 MHz clock. Resolve every warning on the Clock Configuration page. The exact PLL values depend on the board oscillator.

## 6. Generate code

Enable:

```text
Keep User Code when re-generating
```

Then generate the project.

## 7. Copy driver files

Copy:

```text
firmware/Drivers/MPU6050/mpu6050.h → Core/Inc/mpu6050.h
firmware/Drivers/MPU6050/mpu6050.c → Core/Src/mpu6050.c
firmware/Core/Inc/app_mpu6050.h → Core/Inc/app_mpu6050.h
firmware/Core/Src/app_mpu6050.c → Core/Src/app_mpu6050.c
```

## 8. Edit main.c

Use the marked code from:

```text
firmware/Core/Src/main_user_code_example.c
```

Essential calls:

```c
APP_MPU6050_Init(&hi2c1);
APP_MPU6050_Process();
```

## 9. Floating-point printf

Add this linker option:

```text
-u _printf_float
```

## 10. Build and flash

Build the project, connect the board, and press Run or Debug.

## 11. Terminal

Find the Virtual COM Port in Windows Device Manager and use:

```text
Baud: 9600
Data bits: 8
Parity: None
Stop bits: 1
Flow control: None
```
