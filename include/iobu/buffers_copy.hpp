#pragma once

#include "iobu/buffer_iterator.hpp"
#include "iobu/buffer_ref.hpp"
#include "iobu/concepts/buffer_sequence.hpp"
#include <cstring>
#include <limits>

namespace iobu
{

constexpr struct
{
    template <read_buffer_sequence RdBufSeqTy, write_buffer_sequence WrBufSeqTy>
    auto operator()(RdBufSeqTy const &read_buffers, WrBufSeqTy &write_buffers,
                    std::size_t at_most = std::numeric_limits<std::size_t>::max()) const noexcept
        -> std::size_t
    {
        std::size_t total{};
        std::size_t rd_pos{};
        std::size_t wr_pos{};
        auto const rd_beg = read_buffers_begin(read_buffers);
        auto const rd_end = read_buffers_end(read_buffers);
        auto const wr_beg = write_buffers_begin(write_buffers);
        auto const wr_end = write_buffers_end(write_buffers);
        auto rd_it = rd_beg;
        auto wr_it = wr_beg;
        while (total < at_most and (rd_it != rd_end) and (wr_it != wr_end))
        {
            auto const rd = read_buffer_ref{*rd_it}.remove_prefix(rd_pos);
            auto const wr = write_buffer_ref{*wr_it}.remove_prefix(wr_pos);
            auto const to_copy =
                std::min(std::min<std::size_t>(rd.size(), wr.size()), at_most - total);
            if (to_copy != 0)
            {
                std::memcpy(wr.data(), rd.data(), to_copy);
            }
            if (to_copy >= rd.size())
            {
                ++rd_it;
                rd_pos = 0;
            }
            else
            {
                rd_pos += to_copy;
            }
            if (to_copy >= wr.size())
            {
                ++wr_it;
                wr_pos = 0;
            }
            else
            {
                wr_pos += to_copy;
            }
            total += to_copy;
        }
        return total;
    }
} buffers_copy;

} // namespace iobu
