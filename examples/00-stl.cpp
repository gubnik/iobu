#include "iobu/buffers_cat.hpp"
#include "iobu/buffers_slice.hpp"
#include "iobu/concepts/buffer_sequence.hpp"
#include <cstddef>
#include <iobu/buffer_ref.hpp>
#include <iobu/buffers_copy.hpp>
#include <print>
#include <string>
#include <string_view>
#include <vector>

int main (int argc, char **argv)
{
    std::vector<std::byte> dest;
    dest.resize(1'024);
    std::string src = "Hello world!";
    auto const copied = iobu::bufcpy(src, dest);
    std::println("Copied: {}", copied);
    std::println("src: {}", src);
    std::println("dest: {}", std::string_view{(char const *)dest.data(), copied});
    auto const cat = iobu::buffers_concatenation(src, dest);
    for (iobu::read_buffer_ref rb : cat)
    {
        std::println("From cat: {}", std::string_view{(char const *)rb.data(), rb.size()});
    }
    auto dest_1{dest};
    std::size_t const n = iobu::bufcpy(cat, dest_1);
    std::println("Copied from cat: {}",
                 std::string_view{(char const *)dest_1.data(), dest_1.size()});
    auto first_15 = iobu::buffers_slice(dest_1, 0, 15);
    static_assert(iobu::read_buffer_sequence<decltype(first_15)>);
    for (auto const rd_buf : first_15)
    {
        std::println("slice of first 15: {}",
                     std::string_view{(char const *)rd_buf.data(), rd_buf.size()});
    }
}
