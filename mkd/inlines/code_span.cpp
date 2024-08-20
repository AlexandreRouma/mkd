#include "code_span.h"

namespace mkd::inlines {
    CodeSpan::CodeSpan(const std::string& code) :
        Inline(INLINE_TYPE_CODE_SPAN),
        code(code)
    {}

    std::shared_ptr<CodeSpan> CodeSpan::parse(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect a backtick string
        int backticks = parseBacktickString(src);
        if (!backticks) {
            src.abort();
            return NULL;
        }

        // Read characters until a matching backtick string
        std::string code;
        while (true) {
            // If no more characters are available and the string didn't end with the correct back string, give up
            if (!src.available()) {
                src.abort();
                return NULL;
            }

            // Look for a backtick string
            int bstr = parseBacktickString(src);

            // If it matches the one in the beginning, stop
            if (bstr == backticks) { break; }

            // Otherwise add them to the code
            for (int i = 0; i < bstr; i++) { code += '`'; }

            // Read as many non-backtick characters as possible
            while (src.expect([&code](char c){
                // Don't allow backticks
                if (c == '`') { return false; }

                // Line endings are converted to spaces
                if (c == '\n') { c = ' '; }

                // Accept and add to the code string
                code += c;
                return true;
            }));
        }

        // If the string starts and ends with a space, remove them
        if (code.starts_with(" ") && code.ends_with(" ")) {
            code = code.substr(1, code.size() - 2);
        }

        // Commit the parsing session and return successfully
        src.commit();
        return std::make_shared<CodeSpan>(code);
    }

    int CodeSpan::parseBacktickString(Source& src) {
        // Expect any number of backticks
        int count = 0;
        for (; src.expect('`'); count++);
        return count;
    }
}