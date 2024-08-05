#pragma once
#include <string>
#include <vector>
#include <stack>
#include <stdexcept>

namespace mkd {
    class Source {
    public:
        Source(const std::string& str) {
            data = str;
        }

        inline void begin() {
            sessions.push(offset);
        }

        inline void commit() {
            if (!sessions.size()) {
                throw std::runtime_error("Cannot commit session, no session exists.");
            }
            sessions.pop();
        }

        inline void abort() {
            if (!sessions.size()) {
                throw std::runtime_error("Cannot abort session, no session exists.");
            }
            offset = sessions.top();
            sessions.pop();
        }

        inline size_t size() const {
            return data.size();
        }

        inline size_t available() const {
            return size() - offset;
        }

        bool empty() const {
            return data.empty();
        }

        inline int tell() const {
            return offset;
        }

        inline void seek(int pos) {
            offset = pos;
        }

        inline char& operator[](int index) {
            return data[offset + index];
        }

        inline void consume(int count) {
            offset += count;
            if (offset > data.size()) {
                throw std::runtime_error("Can't consume more data than is in the list");
            }
        }

        template<typename Func>
        inline bool expect(Func pred) {
            if (!available()) { return false; }
            bool valid = pred(data[offset]);
            if (valid) { consume(1); }
            return valid;
        }

        inline bool expect(char c) {
            if (!available()) { return false; }
            bool valid = (data[offset] == c);
            if (valid) { consume(1); }
            return valid;
        }

    protected:
        std::string data;
        std::stack<int> sessions;
        int offset = 0;
    };
}