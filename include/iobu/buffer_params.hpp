#pragma once

#include "iobu/concepts/buffer_compatible.hpp"
namespace iobu
{

constexpr struct
{
    template <write_buffer_compatible WrBufTy>
    constexpr auto operator()(WrBufTy &buf) const noexcept -> void *
    {
        return buf.data();
    }

    template <read_buffer_compatible RdBufTy>
    constexpr auto operator()(RdBufTy const &buf) const noexcept -> void *
    {
        return buf.data();
    }
} buffer_data;

constexpr struct
{
    template <write_buffer_compatible WrBufTy>
    constexpr auto operator()(WrBufTy &buf) const noexcept -> std::size_t
    {
        return buf.size();
    }

    template <read_buffer_compatible RdBufTy>
    constexpr auto operator()(RdBufTy const &buf) const noexcept -> std::size_t
    {
        return buf.size();
    }
} buffer_size;

} // namespace iobu
