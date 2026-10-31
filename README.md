# iobu

Iobu (I/O Buffers) is a library providing concepts and generic utilities for operating
on buffers. A buffer is a contiguous region of memory, and `iobu` ensures that any way the programmer
chooses to provide such a region of memory, the utilities for operating on them stay the same.

Iobu aims to be compatible with as many STL and 3rd-party buffer-like facilities as possible via its
open set of structural of concepts. Out of the box, the following types (and more) and considered buffer types in Iobu:
- `std::vector<std::uint8_t>`
- `std::span<std::byte>`
- `std::string`
- `boost::capy::mutable_buffer` and `boost::capy::const_buffer`
- `boost::asio::mutable_buffer` and `boost::asio::const_buffer`

Iobu also inherits the notion of _buffer sequences_ from Asio, making something like `std::vector<std::string>`
a supported buffer type by default. No conversions needed at the call site, no `std::ranges::views` magic.
