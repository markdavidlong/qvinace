/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * vinace
 * Copyright (C) P.Y. Rollo 2009 <dev@pyrollo.com>
 *
 * vinace is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * vinace is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _C_MEMORY_HPP_
#define _C_MEMORY_HPP_

#include "types.h"

#include <QFile>

//
// CMemory : Abstract memory class
//

class CMemory
{
public:
    virtual ~CMemory() { }
    virtual uint8_t read(uint16_t) = 0;
    virtual void write(uint16_t, uint8_t) {};
};

//
// CDummyMemory : A dummy memory that does not remember anything
//

class CDummyMemory : public CMemory
{
public:
    uint8_t read(uint16_t) { return dummy_byte(); };
    void write(uint16_t, uint8_t) {};
};

//
// CMemoryProxy : A subpart of a memory with addresses translation
//

class CMemoryProxy: public CMemory
{
public:
    CMemoryProxy(CMemoryProxy *proxy, uint16_t start);
    CMemoryProxy(CMemory *memory, uint16_t start);

    uint8_t read(uint16_t addr) { return memory->read(addr + start); }
    void write(uint16_t addr, uint8_t byte) { memory->write(addr + start, byte); }

    // These methods are used when pipelining several proxies
    int get_start();
    CMemory *get_memory();

protected:
    int start;
    CMemory *memory;
};

//
// CRamMemory : Read / Write standard memory
//

class CRamMemory: public CMemory
{
public:
    CRamMemory(int size);
    uint8_t read(uint16_t addr) { return (addr < size)?buffer[addr]:dummy_byte(); };
    void write(uint16_t addr, uint8_t byte) { if (addr < size) buffer[addr] = byte; };
    uint8_t *get_buffer() { return buffer; }
    int get_size() { return size; }

protected:
    int size;
    uint8_t *buffer;
};

//
// CRomMemory : Read only memory
//

class CRomMemory: public CRamMemory
{
public:
    CRomMemory(int size):CRamMemory(size) { };
    void write(uint16_t addr, uint8_t byte) { Q_UNUSED(addr); Q_UNUSED(byte);  };

    void loadFromFile(const char* filename, int file_offset) {
        QFile file(filename);


        if (file.open(QIODevice::ReadOnly))
        {
            file.seek(file_offset);
            file.read((char*)this->get_buffer(), this->get_size());
            file.close();
        }
        else
        {
            qWarning("Unable to load ROM file \"%s\".",filename);
        }
    }

};

#endif // _C_MEMORY_HPP_
