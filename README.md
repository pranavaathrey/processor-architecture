# Custom 8-bit Processor

## Overview

This repository contains the design and implementation of a custom 8-bit processor built in **Logisim Evolution**. The project demonstrates the inner workings of CPU architecture by implementing a minimal yet functional Harvard-style processor. It was developed as part of a Microprocessors and Microcontrollers course project.

## Features

* **8-bit Harvard architecture** with separate ROM and RAM modules.
* **32-bit instruction word** (1 byte opcode + 2 operand bytes + 1 destination/jump byte).
* **RISC-style ISA** with arithmetic, logical, memory, and control instructions.
* **Single-cycle execution model** – each instruction completes in one clock cycle.
* **16 I/O pins** (8 input, 8 output) for basic interfacing.
* **Six writable registers**, including a dedicated RAM address register.
* **C-based assembler** for converting assembly programs into binary machine code.

## Other Details

* **ROM (Program Memory):** 1024 bytes, addressed via 8-bit Program Counter (256 instructions × 4 bytes).
* **RAM (Data Memory):** 256 bytes, accessed through the RAM Address Register.
* **ALU:** Supports addition, subtraction, multiplication, division, and bitwise operations.
* **Control Unit:** Handles instruction decoding, branching, and immediate-mode evaluation.
* **Conditional Unit:** Supports relational and equality-based branching.

## Tools Used

* **Logisim Evolution** – for hardware design and simulation.
* **C Programming Language** – for developing the assembler.

## Testing and Demonstrations

Sample assembly programs were used for verification:

1. **Bubble Sort Algorithm** – Tests memory operations and conditional branching.
2. **Maximum Finder** – Demonstrates I/O usage and comparison operations.

There are more such programs (and assembled binaries) that can be found in the `binaries` directory.

Here is a video demonstration of compiling and running the two mentioned programs:
<p>
    <a href="https://youtu.be/PkqjSfT5Vvo" target="_blank">
        <img src="images/thumbnail.png" alt="video thumbnail" width="640" height="360" border="2" />
    </a>
</p>