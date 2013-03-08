#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
    
#define LINE_WIDTH 2
#define TEXT_LINE_HEIGHT 6
#define UNIT_LEN 48
#define MAX_ROWS 29
#define MAX_COLS 3
#define WIDE_CHAR_WIDTH 4
#define NORMAL_CHAR_WIDTH 3
#define NORMAL_STR_OFFSET 17
#define WIDE_STR_OFFSET 12
    
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);


private:

    void    readDataFile(string fileName);
    void	drawText();
    void    drawSquareWithDiag(float x, float y, int w, bool bIsText);
    void    drawTextUnit(int x, int y);
    void    drawLowerLeftTextUnit(int x, int y);
    void    drawUpperRightTextUnit(int x, int y);
    void    drawSmallLine_LL(float i);
    void    drawSmallLine_UR(float i);
    
    
    string	text;
    int		position;
    int multiplier;
    int sqWidth;
    ofPoint startingPt;
    ofColor bkgrndColor;
    
    unsigned int charMap[MAX_ROWS][MAX_COLS];
};
