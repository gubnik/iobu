#pragma once

#include "iobu/buffer_iterator.hpp"
#include "iobu/buffer_ref.hpp"
#include "iobu/concepts/buffer_sequence.hpp"

namespace iobu
{

constexpr struct
{
    template <write_buffer_sequence WrBufSeqTy>
    constexpr auto operator()(WrBufSeqTy &write_buffers) const noexcept -> write_buffer_ref
    {
        return write_buffer_ref{*write_buffers_begin(write_buffers)};
    }

    template <read_buffer_sequence RdBufSeqTy>
        requires(not write_buffer_sequence<RdBufSeqTy>)
    constexpr auto operator()(RdBufSeqTy const &read_buffers) const noexcept -> read_buffer_ref
    {
        return read_buffer_ref{*read_buffers_begin(read_buffers)};
    }

    template <read_buffer_sequence RdBufSeqTy>
        requires(not write_buffer_sequence<RdBufSeqTy>)
    constexpr auto operator()(RdBufSeqTy const &&read_buffers) const noexcept
        -> read_buffer_ref = delete;

} buffers_front;

} // namespace iobu
