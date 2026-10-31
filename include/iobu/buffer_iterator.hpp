#pragma once

#include "iobu/concepts/buffer_compatible.hpp"
#include "iobu/concepts/buffer_sequence.hpp"
#include <iterator>
#include <memory>
#include <ranges>

namespace iobu
{

constexpr struct
{
    template <write_buffer_sequence BufSeqTy>
        requires(write_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy &buffer) const noexcept
    {
        return std::addressof(buffer);
    }

    template <write_buffer_sequence BufSeqTy>
        requires(not write_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy &buffers) const noexcept
    {
        return std::ranges::begin(buffers);
    }
} write_buffers_begin;

constexpr struct
{
    template <write_buffer_sequence BufSeqTy>
        requires(write_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy &buffer) const noexcept
    {
        return std::addressof(buffer) + 1;
    }

    template <write_buffer_sequence BufSeqTy>
        requires(not write_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy &buffers) const noexcept
    {
        return std::ranges::end(buffers);
    }
} write_buffers_end;

constexpr struct
{
    template <read_buffer_sequence BufSeqTy>
        requires(read_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy const &buffer) const noexcept
    {
        return std::addressof(buffer);
    }

    template <read_buffer_sequence BufSeqTy>
        requires(not read_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy const &buffers) const noexcept
    {
        return std::ranges::begin(buffers);
    }
} read_buffers_begin;

constexpr struct
{
    template <read_buffer_sequence BufSeqTy>
        requires(read_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy const &buffer) const noexcept
    {
        return std::addressof(buffer) + 1;
    }

    template <read_buffer_sequence BufSeqTy>
        requires(not read_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy const &buffers) const noexcept
    {
        return std::ranges::end(buffers);
    }
} read_buffers_end;

} // namespace iobu
