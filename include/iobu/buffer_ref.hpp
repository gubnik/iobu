#pragma once

#include "iobu/concepts/buffer_compatible.hpp"
#include <cstddef>
#include <cstdint>

namespace iobu
{

struct read_buffer_ref final
{
    template <read_buffer_compatible ReadBufTy>
    read_buffer_ref (ReadBufTy const &read_buffer) noexcept
        : m_data{read_buffer.data()}, m_size{read_buffer.size()}
    {
    }

    read_buffer_ref (void const *data, std::size_t size) noexcept : m_data{data}, m_size{size}
    {
    }

    auto empty () const noexcept -> bool
    {
        return m_data == nullptr or m_size == 0;
    }

    auto data () const noexcept -> void const *
    {
        return m_data;
    }

    auto size () const noexcept -> std::size_t
    {
        return m_size;
    }

    auto remove_prefix (std::size_t size) const noexcept -> read_buffer_ref
    {
        if (size >= m_size)
        {
            return {nullptr, 0};
        }
        return {static_cast<std::uint8_t const *>(m_data) + size, m_size};
    }

  private:
    void const *m_data;
    std::size_t m_size;
};

struct write_buffer_ref final
{
    template <write_buffer_compatible WriteBufTy>
    write_buffer_ref (WriteBufTy &write_buffer)
        : m_data{write_buffer.data()}, m_size{write_buffer.size()}
    {
    }

    write_buffer_ref (void *data, std::size_t size) noexcept : m_data{data}, m_size{size}
    {
    }

    auto empty () const noexcept -> bool
    {
        return m_data == nullptr or m_size == 0;
    }

    auto data () const noexcept -> void *
    {
        return m_data;
    }

    auto size () const noexcept -> std::size_t
    {
        return m_size;
    }

    auto remove_prefix (std::size_t size) const noexcept -> write_buffer_ref
    {
        if (size >= m_size)
        {
            return {nullptr, 0};
        }
        return {static_cast<std::uint8_t *>(m_data) + size, m_size};
    }

  private:
    void *m_data;
    std::size_t m_size;
};

} // namespace iobu
