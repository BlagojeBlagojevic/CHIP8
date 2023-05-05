# CHIP8
# About Chip-8  



Chip-8 is a simple, interpreted, programming language which was first used on some do-it-yourself computer systems in the late 1970s and early 1980s. The COSMAC VIP, DREAM 6800, and ETI 660 computers are a few examples. These computers typically were designed to use a television as a display, had between 1 and 4K of RAM, and used a 16-key hexadecimal keypad for input. The interpreter took up only 512 bytes of memory, and programs, which were entered into the computer in hexadecimal, were even smaller.


# Chip-8 Specifications 

# 16 8-bit Registers

The CHIP-8 has sixteen 8-bit registers, labeled V0 to VF. Each register is able to hold any value from 0x00 to 0xFF.


# 4K Bytes of Memory

The address space is divided into three sections:

# 1. 0x000-0x1FF: 

Originally reserved for the CHIP-8 interpreter, but in our modern emulator we will just never write to or read from that area.

# 2. 0x050-0x0A0: 

Storage space for the 16 built-in characters (0 through F), which we will need to manually put into our memory because ROMs will be looking for those characters.

# 3. 0x200-0xFFF: 

Instructions from the ROM will be stored starting at 0x200, and anything left after the ROM’s space is free to use.




#  16-bit Index Register
The Index Register is a special register used to store memory addresses for use in operations. It’s a 16-bit register because the maximum memory address (0xFFF) is too big for an 8-bit register

#  Other  Registers

 16-bit Program Counter

  16-level Stack

  8-bit Stack Pointer

 8-bit Delay Timer

 8-bit Sound Timer

# The Instructions

The CHIP-8 has 34 instructions:

            00E0 - CLS
            00EE - RET
            0nnn - SYS addr
            1nnn - JP addr
            2nnn - CALL addr
            3xkk - SE Vx, byte
            4xkk - SNE Vx, byte
            5xy0 - SE Vx, Vy
            6xkk - LD Vx, byte
            7xkk - ADD Vx, byte
            8xy0 - LD Vx, Vy
            8xy1 - OR Vx, Vy
            8xy2 - AND Vx, Vy
            8xy3 - XOR Vx, Vy
            8xy4 - ADD Vx, Vy
            8xy5 - SUB Vx, Vy
            8xy6 - SHR Vx {, Vy}
            8xy7 - SUBN Vx, Vy
            8xyE - SHL Vx {, Vy}
            9xy0 - SNE Vx, Vy
            Annn - LD I, addr
            Bnnn - JP V0, addr
            Cxkk - RND Vx, byte
            Dxyn - DRW Vx, Vy, nibble
            Ex9E - SKP Vx
            ExA1 - SKNP Vx
            Fx07 - LD Vx, DT
            Fx0A - LD Vx, K
            Fx15 - LD DT, Vx
            Fx18 - LD ST, Vx
            Fx1E - ADD I, Vx
            Fx29 - LD F, Vx
            Fx33 - LD B, Vx
            Fx55 - LD [I], Vx
            Fx65 - LD Vx, 



