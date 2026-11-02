#pragma once

#include "iobu/buffer_iterator.hpp"
#include "iobu/buffer_ref.hpp"
#include "iobu/buffers_view.hpp"
#include "iobu/concepts/buffer_sequence.hpp"
#include <limits>
#include <utility>

namespace iobu
{

template <read_buffer_sequence RbBufSeqTy> struct buffers_slice final
{
    using raw_iterator_type = decltype(read_buffers_begin(std::declval<RbBufSeqTy const &>()));

    explicit buffers_slice (RbBufSeqTy const &buffers, std::size_t offset = 0,
                            std::size_t length = std::numeric_limits<std::size_t>::max())
        : m_view{buffers}, m_offset{offset}, m_length{length}
    {
    }

    explicit buffers_slice(RbBufSeqTy const &&buffers, std::size_t offset = 0,
                           std::size_t length = std::numeric_limits<std::size_t>::max()) = delete;

    struct slicing_iterator final
    {
        using value_type = read_buffer_ref;
        using difference_type = std::iter_difference_t<raw_iterator_type>;
        using reference = read_buffer_ref;
        using pointer = void;
        using iterator_concept = std::bidirectional_iterator_tag;
        using iterator_category = std::bidirectional_iterator_tag;

        slicing_iterator() noexcept = default;

        slicing_iterator (buffers_slice const *owner, std::size_t position, bool end = false)
            : slice{owner}, end{end}
        {
            set_at(position);
        }

        auto operator*() const noexcept -> reference
        {
            read_buffer_ref const buffer = read_buffer_ref{*curr}.remove_prefix(curr_offset);
            auto const available = slice->limit() - position;

            if (buffer.size() <= available)
            {
                return read_buffer_ref{buffer};
            }

            return read_buffer_ref{buffer.data(), available};
        }

        auto operator++() noexcept -> slicing_iterator &
        {
            if (curr == slice->m_view.end())
            {
                end = true;
                return *this;
            }

            auto const buffer_start = position - curr_offset;
            auto const next_position = buffer_start + read_buffer_ref{*curr}.size();

            set_at(std::min(next_position, slice->limit()));
            if (next_position >= slice->limit())
                end = true;
            return *this;
        }

        auto operator++(int) noexcept -> slicing_iterator
        {
            auto old = *this;
            ++*this;
            return old;
        }

        auto operator--() noexcept -> slicing_iterator &
        {
            auto it = slice->m_view.begin();
            auto const end = slice->m_view.end();
            std::size_t buffer_start = 0;

            bool found = false;
            raw_iterator_type previous{};
            std::size_t previous_offset = 0;
            std::size_t previous_position = 0;

            while (it != end)
            {
                auto const size = read_buffer_ref{*it}.size();
                auto const chunk_start = std::max(buffer_start, slice->m_offset);
                auto const chunk_end = std::min(buffer_start + size, slice->limit());

                if (chunk_start < chunk_end && chunk_start < position)
                {
                    found = true;
                    previous = it;
                    previous_offset = chunk_start - buffer_start;
                    previous_position = chunk_start;
                }

                buffer_start += size;
                ++it;
            }

            if (found)
            {
                curr = previous;
                curr_offset = previous_offset;
                position = previous_position;
            }

            return *this;
        }

        auto operator--(int) noexcept -> slicing_iterator
        {
            auto old = *this;
            --*this;
            return old;
        }

        friend bool operator==(slicing_iterator const &a, slicing_iterator const &b) noexcept
        {
            bool const res = a.slice == b.slice && a.curr == b.curr &&
                             a.curr_offset == b.curr_offset && a.position == b.position &&
                             a.end == b.end;
            return res;
        }

      private:
        void set_at (std::size_t requested_position) noexcept
        {
            if (requested_position >= slice->limit())
            {
                end = true;
                curr = slice->m_view.end();
                return;
            }

            curr = slice->m_view.begin();
            auto const end = slice->m_view.end();
            std::size_t buffer_start = 0;

            while (curr != end)
            {
                auto const size = read_buffer_ref{*curr}.size();

                // Strict comparison makes an exact buffer boundary point to the
                // next non-empty buffer.
                if (requested_position >= buffer_start && requested_position - buffer_start < size)
                {
                    curr_offset = requested_position - buffer_start;
                    position = requested_position;
                    return;
                }

                buffer_start += size;
                ++curr;
            }

            // Clamp positions past the actual data to the real sequence end.
            curr_offset = 0;
            position = buffer_start;
        }

        raw_iterator_type curr{};
        buffers_slice const *slice{};
        std::size_t curr_offset{0};
        std::size_t position{0};
        bool end{false};
    };

    auto limit () const noexcept -> std::size_t
    {
        if (m_length > std::numeric_limits<std::size_t>::max() - m_offset)
        {
            return std::numeric_limits<std::size_t>::max();
        }

        return m_offset + m_length;
    }

    auto begin () const noexcept -> slicing_iterator
    {
        return slicing_iterator{this, m_offset};
    }

    auto end () const noexcept -> slicing_iterator
    {
        return slicing_iterator{this, limit(), true};
    }

  private:
    buffers_view<RbBufSeqTy> m_view;
    std::size_t m_offset{0};
    std::size_t m_length{std::numeric_limits<std::size_t>::max()};
};

} // namespace iobu
