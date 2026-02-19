#pragma once

namespace emu::cpu {

class interrupt_signal {
public:
    interrupt_signal(bool& interrupt_flag) : my_interrupt_flag(interrupt_flag)
    {}

    void trigger() const noexcept {
        my_interrupt_flag = true;
    }

private:
    bool& my_interrupt_flag;
};

} // namespace emu::cpu
