#pragma once
#include <string>
#include <vector>
#include "block.h"
#include "inline.h"
#include "source.h"

namespace mkd {
    class Document {
    public:
        Document(const std::string& src);

        std::vector<std::shared_ptr<Block>> blocks;

    private:
        Source src;
    };
}