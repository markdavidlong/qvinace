/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Vinace
 * Copyright (C) P.Y. Rollo 2009 <dev@pyrollo.com>
 * 
 * Vinace is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Vinace is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _C_PROCESSOR6502_HPP_
#define _C_PROCESSOR6502_HPP_

#include "types.h"
#include "c-processor.hpp"
#include "c-memory.hpp"

// Signals
#define NUMBER_OF_SIGNALS 4
#define SIGNAL_RESET 0 // Reset signal
#define SIGNAL_NMI   1 // Non masquable interrupt signal
#define SIGNAL_IRQ   2 // Interrupt request signal
#define SIGNAL_BRK   3 // Break signal

// State register bits
#define N_BIT	0X80
#define V_BIT	0X40
#define B_BIT	0x10
#define D_BIT	0x08
#define I_BIT	0x04
#define Z_BIT	0x02
#define C_BIT	0x01

class CProcessor6502: public CProcessor 
{
public:
	CProcessor6502(CMemory *memory);
	void cycle(); // Play one machine cycle (may do nothing)

	// Direct access to registers
    uint16_t get_PC() { return PC; };
    uint8_t get_A()  { return A;  };
    uint8_t get_X()  { return X;  };
    uint8_t get_Y()  { return Y;  };
    uint8_t get_P()  { return P;  };
    uint8_t get_S()  { return S;  };

	// Access to last executed instruction address
    uint16_t get_last_PC() { return last_PC; };


protected:

	// Registers
    uint16_t PC;
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t P;
    uint8_t S;
	
	// Last processed instruction address
    uint16_t last_PC;
	
	// Mandatory CProcessor virtual methods
	virtual void reset(); // Reset the processor
	virtual void process_instruction(); // Execute the next opcode
	virtual bool process_signals(); // Process signals, returns true if no signal and false if there was one
	

	// Memory methods
    void write_byte(uint16_t addr, uint8_t byte);
    uint8_t read_byte(uint16_t addr);
    uint16_t read_word(uint16_t addr);
    uint8_t next_byte();
    uint16_t next_word();
	
	// Stack
    void push(uint8_t byte);
    uint8_t pull();
    void push_word(uint16_t word);
    uint16_t pull_word();
	
	// Status register
	void set_p(short bit, bool state);
    int get_p(short bit);

	// Pseudo decimal mode
    uint16_t bcdadjustadd(uint16_t result);
    uint16_t bcdadjustsub(uint16_t result);

	// Addresses methods
    void check_page_crossing(uint16_t addr, int offset);

    uint16_t eazp(uint8_t offset=0);
    uint16_t eaabs(uint8_t offset=0, bool extracycle = true);
    uint16_t eazpxind();
    uint16_t eazpindy(bool extracycle = true);

    // Factorized code
    void branch(uint8_t operand);
    void testresult(uint16_t result);
    void testvalue(uint8_t value);
    void opadd(uint8_t operand);
    void opsub(uint8_t operand);
    void oprol(uint16_t addr, bool rotate);
    void opror(uint16_t addr, bool rotate);
    void opcmp(uint8_t operand1, uint8_t operand2);
    void opdec(uint16_t addr);
    void opinc(uint16_t addr);

private:

	// Some variables put here for optimisation purpose (avoid memory reservation once proc started)
    uint8_t opcode;
    uint8_t operand;
    uint8_t byte;
    uint16_t word;
    uint16_t addr;

};


#endif // _C_PROCESSOR6502_HPP_
