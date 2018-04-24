#include "base/io_buffer.h"

namespace base {
IOBuffer::IOBuffer(size_t size) {
  data_ = new char[size];
}

IOBuffer::~IOBuffer() {
  if (data_) {
    delete[] data_;
  }
  data_ = nullptr;
}

IOBufferWithSize::IOBufferWithSize(size_t size) : IOBuffer(size), size_(size) {}

GrowableIOBuffer::GrowableIOBuffer() : IOBuffer(), capacity_(0), offset_(0) {}

void GrowableIOBuffer::SetCapacity(int capacity) {
  // realloc will crash if it fails.
  real_data_.Reset();
  real_data_.Reset(new char[capacity]);
  capacity_ = capacity;
  if (offset_ > capacity)
    set_offset(capacity);
  else
    set_offset(offset_);  // The pointer may have changed.
}

void GrowableIOBuffer::set_offset(int offset) {
  offset_ = offset;
  data_ = real_data_.Get() + offset;
}

int GrowableIOBuffer::RemainingCapacity() {
  return capacity_ - offset_;
}

char* GrowableIOBuffer::StartOfBuffer() {
  return real_data_.Get();
}

GrowableIOBuffer::~GrowableIOBuffer() {
  data_ = NULL;
}
}  // namespace base
