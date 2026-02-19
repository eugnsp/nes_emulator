#pragma once

#include "emu/cpu/addressing_mode_tags.h"
#include "emu/cpu/effective_address.h"
#include "emu/cpu/operand.h"
#include "emu/cpu/vcpu.h"

namespace emu::cpu {

template<class Bus, addressing_mode_tag AddrMode, bool AddCycleOnPageCross = false>
class operand_access {
public:
    operand_access(vcpu<Bus>& cpu, typename AddrMode::operand op) :
        my_eff_address{effective_address<AddrMode, AddCycleOnPageCross>::get(cpu, op)},
        my_mem(cpu.system_bus())
    {}

    abstract_address get() const noexcept {
        return my_eff_address;
    }

    std::uint8_t load() const {
        /*if (!my_value.has_value())
            my_value = my_mem.load(my_eff_address);
        return *my_value;*/
        return my_mem.load(my_eff_address);
    }

    void store(std::uint8_t value) const {
        my_mem.store(my_eff_address, std::uint8_t{value});
    }

private:
    abstract_address my_eff_address;
    //mutable std::optional<std::uint8_t> my_value;
    Bus& my_mem;
};

template<class Bus>
class operand_access<Bus, addressing_mode_tags::imm> {
public:
    operand_access(vcpu<Bus> const&, byte_operand op) noexcept :
        my_imm_value{*op}
    {}

    std::uint8_t load() const noexcept {
        return my_imm_value;
    }

private:
    std::uint8_t my_imm_value;
};

template<class Bus>
class operand_access<Bus, addressing_mode_tags::acc> {
public:
    operand_access(vcpu<Bus>& cpu, null_operand) noexcept :
        my_cpu(cpu)
    {}

    std::uint8_t load() const noexcept {
        return my_cpu.a();
    }

    void store(std::uint8_t value) const noexcept {
        my_cpu.set_a(value);
    }

public:
    vcpu<Bus>& my_cpu;
};

template<class Bus>
class operand_access<Bus, addressing_mode_tags::imp> {
public:
    operand_access(vcpu<Bus> const&, null_operand) noexcept
    {}
};

template<class Bus>
class operand_access<Bus, addressing_mode_tags::rel> {
public:
    operand_access(vcpu<Bus> const&, byte_operand op) noexcept :
        my_rel_address{static_cast<std::int8_t>(*op)}
    {}

    std::int8_t get() const noexcept {
        return my_rel_address;
    }

private:
    std::int8_t my_rel_address;
};

/*
class operand_access_get {
public:
    operand_access_get() {
        my_value = op_access.load();
    }

    int operator*() const {
        return my_value;
    }

private:
    int my_value;
};

class operand_access_set {
public:
    ~operand_access_set() {
        op_access.store(my_value);
    }

    int& operator*() const {
        return my_value;
    }

private:
    int my_value;
};

class operand_access_get_set {
public:
    operand_access_get_set() {
        my_value = op_access.load();
    }

    ~operand_access_get_set() {
        op_access.store(my_value);
    }
};
*/

} // namespace emu::cpu
