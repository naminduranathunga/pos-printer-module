#include <blocks/page.h>
#include <string>
#include <iostream>

using namespace std;
using namespace SimpleDoc;
using namespace rapidxml;



Column::Column(xml_node<>* page_node): Block(page_node){
    gap = "0";
    if (page_node->first_attribute("gap") != 0){
        gap = page_node->first_attribute("gap")->value();
    }

    /*justifyContent = JUSTIFY_CONTENT_START;
    if (page_node->first_attribute("justify-content") != 0){
        string justify_content = page_node->first_attribute("justify-content")->value();
        if (justify_content == "center"){
            justifyContent = JUSTIFY_CONTENT_CENTER;
        } else if (justify_content == "end"){
            justifyContent = JUSTIFY_CONTENT_END;
        }
    }*/
    
    fill_children(page_node);
}


/**
 * Calculate the rect for the column
 */
void Column::CalculateRect(LayoutReferenceToolbox toolbox){
    Block::CalculateRect(toolbox);
    
    int max_width = get_parent()->layoutRect.width;
    int max_height = MEASEUERMENT_UNIT_AUTO;

    int gap_x = parse_unit(gap, &toolbox, RelativeDirection::HORIZONTAL);
    int gap_y = parse_unit(gap, &toolbox, RelativeDirection::VERTICAL);

    if (width == "auto"){
        layoutRect.width = max_width;
        // max width for childrn is calculated based on number of children
        int total_gap = gap_x * (children.size() - 1);
    } else {
        layoutRect.width = parse_unit(width, &toolbox, RelativeDirection::HORIZONTAL);
    }

    if (height == "auto"){
        layoutRect.height = max_height;
    } else {
        layoutRect.height = parse_unit(height, &toolbox, RelativeDirection::VERTICAL);
    }

    // set inner width and height
    int inner_width = layoutRect.width - padding_values.left - padding_values.right;
    int inner_height = layoutRect.height - padding_values.top - padding_values.bottom;

    LayoutReferenceToolbox child_toolbox;
    child_toolbox.dpi = toolbox.dpi;
    child_toolbox.pageWidth = toolbox.pageWidth;
    child_toolbox.pageHeight = toolbox.pageHeight;
    child_toolbox.parentWidth = inner_width;
    child_toolbox.parentHeight = inner_height;

    int cursor_x = 0;
    for (int i = 0; i < children.size(); i++){
        Block* child = children[i];
        child->CalculateRect(child_toolbox);
        
        // now set it's position x = cursor_x
        child->layoutRect.y = cursor_x + layoutRect.y + padding_values.top;
        cursor_x = cursor_x + child->layoutRect.height + gap_x;
        child->layoutRect.x = 0;


        // vertical position
        /*if (justifyContent == string(JUSTIFY_CONTENT_CENTER)){
            child->layoutRect.y = (layoutRect.height - child->layoutRect.height) / 2;
        } else if (justifyContent == string(JUSTIFY_CONTENT_END)){
            child->layoutRect.y = layoutRect.height - child->layoutRect.height;
        } else {
            
        }*/
    }

    // set the height of the column based on children
    int max_child_height = 0;
    for (int i = 0; i < children.size(); i++){
        Block* child = children[i];
        max_child_height += child->layoutRect.height;
    }
    layoutRect.height = max_child_height + padding_values.top + padding_values.bottom;
}
