
#ifndef PAGE_H
#define PAGE_H 1

#include "block.h"
#include <gdiplus.h>


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

    class Image: public Block{
        public:
            Image(rapidxml::xml_node<>* image_node);
            ~Image();
            void CalculateRect(LayoutReferenceToolbox toolbox);
            void RenderBlock(HDC hdc);

            string src;
            string width;
            string height;
            int image_width;
            int image_height;
            float aspect_ratio;

        private:
            Gdiplus::Bitmap* bitmap;
            ULONG_PTR gdiplusToken;
            string parse_base64Images(string src);
            string temp_file_name;

    };
};

#endif