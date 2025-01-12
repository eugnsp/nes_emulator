#pragma once

#include "emu/types.h"

namespace emu {

enum class address_size { byte, word };

/** Address page index. */
enum class address_page_index : uint8_t {};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<address_size Size> class raw_address;
template<address_page_index Page> class fixed_page_address;

/** Abstract 16-bit address. */
using abstract_address = raw_address<address_size::word>;

/** Zero page 8-bit address. */
using zp_address = fixed_page_address<address_page_index{0}>;

/** Stack page 8-bit address. */
using sp_address = fixed_page_address<address_page_index{1}>;

} // namespace emu
