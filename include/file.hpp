#pragma once
#include <cstdint>
#include <expected>
#include <memory>
#include <string>
#include <utility>
#include <string_view>

class File {
 public:
  explicit File(std::string name, std::string path)
      : name_{std::move(name)}, path_{std::move(path)} {
    HandleResult(Open());
  }

  explicit File(std::string name, std::string path, const uint64_t size)
      : name_{std::move(name)}, path_{std::move(path)}, size_{size} {
    HandleResult(Open());
  }

  [[nodiscard]] std::expected<void, int32_t> Open() noexcept;
  [[nodiscard]] std::expected<void, int32_t> MapMemory() noexcept;

  void Close() noexcept;
  void UnmapMemory() noexcept;

  File& operator=(const File&) = delete;
  File& operator=(File&&) = delete;
  File(const File&) = delete;
  File(File&&) = delete;

  [[nodiscard]] std::string_view name() const {
    return name_;
  }

  [[nodiscard]] const char* c_name() const {
    return name_.c_str();
  }

  [[nodiscard]] std::string_view path() const {
    return path_;
  }

  [[nodiscard]] const char* c_path() const {
    return path_.c_str();
  }

  [[nodiscard]] constexpr uint64_t size() const {
    return size_;
  }

  [[nodiscard]] constexpr uint8_t* memory_map() const {
    return memory_map_;
  }

  [[nodiscard]] constexpr int32_t file_descriptor() const {
    return file_descriptor_;
  }


  ~File() {
    UnmapMemory();
    Close();
  }

 private:
  constexpr static uint64_t kPageSize = {4 * 1024};

  std::string name_{"default.dart"};
  std::string path_{"/database/"};
  uint64_t size_ {kPageSize};
  uint8_t* memory_map_{nullptr};
  int32_t file_descriptor_{-1};

  void HandleResult(std::expected<void, int32_t> result) const;
};