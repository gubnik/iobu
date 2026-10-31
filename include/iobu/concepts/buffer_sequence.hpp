#pragma once

#include "iobu/detail/xor.hpp"
#include <iobu/concepts/buffer_compatible.hpp>
#include <ranges>

namespace iobu
{

template <typename BufSeqTy>
concept write_buffer_sequence =
    detail::xor_fn(write_buffer_compatible<BufSeqTy>,
                   (std::ranges::bidirectional_range<BufSeqTy> and
                    write_buffer_compatible<std::ranges::range_value_t<BufSeqTy>>));

template <typename BufSeqTy>
concept read_buffer_sequence =
    detail::xor_fn(read_buffer_compatible<BufSeqTy>,
                   (std::ranges::bidirectional_range<BufSeqTy> and
                    read_buffer_compatible<std::ranges::range_value_t<BufSeqTy>>));

} // namespace iobu
