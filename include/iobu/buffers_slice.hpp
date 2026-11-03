#pragma once

#include "iobu/buffer_iterator.hpp"
#include "iobu/buffer_type.hpp"
#include "iobu/concepts/buffer_sequence.hpp"

#include <cstddef>
#include <iterator>
#include <limits>
#include <type_traits>

namespace iobu
{

/**
 * @brief A borrowed view over a byte sub-range of a buffer sequence.
 *
 * Credit: Steve Gerbino, the code is adapted from Boost.Capy's detail::slice_of type
 * https://github.com/cppalliance/capy/blob/develop/include/boost/capy/detail/slice_of.hpp
 */
template <read_buffer_sequence BufSeqTy> struct buffers_slice
{
    using buffer_type = iobu::buffer_ref_type<BufSeqTy>;
    using iterator_type = decltype(iobu::buffer_sequence_begin<BufSeqTy>(
        std::declval<buffer_sequence_ref<BufSeqTy>>()));

  private:
    static buffer_type adjust (buffer_type const &b, std::size_t front_n,
                               std::size_t back_n) noexcept
    {
        if constexpr (std::is_same_v<buffer_type, write_buffer_ref>)
            return write_buffer_ref(static_cast<char *>(b.data()) + front_n,
                                    b.size() - front_n - back_n);
        else
            return read_buffer_ref(static_cast<char const *>(b.data()) + front_n,
                                   b.size() - front_n - back_n);
    }

  public:
    struct const_iterator
    {
        iterator_type cur{};
        iterator_type anchor_first{};
        iterator_type anchor_last{};
        std::size_t skip_front = 0;
        std::size_t skip_back = 0;

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = buffer_type;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type;

        const_iterator() noexcept = default;

        const_iterator (iterator_type cur, iterator_type anchor_first, iterator_type anchor_last,
                        std::size_t front_skip, std::size_t back_skip) noexcept
            : cur(cur), anchor_first(anchor_first), anchor_last(anchor_last),
              skip_front(front_skip), skip_back(back_skip)
        {
        }

        bool operator==(const_iterator const &o) const noexcept
        {
            return cur == o.cur;
        }

        bool operator!=(const_iterator const &o) const noexcept
        {
            return !(*this == o);
        }

        value_type operator*() const noexcept
        {
            buffer_type buf{*cur};
            auto const front_n = (cur == anchor_first) ? skip_front : 0;
            auto next = cur;
            ++next;
            auto const back_n = (next == anchor_last) ? skip_back : 0;
            return adjust(buf, front_n, back_n);
        }

        const_iterator &operator++() noexcept
        {
            ++cur;
            return *this;
        }
        const_iterator operator++(int) noexcept
        {
            auto t = *this;
            ++*this;
            return t;
        }
        const_iterator &operator--() noexcept
        {
            --cur;
            return *this;
        }
        const_iterator operator--(int) noexcept
        {
            auto t = *this;
            --*this;
            return t;
        }
    };

    buffers_slice() noexcept = default;

    buffers_slice (buffer_sequence_ref<BufSeqTy> bs, std::size_t offset,
                   std::size_t length = (std::numeric_limits<std::size_t>::max)()) noexcept
    {
        buf_begin = iobu::buffer_sequence_begin(bs);
        buf_end = iobu::buffer_sequence_end(bs);
        init(offset, length);
    }

    auto init (std::size_t offset, std::size_t length = (std::numeric_limits<std::size_t>::max)())
    {
        std::size_t skip = offset;
        while (buf_begin != buf_end)
        {
            std::size_t const sz = buffer_type(*buf_begin).size();
            if (skip < sz)
            {
                skip_front = skip;
                break;
            }
            skip -= sz;
            ++buf_begin;
        }

        if (buf_begin == buf_end)
            return;
        if (length == (std::numeric_limits<std::size_t>::max)())
            return;

        std::size_t left = length;
        auto cursor = buf_begin;
        std::size_t cursor_front = skip_front;
        while (cursor != buf_end && left > 0)
        {
            std::size_t const sz = buffer_type(*cursor).size();
            std::size_t const avail = sz - cursor_front;
            if (left <= avail)
            {
                skip_back = avail - left;
                ++cursor;
                buf_end = cursor;
                return;
            }
            left -= avail;
            ++cursor;
            cursor_front = 0;
        }
        buf_end = cursor;
    }

    buffers_slice (iterator_type first, iterator_type last, std::size_t front_skip = 0,
                   std::size_t back_skip = 0) noexcept
        : buf_begin(first), buf_end(last), skip_front(front_skip), skip_back(back_skip)
    {
    }

    const_iterator begin () const noexcept
    {
        return const_iterator(buf_begin, buf_begin, buf_end, skip_front, skip_back);
    }

    const_iterator end () const noexcept
    {
        return const_iterator(buf_end, buf_begin, buf_end, skip_front, skip_back);
    }

  private:
    iterator_type buf_begin{};
    iterator_type buf_end{};
    std::size_t skip_front = 0;
    std::size_t skip_back = 0;
};

template <read_buffer_sequence BufSeqTy>
    requires(not write_buffer_sequence<BufSeqTy>)
buffers_slice(BufSeqTy const &bs, std::size_t offset,
              std::size_t length = (std::numeric_limits<std::size_t>::max)()) noexcept
    -> buffers_slice<BufSeqTy>;

template <read_buffer_sequence BufSeqTy>
    requires(write_buffer_sequence<BufSeqTy>)
buffers_slice(BufSeqTy &bs, std::size_t offset,
              std::size_t length = (std::numeric_limits<std::size_t>::max)()) noexcept
    -> buffers_slice<BufSeqTy>;

} // namespace iobu
