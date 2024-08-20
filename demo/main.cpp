#include <fstream>
#include <sstream>
#include "flog/flog.h"
#include "document.h"


std::string offsetStr(int count) {
    std::string str;
    for (int i = 0; i < count; i++) { str += "    "; }
    return str;
}

void dumpInline(std::shared_ptr<mkd::Inline> inl, int offset = 0) {
    std::string ofs = offsetStr(offset);
    if (inl->type == mkd::INLINE_TYPE_CODE_SPAN) {
        std::shared_ptr<mkd::inlines::CodeSpan> code = std::dynamic_pointer_cast<mkd::inlines::CodeSpan>(inl);
        flog::debug("{}CODE_SPAN: '{}',", ofs, code->code);
    }
    else if (inl->type == mkd::INLINE_TYPE_LINK) {
        std::shared_ptr<mkd::inlines::Link> link = std::dynamic_pointer_cast<mkd::inlines::Link>(inl);
        flog::debug("{}LINK[DEST='{}', TITLE='{}'] \\{", ofs, link->destination, link->title);
        for (const auto& text : link->inlines) {
            dumpInline(text, offset + 1);
        }
        flog::debug("{}}", ofs);
    }
    else if (inl->type == mkd::INLINE_TYPE_IMAGE) {
        std::shared_ptr<mkd::inlines::Image> image = std::dynamic_pointer_cast<mkd::inlines::Image>(inl);
        flog::debug("{}IMAGE[DEST='{}', TITLE='{}'] \\{", ofs, image->destination, image->title);
        for (const auto& text : image->inlines) {
            dumpInline(text, offset + 1);
        }
        flog::debug("{}}", ofs);
    }

    else if (inl->type == mkd::INLINE_TYPE_HARD_BREAK) {
        flog::debug("{}HARD_BREAK,", ofs);
    }
    else if (inl->type == mkd::INLINE_TYPE_SOFT_BREAK) {
        flog::debug("{}SOFT_BREAK,", ofs);
    }
    else if (inl->type == mkd::INLINE_TYPE_TEXT) {
        std::shared_ptr<mkd::inlines::Text> text = std::dynamic_pointer_cast<mkd::inlines::Text>(inl);
        flog::debug("{}TEXT: '{}',", ofs, text->text);
    }
    else {
        flog::debug("{}UNKNOWN_INLINE,", ofs);
    }
}

void dumpBlock(std::shared_ptr<mkd::Block> block, int offset = 0) {
    std::string ofs = offsetStr(offset);
    if (block->type == mkd::BLOCK_TYPE_THEMATIC_BREAK) {
        flog::debug("{}THEMATIC_BREAK,", ofs);
    }
    else if (block->type == mkd::BLOCK_TYPE_PARAGRAPH) {
        std::shared_ptr<mkd::blocks::Paragraph> paragraph = std::dynamic_pointer_cast<mkd::blocks::Paragraph>(block);
        flog::debug("{}PARAGRAPH \\{", ofs);
        for (const auto& text : paragraph->inlines) {
            dumpInline(text, offset + 1);
        }
        flog::debug("{}},", ofs);
    }
    else if (block->type == mkd::BLOCK_TYPE_HEADING) {
        std::shared_ptr<mkd::blocks::Heading> heading = std::dynamic_pointer_cast<mkd::blocks::Heading>(block);
        flog::debug("{}HEADING[{}] \\{", ofs, heading->level);
        for (const auto& text : heading->inlines) {
            dumpInline(text, offset + 1);
        }
        flog::debug("{}},", ofs);
    }
    else if (block->type == mkd::BLOCK_TYPE_CODE_BLOCK) {
        std::shared_ptr<mkd::blocks::CodeBlock> code = std::dynamic_pointer_cast<mkd::blocks::CodeBlock>(block);
        flog::debug("{}CODE_BLOCK[INFO='{}']: '{}',", ofs, code->info, code->code);
    }
    else if (block->type == mkd::BLOCK_TYPE_LIST_ITEM) {
        std::shared_ptr<mkd::blocks::ListItem> item = std::dynamic_pointer_cast<mkd::blocks::ListItem>(block);
        flog::debug("{}LIST_ITEM \\{", ofs);
        for (const auto& text : item->inlines) {
            dumpInline(text, offset + 1);
        }
        flog::debug("{}},", ofs);
    }
    else if (block->type == mkd::BLOCK_TYPE_LIST) {
        std::shared_ptr<mkd::blocks::Heading> heading = std::dynamic_pointer_cast<mkd::blocks::Heading>(block);
        flog::debug("{}HEADING[{}] \\{", ofs, heading->level);
        for (const auto& text : heading->inlines) {
            dumpInline(text, offset + 1);
        }
        flog::debug("{}},", ofs);
    }
    else {
        flog::debug("{}UNKNOWN_BLOCK,", ofs);
    }
}

int main() {
    try {
        // Open source file
        flog::info("Loading source code...");
        std::ifstream file("../test_files/test.md");
        if (!file.is_open()) {
            flog::error("Failed to open markdown file");
            return -1;
        }

        // Read the file's content into a string and close it
        std::stringstream ss;
        ss << file.rdbuf();
        std::string src = ss.str();
        file.close();

        // Parse markdown document
        flog::info("Parsing document...");
        mkd::Document doc(src);

        // Dump all blocks
        flog::debug("===== AST DUMP =====");
        for (const auto& block : doc.blocks) {
            dumpBlock(block);
        }
    }
    catch (std::runtime_error e) {
        flog::error("{}", e.what());
        return -1;
    }
    return 0;
}