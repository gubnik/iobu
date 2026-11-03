#pragma once

#include "iobu/buffer_ref.hpp"
#include "iobu/concepts/buffer_sequence.hpp"
#include <type_traits>

namespace iobu
{

template <typename BufSeqTy>
using buffer_ref_type = std::conditional_t<[] () -> bool
                                           {
                                               static_assert(read_buffer_sequence<BufSeqTy>);
                                               return write_buffer_sequence<BufSeqTy>;
                                           }(),
                                           write_buffer_ref, read_buffer_ref>;
} // namespace iobu
