#pragma once

#include "iobu/buffer_iterator.hpp"
#include "iobu/buffer_ref.hpp"
#include "iobu/buffer_type.hpp"
#include "iobu/concepts/buffer_sequence.hpp"
#include <concepts>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iobu
{

template <read_buffer_sequence LeftBufSeqTy, read_buffer_sequence RightBufSeqTy>
struct buffers_concatenation final
{
    using left_iterator_type = decltype(buffer_sequence_begin<LeftBufSeqTy>(
        std::declval<buffer_sequence_ref<LeftBufSeqTy>>()));
    using right_iterator_type = decltype(buffer_sequence_begin<RightBufSeqTy>(
        std::declval<buffer_sequence_ref<RightBufSeqTy>>()));

    struct common_iterator;
    using iterator_type = common_iterator;

    using buffer_type = std::conditional_t<write_buffer_sequence<LeftBufSeqTy> and
                                               write_buffer_sequence<RightBufSeqTy>,
                                           write_buffer_ref, read_buffer_ref>;

    buffers_concatenation (LeftBufSeqTy &left_seq, RightBufSeqTy &right_seq)
        requires(std::same_as<buffer_type, write_buffer_ref>)
        : m_left_begin{write_buffers_begin(left_seq)}, m_left_end{write_buffers_end(left_seq)},
          m_right_begin{write_buffers_begin(right_seq)}, m_right_end{write_buffers_end(right_seq)}
    {
    }

    buffers_concatenation (LeftBufSeqTy const &left_seq, RightBufSeqTy const &right_seq)
        requires(not std::same_as<buffer_type, write_buffer_ref>)
        : m_left_begin{read_buffers_begin(left_seq)}, m_left_end{read_buffers_end(left_seq)},
          m_right_begin{read_buffers_begin(right_seq)}, m_right_end{read_buffers_end(right_seq)}
    {
    }

    struct common_iterator final
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using iterator_concept = std::bidirectional_iterator_tag;
        using value_type = buffer_type;
        using difference_type = std::ptrdiff_t;
        using reference = value_type;
        using pointer = void;

        common_iterator() noexcept = default;

        common_iterator (buffers_concatenation const *owner, left_iterator_type left,
                         right_iterator_type right, bool on_right) noexcept
            : cat{owner}, left_it{std::move(left)}, right_it{std::move(right)}, on_right{on_right}
        {
            if (cat && !this->on_right && left_it == cat->m_left_end)
                this->on_right = true;
        }

        auto operator*() const noexcept -> reference
        {
            return on_right ? reference{*right_it} : reference{*left_it};
        }

        auto operator++() noexcept -> common_iterator &
        {
            if (on_right)
            {
                ++right_it;
            }
            else if (++left_it == cat->m_left_end)
            {
                on_right = true;
            }

            return *this;
        }

        auto operator++(int) noexcept -> common_iterator
        {
            auto previous = *this;
            ++*this;
            return previous;
        }

        auto operator--() noexcept -> common_iterator &
        {
            if (on_right)
            {
                if (right_it != cat->m_right_begin)
                {
                    --right_it;
                }
                else
                {
                    on_right = false;
                    left_it = cat->m_left_end;
                    --left_it;
                }
            }
            else
            {
                --left_it;
            }

            return *this;
        }

        auto operator--(int) noexcept -> common_iterator
        {
            auto previous = *this;
            --*this;
            return previous;
        }

        friend bool operator==(common_iterator const &a, common_iterator const &b) noexcept
        {
            if (a.cat != b.cat || a.on_right != b.on_right)
            {
                return false;
            }

            if (!a.cat)
                return true;
            bool const res = a.on_right ? a.right_it == b.right_it : a.left_it == b.left_it;
            return res;
        }

        friend bool operator!=(common_iterator const &a, common_iterator const &b) noexcept
        {
            return !(a == b);
        }

      private:
        buffers_concatenation const *cat{nullptr};
        left_iterator_type left_it;
        right_iterator_type right_it;
        bool on_right{false};
    };

    auto begin () const noexcept -> common_iterator
    {
        return {this, m_left_begin, m_right_begin, false};
    }

    auto end () const noexcept -> common_iterator
    {
        return {this, m_left_end, m_right_end, true};
    }

    auto cbegin () const noexcept -> common_iterator
    {
        return {this, m_left_begin, m_right_begin, false};
    }

    auto cend () const noexcept -> common_iterator
    {
        return {this, m_left_end, m_right_end, true};
    }

  private:
    left_iterator_type m_left_begin;
    left_iterator_type m_left_end;
    right_iterator_type m_right_begin;
    right_iterator_type m_right_end;
};

} // namespace iobu
