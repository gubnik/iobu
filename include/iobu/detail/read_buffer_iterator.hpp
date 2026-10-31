#pragma once

#include "iobu/buffer_ref.hpp"
#include <cstdint>
#include <iterator>
#include <limits>

namespace iobu::detail
{

template <typename Ty>
concept read_buffer_source_iterator =
    std::bidirectional_iterator<Ty> &&
    iobu::read_buffer_compatible<std::remove_cvref_t<std::iter_reference_t<Ty>>>;

constexpr struct
{
} sentinel_iterator;

template <read_buffer_source_iterator IterTy> class read_buffer_iterator
{

  public:
    using value_type = read_buffer_ref;
    using difference_type = std::iter_difference_t<IterTy>;
    using reference = read_buffer_ref;
    using pointer = void;
    using iterator_concept = std::bidirectional_iterator_tag;
    using iterator_category = std::bidirectional_iterator_tag;

    read_buffer_iterator() noexcept = default;

    read_buffer_iterator (IterTy begin) noexcept
        : m_current{begin}, m_offset{0}, m_limit{std::numeric_limits<std::size_t>::max()}
    {
    }

    read_buffer_iterator (IterTy begin, IterTy end, std::size_t offset) noexcept
        : m_current{begin}, m_offset{offset}, m_limit{std::numeric_limits<std::size_t>::max()}
    {
        while (m_current != end && (m_bytes_seen + std::size_t((*m_current).size())) <= m_offset)
        {
            m_bytes_seen += std::size_t((*m_current).size());
            ++m_current;
        }
    }

    read_buffer_iterator (IterTy begin, IterTy end, std::size_t offset, std::size_t length) noexcept
        : m_current{begin}, m_offset{offset}, m_limit{offset + length}
    {
        while (m_current != end && (m_bytes_seen + std::size_t((*m_current).size())) <= m_offset)
        {
            m_bytes_seen += std::size_t((*m_current).size());
            ++m_current;
        }
    }

    auto operator*() const noexcept -> read_buffer_ref
    {
        auto const &buf = *m_current;
        std::size_t slice_start = (m_bytes_seen < m_offset) ? (m_offset - m_bytes_seen) : 0;
        std::size_t slice_end = std::min(buf.size(), m_limit - m_bytes_seen);
        if (slice_start >= slice_end)
        {
            return {nullptr, 0};
        }
        return {static_cast<std::uint8_t const *>(buf.data()) + slice_start,
                slice_end - slice_start};
    }

    read_buffer_iterator &operator++() noexcept
    {
        m_bytes_seen += std::size_t((*m_current).size());
        ++m_current;
        return *this;
    }

    read_buffer_iterator operator++(int) noexcept
    {
        auto old = *this;
        ++*this;
        return old;
    }

    read_buffer_iterator &operator--() noexcept
    {
        --m_current;
        m_bytes_seen -= std::size_t((*m_current).size());
        return *this;
    }

    read_buffer_iterator operator--(int) noexcept
    {
        auto old = *this;
        --*this;
        return old;
    }

    friend bool operator==(read_buffer_iterator const &a, read_buffer_iterator const &b) noexcept
    {
        return a.m_current == b.m_current && a.m_bytes_seen == b.m_bytes_seen;
    }

    read_buffer_iterator (decltype(sentinel_iterator) const &, IterTy end, std::size_t offset,
                          std::size_t length) noexcept
        : m_current{end}, m_offset{offset}, m_limit{offset + length}, m_bytes_seen{m_limit}
    {
    }

    IterTy m_current;
    std::size_t m_offset;
    std::size_t m_limit;
    std::size_t m_bytes_seen{0};
};

template <read_buffer_source_iterator IterTy>
constexpr auto read_iterator_end (IterTy end, std::size_t offset, std::size_t length)
    -> read_buffer_iterator<IterTy>
{
    return {sentinel_iterator, end, offset, length};
}

} // namespace iobu::detail
