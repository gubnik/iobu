#pragma once

#include "iobu/buffer_iterator.hpp"
#include "iobu/concepts/buffer_sequence.hpp"
#include "iobu/detail/read_buffer_iterator.hpp"
#include <utility>

namespace iobu
{

template <read_buffer_sequence RdBufSeqTy> struct buffers_view final
{
    using raw_iterator_type = decltype(read_buffers_begin(std::declval<RdBufSeqTy const &>()));
    using iterator_type = detail::read_buffer_iterator<raw_iterator_type>;

    buffers_view (RdBufSeqTy const &read_buffers) noexcept
        : m_begin{read_buffers_begin(read_buffers)}, m_end{read_buffers_end(read_buffers)}
    {
    }

    buffers_view(RdBufSeqTy const &&read_buffers) = delete;

    buffers_view (iterator_type begin, iterator_type end) noexcept : m_begin{begin}, m_end{end}
    {
    }

    buffers_view (raw_iterator_type begin, raw_iterator_type end) noexcept
        : m_begin{begin}, m_end{end}
    {
    }

    auto begin () const noexcept -> iterator_type
    {
        return m_begin;
    }

    auto end () const noexcept -> iterator_type
    {
        return m_begin;
    }

    auto cbegin () const noexcept -> iterator_type
    {
        return m_begin;
    }

    auto cend () const noexcept -> iterator_type
    {
        return m_begin;
    }

  private:
    iterator_type m_begin;
    iterator_type m_end;
};

} // namespace iobu
