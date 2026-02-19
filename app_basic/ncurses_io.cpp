#include "ncurses_io.h"

#include "emu/cpu/vcpu.h"

#include <curses.h>

#include <cstdint>

namespace ncurses_io {

namespace {

constexpr std::uint8_t char_lf = 0x0A;  // Line feed
constexpr std::uint8_t char_cr = 0x0D;  // Carriage return
constexpr std::uint8_t char_bs = 0x7F;  // Backspace

std::uint8_t get_char() noexcept {
    std::uint8_t const ch = static_cast<std::uint8_t>(::getch());
    return (ch != char_lf) ? ch : char_cr;
}

void print_char(std::uint8_t ch) noexcept {
    if (ch != char_cr)
        ::addch(static_cast<::chtype>(ch));
}

void delete_char() noexcept {
    int const cur_x = ::getcurx(::stdscr);
    if (cur_x == 0)
        return;

    int const cur_y = ::getcury(::stdscr);

    ::move(cur_y, cur_x - 1);
    ::delch();
    ::refresh();
}

} // anonymous namespace

/** Initialize ncurses. */
void init() noexcept {
    ::initscr();
    ::cbreak();

    ::scrollok(::stdscr, true);
    ::noecho();
}

/** Finalizes ncurses. */
void finalize() noexcept {
    ::endwin();
}

/** Reads a character from standard input and stores it in the CPU's A register. */
void input(emu::cpu::vcpu_state& cpu) noexcept {
    std::uint8_t const ch = get_char();
    cpu.set_a(ch);

    if (ch != char_bs)
        print_char(ch);
    else
        delete_char();
}

/** Writes the value of the CPU's A register to standard output. */
void output(emu::cpu::vcpu_state const& cpu) noexcept {
    std::uint8_t const ch = cpu.a();
    print_char(ch);
}

} // namespace ncurses_io
