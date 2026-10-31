#pragma once

#include "iobu/concepts/buffer_compatible.hpp"
#include "iobu/concepts/buffer_sequence.hpp"
#include <cstddef>
#include <cstdint>

namespace iobu
{

struct read_buffer_ref final
{
    template <read_buffer_compatible ReadBufTy>
    constexpr read_buffer_ref (ReadBufTy const &read_buffer) noexcept
        : m_data{read_buffer.data()}, m_size{read_buffer.size()}
    {
    }
    constexpr read_buffer_ref (void const *data, std::size_t size) noexcept
        : m_data{data}, m_size{size}
    {
    }

    constexpr read_buffer_ref () noexcept : m_data{nullptr}, m_size{0}
    {
    }

    constexpr auto empty () const noexcept -> bool
    {
        return m_data == nullptr or m_size == 0;
    }

    constexpr auto data () const noexcept -> void const *
    {
        return m_data;
    }

    constexpr auto size () const noexcept -> std::size_t
    {
        return m_size;
    }

    constexpr auto remove_prefix (std::size_t size) const noexcept -> read_buffer_ref
    {
        if (size > m_size)
        {
            return {m_data, 0};
        }
        return {static_cast<std::uint8_t const *>(m_data) + size, m_size - size};
    }

    constexpr auto remove_suffix (std::size_t size) const noexcept -> read_buffer_ref
    {

        if (size >= m_size)
        {
            return {m_data, 0};
        }
        return {m_data, m_size - size};
    }

  private:
    void const *m_data;
    std::size_t m_size;
};

struct write_buffer_ref final
{
    template <write_buffer_compatible WriteBufTy>
    constexpr write_buffer_ref (WriteBufTy &write_buffer)
        : m_data{write_buffer.data()}, m_size{write_buffer.size()}
    {
    }

    constexpr write_buffer_ref (void *data, std::size_t size) noexcept : m_data{data}, m_size{size}
    {
    }

    constexpr write_buffer_ref () noexcept : m_data{nullptr}, m_size{0}
    {
    }

    constexpr auto empty () const noexcept -> bool
    {
        return m_data == nullptr or m_size == 0;
    }

    constexpr auto data () const noexcept -> void *
    {
        return m_data;
    }

    constexpr auto size () const noexcept -> std::size_t
    {
        return m_size;
    }

    constexpr auto remove_prefix (std::size_t size) const noexcept -> write_buffer_ref
    {
        if (size > m_size)
        {
            return {m_data, 0};
        }
        return {static_cast<std::uint8_t *>(m_data) + size, m_size - size};
    }

    constexpr auto remove_suffix (std::size_t size) const noexcept -> write_buffer_ref
    {

        if (size >= m_size)
        {
            return {m_data, 0};
        }
        return {m_data, m_size - size};
    }

  private:
    void *m_data;
    std::size_t m_size;
};

static_assert(read_buffer_sequence<read_buffer_ref>);
static_assert(write_buffer_sequence<write_buffer_ref>);

} // namespace iobu
