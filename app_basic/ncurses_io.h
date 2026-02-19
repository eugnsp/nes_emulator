#pragma once

#include "emu/cpu/fwd.h"

namespace ncurses_io {

/** Initializes ncurses. */
void init() noexcept;

/** Finalizes ncurses. */
void finalize() noexcept;

/** Reads a character from standard input and stores it in the CPU's A register. */
void input(emu::cpu::vcpu_state&) noexcept;

/** Writes the value of the CPU's A register to standard output. */
void output(emu::cpu::vcpu_state const&) noexcept;

} // namespace ncurses_io
