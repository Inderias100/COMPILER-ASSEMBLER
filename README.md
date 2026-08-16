# ⚙️ Two-Pass MIPS Assembler in C

A C program that translates a selected subset of MIPS assembly instructions into 32-bit hexadecimal machine code.

## ✨ Features

* Uses a **two-pass assembler design**

  * **Pass 1:** finds labels and stores their addresses in a symbol table.
  * **Pass 2:** encodes instructions into machine code.
* Starts the MIPS text segment at `0x00400000`.
* Supports forward and backward label references.
* Supports MIPS register names such as `$t0`, `$s0`, `$sp`, and `$ra`, as well as numeric registers such as `$8` and `$29`.
* Displays the memory address, generated machine code, and original instruction.

## ✅ Supported instructions

| Type   | Instructions                             |
| ------ | ---------------------------------------- |
| R-type | `add`, `and`, `or`, `xor`, `slt`         |
| I-type | `addi`, `andi`, `ori`, `lw`, `sw`, `beq` |
| J-type | `j`                                      |

The program also handles:

* Branch-offset calculation for `beq`
* Jump-target calculation for `j`
* Base-offset addressing such as `lw $t0, 8($sp)`
* Labels written on their own line or before an instruction

> Note: In this implementation, an `add` instruction with an immediate value is encoded as `addi`.

## 🛠️ How to run

Compile the program from the repository root:

```bash
gcc -std=c11 -o assembler "Assignment 2/Inderias.c"
```

Run it with an assembly input file:

```bash
./assembler "Assignment 2/in3.asm"
```

## 📌 Example output

```text
0x00400000 0x20080000         addi $t0, $zero, 0
0x00400004 0x2009000A         addi $t1, $zero, 10
0x00400008 0x11090002         beq $t0, $t1, end
0x0040000C 0x21080001         add $t0, $t0, 1
0x00400010 0x08100002         j loop
0x00400014 0xAE080000         sw $t0, 0($s0)
```

## 🧪 Test files

The repository includes six sample MIPS programs:

* `in1.asm` — arithmetic and logical instructions
* `in2.asm` — load/store instructions
* `in3.asm` — labels, loops, branches, and jumps
* `in4.asm` — conditional control flow
* `in5.asm` — different register types
* `in6.asm` — numeric registers and backward branches

The `Mars Snaps/` and `Outputs/` folders contain screenshots used to compare results with MARS.

## 📁 Project structure

```text
.
├── Assignment 2/
│   ├── Inderias.c
│   └── in1.asm ... in6.asm
├── Mars Snaps/
├── Outputs/
└── Report Compiler Assignment 2.pdf
```
