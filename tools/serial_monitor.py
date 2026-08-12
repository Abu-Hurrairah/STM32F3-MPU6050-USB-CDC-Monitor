#!/usr/bin/env python3
from __future__ import annotations

import argparse
import sys
import serial
from serial import SerialException

def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Read STM32F3 MPU6050 data from a serial or USB CDC port."
    )
    parser.add_argument("--port", required=True, help="Serial port, e.g. COM4")
    parser.add_argument("--baud", type=int, default=9600)
    parser.add_argument("--timeout", type=float, default=1.0)
    return parser

def main() -> int:
    args = build_parser().parse_args()
    try:
        with serial.Serial(
            port=args.port,
            baudrate=args.baud,
            timeout=args.timeout,
        ) as connection:
            print(f"Connected to {args.port}. Press Ctrl+C to stop.")
            while True:
                line = connection.readline()
                if line:
                    print(line.decode("utf-8", errors="replace").rstrip())
    except KeyboardInterrupt:
        print("\nStopped.")
        return 0
    except SerialException as exc:
        print(f"Serial error: {exc}", file=sys.stderr)
        return 1

if __name__ == "__main__":
    raise SystemExit(main())
