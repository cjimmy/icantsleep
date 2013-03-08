#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    bkgrndColor.set(255,255,255);
    ofBackground(bkgrndColor);
    ofEnableSmoothing();
    
	text = "";
	position = 0;
    
    multiplier = 6;
    sqWidth = UNIT_LEN;
    startingPt.set(64, 55);
        
    readDataFile("characterMap.txt");
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    
    drawSquareWithDiag(0, 0, 960, false); //-- draws background
    drawSquareWithDiag(960, 0, 960, false);
//    drawLowerLeftTextUnit(1,0); 
//    drawUpperRightTextUnit(1,1);
//    drawLowerLeftTextUnit(2,2);
//    drawUpperRightTextUnit(1,3);
    drawText();
}


//--------------------------------------------------------------
void testApp::readDataFile(string fileName) {
    ofFile file;
    string line;
    int pos;
    int i, j = 0;
    try {
        file.open(ofToDataPath(fileName), ofFile::ReadOnly, false);
        ofBuffer buffer = file.readToBuffer();
        while (!buffer.isLastLine()) {
            line = buffer.getNextLine();
            while( (pos = line.find(',')) >= 0) //-- parse it out of the commas  
            {  
                string elem = line.substr(0,pos);  
                line = line.substr(pos+1);  
                charMap[i][j++] = ofToInt(elem);
            }
            charMap[i][j] = ofBinaryToInt(line);
            if(i < MAX_ROWS) i++;
            j=0;
        }
    }
    catch(exception& e) {
        cout << "Error: couldn't find or open characterMap.txt file" << endl;
        throw e;
    }
    
    //-- print array to console for debugging
    //    for(int i = 0; i < MAX_ROWS; i++) {
    //        for (int j = 0; j < MAX_COLS; j++) {
    //            cout << charMap[i][j] << endl;
    //        }
    //    }
}

//-- characters are represented on a 3x5 or a 4x5 grid of "text units."
//-- To store them, they are 0 or 1's and stored as a binary number with
//-- 15 or 20 bits of info. This function decodes that encoding and matches.
//-- the inputted text to charMap, which binarily describes how to draw it.
//-- If it encounters other special characters, like ? and !, it will map those
//-- as well.
//-- Before that, charMap has two other pieces of info: whether it is a 3x5
//-- or a 4x5, and whether it has diagonals in the character. If there are
//-- diagonals, we look at another storage array and get those points.
//--------------------------------------------------------------
void testApp::drawText() {    
    ofPoint cursorPos;
    int charWidth, stringOffset;
    cursorPos.set(0,0);
    text = ofToLower(text);
    if (text.length() > 0) { //-- make sure there's text
        for(int i = 0; i < text.length(); i++){ //-- iterate through each letter
            
            //-- if it's a letter
            if(text[i] <= 'z' && text[i] >= 'a'){
                int normalizedChar = text[i] - 'a';
                if ( charMap[normalizedChar][0] ) {
                    charWidth = NORMAL_CHAR_WIDTH; //-- check whether it's normal or wide
                    stringOffset = NORMAL_STR_OFFSET; //-- converting a uint to binary results in a lot of leading zeros
                } else {
                    charWidth = WIDE_CHAR_WIDTH;
                    stringOffset = WIDE_STR_OFFSET;
                }
                string binaryMap = ofToBinary(charMap[normalizedChar][2]); //-- then pull the corresponding character map
                for (int j = stringOffset; j < binaryMap.length(); j++) { //-- draw the character
                    if(binaryMap[j] == '1' ) drawTextUnit(cursorPos.x + (j-stringOffset)%charWidth, cursorPos.y + (j-stringOffset)/charWidth);
                }
                
                //-- if it needs diagonal pieces, add them here before advancing cursorPos
                if ( charMap[normalizedChar][1] ) { //-- if it needs diagonal pieces
                    switch (text[i]) {
                        case 'b':
                            drawLowerLeftTextUnit(cursorPos.x+2, cursorPos.y+0);
                            break;
                        case 'd':
                            drawLowerLeftTextUnit(cursorPos.x+2, cursorPos.y+0);
                            break;
                        case 'g':
                            drawLowerLeftTextUnit(cursorPos.x+2, cursorPos.y+2);
                            break;
                        case 'j':
                            drawUpperRightTextUnit(cursorPos.x, cursorPos.y+4);
                            break;
                        case 'k':
                            drawLowerLeftTextUnit(cursorPos.x+2, cursorPos.y+3);
                            drawUpperRightTextUnit(cursorPos.x+1, cursorPos.y+3);
                            break;
                        case 'n':
                            drawLowerLeftTextUnit(cursorPos.x+1, cursorPos.y+0);
                            drawUpperRightTextUnit(cursorPos.x+1, cursorPos.y+1);
                            drawLowerLeftTextUnit(cursorPos.x+2, cursorPos.y+1);
                            drawUpperRightTextUnit(cursorPos.x+2, cursorPos.y+2);
                            break;
                        case 'p':
                            drawLowerLeftTextUnit(cursorPos.x+2, cursorPos.y+0);
                            break;
                        case 'q':
                            drawLowerLeftTextUnit(cursorPos.x+2, cursorPos.y+0);
                            drawUpperRightTextUnit(cursorPos.x, cursorPos.y+4);
                            break;
                        case 'r':
                            drawLowerLeftTextUnit(cursorPos.x+2, cursorPos.y+3);
                            drawUpperRightTextUnit(cursorPos.x+1, cursorPos.y+3);
                            break;
                        case 's':
                            drawLowerLeftTextUnit(cursorPos.x+1, cursorPos.y+1);
                            drawUpperRightTextUnit(cursorPos.x+0, cursorPos.y+2);
                            drawLowerLeftTextUnit(cursorPos.x+2, cursorPos.y+2);
                            drawUpperRightTextUnit(cursorPos.x+1, cursorPos.y+3);
                            break;

                            
                        default:
                            break;
                    }
                }
                
                cursorPos.x += charWidth + 1; //-- advance the cursor position with a small space
            }
            
            //-- space
            else if( text[i] == ' ' ) { 
                cursorPos.x += 1;
            }
            
            //-- new line
            else if( text[i] == '\n' ) { 
                cursorPos.x = 0;
                cursorPos.y += TEXT_LINE_HEIGHT; //-- assumes height of char height of 5 + 1 "pixel" space
            }
            
            //-- exclamation mark
            else if( text[i] == '!' ) {
                string binaryMap = ofToBinary(charMap[26][2]); //-- special case it
                stringOffset = NORMAL_STR_OFFSET;
                charWidth = NORMAL_CHAR_WIDTH;
                for (int j = stringOffset; j < binaryMap.length(); j++) {
                    if( binaryMap[j] == '1' ) drawTextUnit(cursorPos.x + (j-stringOffset)%charWidth, cursorPos.y + (j-stringOffset)/charWidth);
                }
                cursorPos.x += charWidth - 1; //-- no space
            }
             
            //-- period
            else if( text[i] == '.' ) {
                string binaryMap = ofToBinary(charMap[27][2]); //-- special case it
                stringOffset = NORMAL_STR_OFFSET;
                charWidth = NORMAL_CHAR_WIDTH;
                for (int j = stringOffset; j < binaryMap.length(); j++) {
                    if( binaryMap[j] == '1' ) drawTextUnit(cursorPos.x + (j-stringOffset)%charWidth, cursorPos.y + (j-stringOffset)/charWidth);
                }
                cursorPos.x += charWidth - 1; //-- no space
            }
            
            //-- question mark
            else if( text[i] == '?' ) {
                string binaryMap = ofToBinary(charMap[28][2]); //-- special case it
                stringOffset = NORMAL_STR_OFFSET;
                charWidth = NORMAL_CHAR_WIDTH;
                for (int j = stringOffset; j < binaryMap.length(); j++) {
                    if( binaryMap[j] == '1' ) drawTextUnit(cursorPos.x + (j-stringOffset)%charWidth, cursorPos.y + (j-stringOffset)/charWidth);
                }
                cursorPos.x += charWidth + 1;
            }
        }
    }    
    
}

//--------------------------------------------------------------
void testApp::drawLowerLeftTextUnit(int x, int y) {
    ofPushMatrix();
    ofTranslate(startingPt.x+x*sqWidth, startingPt.y+y*sqWidth);
    int w = sqWidth;
    ofSetLineWidth(LINE_WIDTH);
    int halfSpace = LINE_WIDTH*multiplier/2;
    ofSetColor(bkgrndColor);
    ofTriangle(0, -halfSpace, 0, w, w+halfSpace, w); //-- clear the background with a triangle
    ofSetColor(0, 0, 0);
    for(int i = halfSpace; i < w; i += LINE_WIDTH*multiplier) { //-- draw just offset from diagonal
        ofLine(0, i, w-i, w);
    }
    ofLine(halfSpace, 0, w, w-halfSpace); //-- last line before its dashed
    drawSmallLine_LL(1.0);
    drawSmallLine_LL(3.0);
    drawSmallLine_LL(5.0);
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::drawSmallLine_LL(float i) {
    ofSetColor(255, 255, 255);
    float halfSpace = LINE_WIDTH*multiplier/2.0;
    ofSetLineWidth(LINE_WIDTH+1);
    float k = (sqWidth-halfSpace)/7.0;
    ofLine(halfSpace+i*k, i*k, halfSpace+(i+1.0)*k, (i+1.0)*k);
}

//--------------------------------------------------------------
void testApp::drawUpperRightTextUnit(int x, int y) {
    ofPushMatrix();
    ofTranslate(startingPt.x+x*sqWidth, startingPt.y+y*sqWidth);
    int w = sqWidth;
    ofSetLineWidth(LINE_WIDTH);
    int halfSpace = LINE_WIDTH*multiplier/2;
    ofSetColor(bkgrndColor);
    ofTriangle(-halfSpace, 0, w, 0, w, w+halfSpace); //-- clear the background with a triangle
    ofSetColor(0, 0, 0);
    for(int i = halfSpace; i < w; i += LINE_WIDTH*multiplier) { //-- draw just offset from diagonal
        ofLine(i, 0, w, w-i);
    }
    ofLine(0, halfSpace, w-halfSpace, w); //-- last line before its dashed
    drawSmallLine_UR(1.0);
    drawSmallLine_UR(3.0);
    drawSmallLine_UR(5.0);
    ofPopMatrix();
}
//--------------------------------------------------------------
void testApp::drawSmallLine_UR(float i) {
    ofSetColor(255, 255, 255);
    float halfSpace = LINE_WIDTH*multiplier/2.0;
    ofSetLineWidth(LINE_WIDTH+1);
    float k = (sqWidth-halfSpace)/7.0;
    ofLine(i*k, halfSpace+i*k,(i+1.0)*k, halfSpace+(i+1.0)*k);
}

//--------------------------------------------------------------
void testApp::drawTextUnit(int x, int y) {
    ofPushMatrix();
    ofTranslate(startingPt.x, startingPt.y);
    drawSquareWithDiag(x*sqWidth, y*sqWidth, sqWidth, true);
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::drawSquareWithDiag(float x, float y, int w, bool bIsText) {
    int lineWidth = LINE_WIDTH;
    ofPushMatrix();
    
    if(bIsText) {
        ofRotateY(180.0);
        ofTranslate(-w-x,y);
    } else {
        ofTranslate(x, y);
    }
    
    ofSetColor(bkgrndColor);
    ofRect(0, 0, w, w); //-- clear the background
    ofSetLineWidth(lineWidth);
    ofSetColor(0, 0, 0);
    for(int i = lineWidth*multiplier/2; i < w; i += lineWidth*multiplier) { //-- draw just offset from diagonal
        ofLine(0, w-i, w-i, 0);
        ofLine(i, w, w, i);
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	/* //-- for debugging
    if (key=='=') sqWidth++;
    if (key=='-' && sqWidth > 1) sqWidth--;
    if (key==OF_KEY_UP) lineWidth+=1;
    if (key==OF_KEY_DOWN && lineWidth > 1) lineWidth -= 1; 
    if ( key == 'w') startingPt.y--;
    if ( key == 's') startingPt.y++;
    if ( key == 'd') startingPt.x++;
    if ( key == 'a') startingPt.x--;

    if (key == 'p') {
        cout << "x: " << startingPt.x << endl;
        cout << "y: " << startingPt.y << endl;
        cout << "square width: " << sqWidth << endl;
        cout << "multiplier: " << multiplier << endl;
        //cout << "line width: " << lineWidth << endl;


    }
    */ 
    
    
    if ( (key >=65 && key <=122) || key == 32 || key == 33 || key == 46 || key == 63) {
        //cout << key << endl;
		text.insert(text.begin()+position, key);
		position++;
	}
    
	if (key==OF_KEY_RETURN) {
		text.insert(text.begin()+position, '\n');
		position++;
	}
    
	if (key==OF_KEY_BACKSPACE) {
		if (position>0) {
			text.erase(text.begin()+position-1);
			--position;
		}
	}
    
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}