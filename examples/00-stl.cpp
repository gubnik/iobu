#include "iobu/buffers_front.hpp"
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
    auto first_5 = iobu::buffers_slice(dest, 0, 5);
    static_assert(iobu::read_buffer_sequence<decltype(first_5)>);
    auto rd_buf = iobu::buffers_front(first_5);
    std::println("slice of first 5: {}",
                 std::string_view{(char const *)rd_buf.data(), rd_buf.size()});
}
