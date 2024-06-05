
#ifndef PAGE_H
#define PAGE_H 1

#include "block.h"

namespace SimpleDoc{
    // this contains printer specific information for a page
    class Page: public Block{
        private:
            int resolution; // dpi

        public:
            Page(rapidxml::xml_node<>* page_node);
            void CalculateRect(LayoutReferenceToolbox toolbox);
    };

    class Column: public Block{
        public:
            std::string gap;
            std::string justifyContent;
            Column(rapidxml::xml_node<>* column_node);
            void CalculateRect(LayoutReferenceToolbox toolbox);
    };

    class Row: public Block{
        public:
            std::string gap;
            std::string justifyContent;
            Row(rapidxml::xml_node<>* row_node);
            void CalculateRect(LayoutReferenceToolbox toolbox);
    };

    class Text: public Block{
        public:
            Text(rapidxml::xml_node<>* text_node);
            void CalculateRect(LayoutReferenceToolbox toolbox);
            void RenderBlock(HDC hdc);

            string text;
            string fontName;
            string fontSize;
            string color;
            int align;
            int fontWeight;
            bool italic;
            bool underline;
        private: 
            int font_height;
    };
};

#endif