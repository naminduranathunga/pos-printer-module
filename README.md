# pos-printer-module
Allow to print a document 


## Commands

> printdoc.exe list

This will give a list of all the printers available in the system.


### Print a xml file

The program will accept a file or standard input and print it to the printer. 

To print using a file, use the following command:

> pos_print.exe print -f "C:\Users\user\Documents\document.xml" -p "Printer Name"

To print using standard input, use the following command:

> pos_print.exe print -p "Printer Name"

or use pipeline to send the content to the program:

> cat "C:\Users\user\Documents\document.xml" | pos_print.exe print -p "Printer Name"



## Syntax

This module uses xml do define the document to be printed. The xml file must have the following structure:

```xml
<document>
    <page width="58mm" height="auto" comment="for 58mm document">
        <column>
            <text>Text 1</text>
            <text>=========================</text>
            <row padding="10dp 0 15dp 0">
                <text width="33%" font="Arial" fsize="24">Text 2</text>
                <text width="33%">Text 3</text>
                <text width="33%">Text 4</text>
            </row>
            <text>=========================</text>
        </column>
    </page>
</document>
```

The document tag is the root tag and must have at least one page tag. The page tag must have a width attribute that defines the width of the page. You can use rows and columns to organize the content of the page. The text tag is used to print text in the document. The row tag is used to organize the text in columns. The text tag inside the row tag must have a width attribute that defines the width of the column.

### column

The column tag is used to organize the content of the page in columns.

Properties:
* width
* height
* padding
* margin

### row

The row tag is used to organize the content of the page in rows.

Properties:
* width
* height
* padding
* margin

### text

The text tag is used to print text in the document.

Properties:
* width
* height
* padding
* margin
* font
* fsize
* fweight (100, 200, ... 900)
* italic (true or false)
* underline (true or false)
* align (left, center, right)

