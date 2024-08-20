#pragma once
#include <string>
#include <memory>
#include "source.h"

namespace mkd {
    enum BlockType {
        BLOCK_TYPE_QUOTE,
        BLOCK_TYPE_LIST_ITEM,
        BLOCK_TYPE_LIST,
        BLOCK_TYPE_THEMATIC_BREAK,
        BLOCK_TYPE_HEADING,
        BLOCK_TYPE_CODE_BLOCK,
        BLOCK_TYPE_HTML_BLOCK,
        BLOCK_TYPE_LINK_REF_DEF,
        BLOCK_TYPE_PARAGRAPH,
        BLOCK_TYPE_BLANK_LINE
    };

    class Block {
    public:
        Block(BlockType type);
        virtual ~Block();

        static std::shared_ptr<Block> parse(Source& src);

        const BlockType type;
    };
}

#include "blocks/list.h"
#include "blocks/thematic_break.h"
#include "blocks/heading.h"
#include "blocks/paragraph.h"
#include "blocks/blank_line.h"