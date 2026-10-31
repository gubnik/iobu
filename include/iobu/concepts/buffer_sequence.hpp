#pragma once

#include <iobu/concepts/buffer_compatible.hpp>
#include <ranges>

namespace iobu
{

template <typename BufSeqTy>
concept write_buffer_sequence = write_buffer_compatible<BufSeqTy> or
                                (std::ranges::bidirectional_range<BufSeqTy> and
                                 write_buffer_compatible<std::ranges::range_value_t<BufSeqTy>>);

template <typename BufSeqTy>
concept read_buffer_sequence = read_buffer_compatible<BufSeqTy> or
                               (std::ranges::bidirectional_range<BufSeqTy> and
                                read_buffer_compatible<std::ranges::range_value_t<BufSeqTy>>);

} // namespace iobu
