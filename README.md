# iobu

IOBU is an independent library providing generic buffer utilities.

In IOBU, a buffer is simply a pointer and a length, thus any type that exposes these components is suited to
work with IOBU. Such types include, among others:
- `std::string`
- `std::span<std::byte>`
- `std::vector<uint8_t>`
- Asio and Capy `mutable_buffer` and `const_buffer`

IOBU inherits the notion of *buffer sequences* from Asio: a buffer sequence is either a single buffer, or a bidirectional range of buffers.
All IOBU utilities are designed to work for buffer sequences.
