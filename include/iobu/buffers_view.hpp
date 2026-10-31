#pragma once

#include "iobu/buffer_iterator.hpp"
#include "iobu/concepts/buffer_sequence.hpp"

namespace iobu
{

template <read_buffer_sequence BufSeqTy> struct buffers_view final
{
    using iterator_type = decltype(read_buffers_begin(std::declval<BufSeqTy>()));

    buffers_view (BufSeqTy const &read_buffers)
        : m_begin{read_buffers_begin(read_buffers)}, m_end{read_buffers_end(read_buffers)}
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
