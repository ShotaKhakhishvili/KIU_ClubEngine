#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <cstddef>

namespace CE::FileIO
{
    /**
     * @brief Reads an entire text file into a string.
     * @param path Path to the file.
     * @return File contents as std::string.
     */
    std::string ReadTextFile(const std::filesystem::path& path);

    /**
     * @brief Writes text to a file, overwriting existing content.
     * @param path Path to the file.
     * @param content Text to write.
     */
    void WriteTextFile(const std::filesystem::path& path, std::string_view content);

    /**
     * @brief Appends text to the end of a file.
     * @param path Path to the file.
     * @param content Text to append.
     */
    void AppendTextFile(const std::filesystem::path& path, std::string_view content);

    /**
     * @brief Reads an entire file as raw binary data.
     * @param path Path to the file.
     * @return File contents as a byte array.
     */
    std::vector<std::byte> ReadBinaryFile(const std::filesystem::path& path);

    /**
     * @brief Writes raw binary data to a file, overwriting existing content.
     * @param path Path to the file.
     * @param data Binary data to write.
     */
    void WriteBinaryFile(const std::filesystem::path& path, const std::vector<std::byte>& data);

    /**
     * @brief Checks whether a file or directory exists.
     * @param path Path to check.
     * @return True if exists, false otherwise.
     */
    bool Exists(const std::filesystem::path& path);

    /**
     * @brief Checks whether a path refers to a directory.
     * @param path Path to check.
     * @return True if directory, false otherwise.
     */
    bool IsDirectory(const std::filesystem::path& path);

    /**
     * @brief Creates directories recursively if they do not exist.
     * @param path Directory path.
     * @return True if created or already exists.
     */
    bool CreateDirectories(const std::filesystem::path& path);

    /**
     * @brief Removes a file or directory.
     * @param path Path to remove.
     * @return True if removed successfully.
     */
    bool Remove(const std::filesystem::path& path);

    /**
     * @brief Returns the size of a file in bytes.
     * @param path Path to the file.
     * @return File size in bytes.
     */
    std::uintmax_t FileSize(const std::filesystem::path& path);
}