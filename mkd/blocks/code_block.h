#pragma once
#include "../block.h"
#include "../source.h"

namespace mkd::blocks {
    class CodeBlock : public Block {
    public:
        CodeBlock(const std::string& info, const std::string& code);

        static std::shared_ptr<CodeBlock> parse(Source& src);

        const std::string info;
        const std::string code;

    private:
        static std::shared_ptr<CodeBlock> parseIndented(Source& src);
        static std::shared_ptr<CodeBlock> parseFenced(Source& src);
        static int parseCodeFence(Source& src, char c);
        static std::string parseBlankLine(Source& src);
    };
}