#include "Huffman.h"
#include "ColorCounter.h"
#include "List.h"
#include <iostream>

Huffman::Huffman(BMP_Surface *surface)
{
    this->surface = surface;
}

Huffman::~Huffman()
{
}

void Huffman::runHoffman()
{
    ColorCounter *colorCntr = new ColorCounter(this->surface);
    int numColors = colorCntr->countColors();
    colorCntr->sort();

    List<Tree<SingleColorData>> huffTreeList;
    Node<SingleColorData> *nodes = new Node<SingleColorData>[ numColors ];
    Tree<SingleColorData> *trees = new Tree<SingleColorData>[ numColors ];

    for (int i = 0; i < numColors; i++)
    {
		nodes[i].setVar(colorCntr->getColor(i));
		trees[i].setRoot(nodes[i]);
		huffTreeList.insertQueue(trees[i]);
    }

    while (huffTreeList.getSize() > 1)
    {
		Tree<SingleColorData> temp1 = huffTreeList.getItem(0);
		Tree<SingleColorData> temp2 = huffTreeList.getItem(1);
		Tree<SingleColorData> res = temp1 + temp2;
		huffTreeList.insertQueue(res);
		huffTreeList.remFirst();
		huffTreeList.remFirst();
    }

    std::cout << "\n\n-------------------------------\n";
    std::cout << "---------- HUFFMAN CODED ----------\n\n";
    huffTreeList.printList();
}
