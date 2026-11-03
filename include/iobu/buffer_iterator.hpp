#pragma once

#include "iobu/concepts/buffer_compatible.hpp"
#include "iobu/concepts/buffer_sequence.hpp"
#include <iterator>
#include <memory>
#include <ranges>
#include <type_traits>

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
        requires(read_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy const &&buffer) const noexcept = delete;

    template <read_buffer_sequence BufSeqTy>
        requires(not read_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy const &buffers) const noexcept
    {
        return std::ranges::begin(buffers);
    }

    template <read_buffer_sequence BufSeqTy>
        requires(not read_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy const &&buffers) const noexcept = delete;

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
        requires(read_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy const &&buffer) const noexcept = delete;

    template <read_buffer_sequence BufSeqTy>
        requires(not read_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy const &buffers) const noexcept
    {
        return std::ranges::end(buffers);
    }

    template <read_buffer_sequence BufSeqTy>
        requires(not read_buffer_compatible<BufSeqTy>)
    constexpr auto operator()(BufSeqTy const &&buffers) const noexcept = delete;

} read_buffers_end;

template <read_buffer_sequence BufSeqTy>
using buffer_sequence_ref =
    std::conditional_t<write_buffer_sequence<BufSeqTy>, BufSeqTy &, BufSeqTy const &>;

template <read_buffer_sequence BufSeqTy>
    requires(not write_buffer_sequence<BufSeqTy>)
constexpr auto buffer_sequence_begin (BufSeqTy const &buffers) noexcept
{
    return read_buffers_begin(buffers);
}

template <read_buffer_sequence BufSeqTy>
    requires(write_buffer_sequence<BufSeqTy>)
constexpr auto buffer_sequence_begin (BufSeqTy &buffers) noexcept
{
    return write_buffers_begin(buffers);
}

template <read_buffer_sequence BufSeqTy>
    requires(not write_buffer_sequence<BufSeqTy>)
constexpr auto buffer_sequence_end (BufSeqTy const &buffers) noexcept
{
    return read_buffers_end(buffers);
}

template <read_buffer_sequence BufSeqTy>
    requires(write_buffer_sequence<BufSeqTy>)
constexpr auto buffer_sequence_end (BufSeqTy &buffers) noexcept
{
    return write_buffers_end(buffers);
}

} // namespace iobu
