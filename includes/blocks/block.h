#ifndef Block_H
#define Block_H 1

#include <string>
#include <vector>
#include <rapidxml/rapidxml.hpp>
#include <iostream> 
#include <windows.h>


using namespace std;

//const char* JUSTIFY_CONTENT_START = "start";
//const char* JUSTIFY_CONTENT_CENTER = "center";
//const char* JUSTIFY_CONTENT_END = "end";
const int MEASEUERMENT_UNIT_AUTO = -10000;

namespace SimpleDoc{
        
    
    enum RelativeDirection {
        HORIZONTAL,
        VERTICAL
    };

    /**
     * A struct to hold the layout of a block
     * -1 means not set or not applicable
    */
    struct LayoutRect{
        int x;
        int y;
        int width;
        int height;
        int innerWidth;
        int innerHeight;
        int maxWidth;
        int maxHeight;
    };

    struct LayoutReferenceToolbox
    {
        /* data */
        int dpi;
        int pageWidth;
        int pageHeight;
        int parentWidth;
        int parentHeight;
    };

    struct Padding{
        int top;
        int right;
        int bottom;
        int left;
    };

    typedef struct Padding Border;



    /**
     * The base class for all blocks
     *  allowed units => pt, mm, % - no unit means pt  
    */
    class Block{
        public:
            // public properties
            string name;
            string border;
            string padding;
            string margin;
            string width;
            string height;
            Padding padding_values = {0, 0, 0, 0};
            Border border_values = {0, 0, 0, 0};

            

            Block* parent = nullptr;
            vector<Block*> children;
            int points = 0;
            LayoutRect layoutRect = {0, 0, -1, -1, -1, -1, -1, -1};

            // public methods
            Block(string name);
            Block(rapidxml::xml_node<>* block_node);
            Block* get_parent();

            /**
             * Calculate the basic rect info ---> width, height
            */
            virtual void CalculateRect(LayoutReferenceToolbox toolbox);

            /**
             * Render the layout's position info
            */
            void CalculateRectPosition(LayoutReferenceToolbox toolbox);

            /**
             * Render the layout's size info
            */
           virtual void RenderBlock(HDC dc);

        protected:    
            /**
             * Parse the value and return the points
            */
            int parse_unit(string value, LayoutReferenceToolbox* toolbox, RelativeDirection direction = HORIZONTAL); 
            void fill_children(rapidxml::xml_node<>* block_node);
            

    };
};

#endif