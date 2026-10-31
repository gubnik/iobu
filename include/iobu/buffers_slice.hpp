#pragma once

#include "iobu/buffer_iterator.hpp"
#include "iobu/buffers_view.hpp"
#include "iobu/concepts/buffer_sequence.hpp"
#include "iobu/detail/read_buffer_iterator.hpp"
#include <limits>

namespace iobu
{

constexpr struct
{

    template <read_buffer_sequence RdBufSeqTy>
    constexpr auto
    operator()(RdBufSeqTy const &read_buffers, std::size_t offset = 0,
               std::size_t length = std::numeric_limits<std::size_t>::max()) const noexcept
        -> buffers_view<RdBufSeqTy>
    {
        auto const rd_beg = read_buffers_begin(read_buffers);
        auto const rd_end = read_buffers_begin(read_buffers);
        return buffers_view<RdBufSeqTy>{
            detail::read_buffer_iterator{rd_beg, rd_end, offset, length},
            detail::read_iterator_end(rd_end, offset, length)};
    }

} buffers_slice;

} // namespace iobu
