#include <blocks/page.h>
#include <string>
#include <iostream>

using namespace std;
using namespace SimpleDoc;
using namespace rapidxml;



Page::Page(xml_node<>* page_node): Block(page_node){
    fill_children(page_node);
}


/**
 * Calculate the rect for the page
 */
void Page::CalculateRect(LayoutReferenceToolbox toolbox){
    int max_width = toolbox.pageWidth;
    int max_height = toolbox.pageHeight;

    if (width == "auto"){
        layoutRect.width = max_width;
    } else {
        layoutRect.width = parse_unit(width, &toolbox, RelativeDirection::HORIZONTAL);
    }

    if (height == "auto"){
        layoutRect.height = max_height;
    } else {
        layoutRect.height = parse_unit(height, &toolbox, RelativeDirection::VERTICAL);
    }
    // page is rendered from top to bottom

    int inner_width = layoutRect.width - padding_values.left - padding_values.right;
    int inner_height = layoutRect.height - padding_values.top - padding_values.bottom;
    LayoutReferenceToolbox child_toolbox = toolbox;
    child_toolbox.parentWidth = inner_width;
    child_toolbox.parentHeight = inner_height;

    int cursor_y = 0;
    for (int i = 0; i < children.size(); i++){
        Block* child = children[i];
        child->CalculateRect(child_toolbox);
        
        // now set it's position y = cursor_y
        child->layoutRect.y = cursor_y;
        cursor_y = cursor_y + child->layoutRect.height;

        // horizontal position
        child->layoutRect.x = 0;
    }

    if (height == "auto"){
        layoutRect.height = cursor_y; 
    }
}