#include <iostream>
#include <graphics.h>
#include <map>
#include <queue>


#define STB_IMAGE_IMPLEMENTATION
#include <F:/Algoritmica/Probleme/QuadTreeProject/stb-master/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <F:/Algoritmica/Probleme/QuadTreeProject/stb-master/stb_image_write.h>

#define POWER (1<<8)-1
#define MAX_HEIGHT 2048
#define MAX_WIDTH 2048

using namespace std;

int WINDOW_WIDTH, WINDOW_HEIGHT;
const char * imagePath = "image3.jpg";

struct color
{
    int red = 0;
    int green = 0;
    int blue = 0;
    color (int x)
    {
        this->red = x & POWER;
        this->green = (x >> 8) & POWER;
        this->blue = (x >> 16) & POWER;
    }
    color () {};
    color(int &&red, int &&green, int &&blue)
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }
    color(const color &c)
    {
        this->red = c.red;
        this->blue = c.blue;
        this->green = c.green;
    }
    void operator += (const color &col)
    {
        this->red += col.red;
        this->green += col.green;
        this->blue += col.blue;
    }
    bool operator < (const color & c) const
    {
        if(red != c.red)
            return red < c.red;
        if(green != c.green);
        return green < c.green;
        return blue < c.blue;
    }
    color operator / (int scalar)
    {
        color newColor;
        newColor.red = this->red / scalar;
        newColor.blue = this->blue / scalar;
        newColor.green = this->green / scalar;
        return newColor;
    }
    void print()
    {
        cout << this->red << ' ' << this->green << ' ' << this->blue << '\n';
    }
};

color grid[MAX_HEIGHT + 5][MAX_WIDTH + 5];

struct point
{
    int row;
    int column;
    point operator + (point b)
    {
        point newPoint;
        newPoint.row = this->row + b.row;
        newPoint.column = this->column + b.column;
        return newPoint;
    }
    point operator / (int scalar)
    {
        point newPoint = {this->row / scalar, this->column / scalar};
        return newPoint;
    }
    point operator + (int scalar)
    {
        point newPoint = {this->row + scalar, this->column + scalar};
        return newPoint;
    }
    bool operator == (point p)
    {
        return this->row == p.row && this->column == p.column;
    }
    bool operator > (point p)
    {
        if(this->row > p.row || (this->row == p.row && this->column > p.column))
            return true;
        return false;
    }
    friend ostream & operator << (ostream & os, const point & x)
    {
        return os << x.row << ' ' << x.column << '\n';

    }
};

struct node
{
    color avgColor;
    point topLeftCoord, bottomRightCoord;
    node * topLeft = NULL, *topRight = NULL, *bottomLeft = NULL, *bottomRight = NULL;
    bool stop = false;
};

color black(0,0,0);
node * root;

bool checkForSameColor(node * nod)
{
    map<color, bool> check;
    if(nod->topRight)
        check[nod->topRight->avgColor] = true;
    if(nod->topLeft)
        check[nod->topLeft->avgColor] = true;
    if(nod->bottomLeft)
        check[nod->bottomLeft->avgColor] = true;
    if(nod->bottomRight)
        check[nod->bottomRight->avgColor] = true;
    return check.size() == 1;
}

void clearMem(node *& nod)
{
    if(nod->topLeft)
        delete nod->topLeft;
    if(nod->topRight)
        delete nod->topRight;
    if(nod->bottomRight)
        delete nod->bottomRight;
    if(nod->bottomLeft)
        delete nod->bottomLeft;
    nod->topLeft = nod->topRight = nod->bottomRight = nod->bottomLeft = NULL;
}

void buildQuadTree(point topLeft, point botRight, node *& nod)
{
    if(topLeft > botRight)
        return;
    nod = new node;
    nod->topLeftCoord = topLeft, nod->bottomRightCoord = botRight;
    if(topLeft == botRight)
    {
        color cool(grid[topLeft.row][topLeft.column]);
        nod->avgColor = cool;
        nod->stop = true;
//        nod->avgColor.print();
        return;
    }
    point middle = (topLeft + botRight) / 2;
    buildQuadTree(topLeft, middle, nod->topLeft); ///Top Left
    buildQuadTree({topLeft.row, middle.column + 1}, {middle.row, botRight.column}, nod->topRight); /// Top Right
    buildQuadTree({middle.row + 1, topLeft.column}, {botRight.row, middle.column}, nod->bottomLeft); /// Bottom Left
    buildQuadTree(middle + 1, botRight, nod->bottomRight); /// Bottom Right
    if(checkForSameColor(nod))
    {
        if(nod->topLeft)
            nod->avgColor = nod->topLeft->avgColor;
        else if(nod->bottomLeft)
            nod->avgColor = nod->bottomLeft->avgColor;
        else if(nod->bottomRight)
            nod->avgColor = nod->bottomRight->avgColor;
        else if(nod->topRight)
            nod->avgColor = nod->topRight->avgColor;
        nod->stop = true;
        clearMem(nod);
    }
    else
    {
        int cnt = 0;
        if(nod->topLeft)
            nod->avgColor += nod->topLeft->avgColor, cnt++;
        if(nod->bottomLeft)
            nod->avgColor += nod->bottomLeft->avgColor, cnt++;
        if(nod->topRight)
            nod->avgColor += nod->topRight->avgColor, cnt++;
        if(nod->bottomRight)
            nod->avgColor += nod->bottomRight->avgColor, cnt++;
        nod->avgColor = nod->avgColor / cnt;
//nod->avgColor.print();
    }

}

void printPixels(int row1, int row2, int column1, int column2, color cool)
{
    for(int row = row1; row <= row2; ++row)
    {
        for(int col = column1; col <= column2; ++col)
            putpixel(row, col, COLOR(cool.red, cool.green, cool.blue));
    }
}

void displayImage(int currlevel, int maxLevel, node * root)
{
    if(root->stop)
    {
        printPixels(root->topLeftCoord.row, root->bottomRightCoord.row,
                    root->topLeftCoord.column,root->bottomRightCoord.column, root->avgColor);
        return;
    }
    if(currlevel == maxLevel)
    {
        printPixels(root->topLeftCoord.row, root->bottomRightCoord.row,
                    root->topLeftCoord.column,root->bottomRightCoord.column, root->avgColor);
    }
    else
    {
        bool ok = false;
        if(root->topLeft)
            displayImage(currlevel + 1, maxLevel, root->topLeft), ok = true;
        if(root->topRight)
            displayImage(currlevel + 1, maxLevel, root->topRight), ok = true;
        if(root->bottomLeft)
            displayImage(currlevel + 1, maxLevel, root->bottomLeft), ok = true;
        if(root->bottomRight)
            displayImage(currlevel + 1, maxLevel, root->bottomRight), ok = true;
        if(!ok)
        {
            printPixels(root->topLeftCoord.row, root->bottomRightCoord.row,
                        root->topLeftCoord.column,root->bottomRightCoord.column, root->avgColor);
        }
    }
}

void bfsQuadTree(node * root)
{
    queue<node*> q;
    q.push(root);
    while(!q.empty())
    {
        node * curr = q.front();
        q.pop();
        printPixels(curr->topLeftCoord.row, curr->bottomRightCoord.row,
                    curr->topLeftCoord.column,curr->bottomRightCoord.column, curr->avgColor);
        if(curr->stop)
            continue;
        if(curr->topLeft)
            q.push(curr->topLeft);
        if(curr->topRight)
            q.push(curr->topRight);
        if(curr->bottomLeft)
            q.push(curr->bottomLeft);
        if(curr->bottomRight)
            q.push(curr->bottomRight);
    }
}

void readPixels()
{
    int col,rows,n, j = 0;
    unsigned char *data = stbi_load(imagePath, &col, &rows, &n, 3);
    int currCol = 1, currRow = 1;
    WINDOW_WIDTH = col, WINDOW_HEIGHT = rows;
    while(currCol <= col)
    {
        int red = static_cast<int>(data[j]),
            green = static_cast<int>(data[j+1]),
            blue = static_cast<int>(data[j+2]),
            transparency = -1;
        j += 3;
        if(n == 4)
            transparency = static_cast<int>(data[j]), j++;
        grid[currRow][currCol].red = red;
        grid[currRow][currCol].green = green;
        grid[currRow][currCol].blue = blue;
        currRow++;
        if(currRow > rows)
            currRow = 1, currCol++;
    }
}


int main()
{
    readPixels();
    initwindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Quad Tree Project");
    buildQuadTree({0, 0}, {WINDOW_HEIGHT - 1, WINDOW_WIDTH - 1}, root);
//    printPixels(0, WINDOW_HEIGHT - 1, 0, WINDOW_WIDTH - 1, black);
    displayImage(1, 6, root);
//    bfsQuadTree(root);
    getch();

}
