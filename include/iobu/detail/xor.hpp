#pragma once

namespace iobu::detail
{

constexpr auto xor_fn (bool lhs, bool rhs) noexcept -> bool
{
    return (lhs and not rhs) or (not lhs and rhs);
}

} // namespace iobu::detail
