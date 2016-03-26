/*
 * @copyright Copyright (C) 2016 Leiden Tech. All rights reserved.
 * @license http://www.gnu.org/licenses/gpl-3.0.txt GNU General Public License v3
 * 
 */
#include <SPI.h>
//This is for text operations by Max72xxPanel used in msgBoard - has to come before the  Max72xxPanel
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

//screen setup - don't really need this since it's only going to be one display anyway
int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 1;

#define MAX72XX_CS 10
//#define MAX72XX_DIN 11
//#define MAX72XX_CLK 13

byte screen[8]={
	B00000000,
	B00000000,
	B00000000,
	B00000000,
	B00000000,
	B00000000,
	B00000000,
	B00000000
};

int cursorIndex=0;

Max72xxPanel matrix = Max72xxPanel(MAX72XX_CS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

//switch setup
int leftButton = 2;
int rightButton = 3;
boolean leftPress=false;
boolean rightPress=false;
boolean leftLongPress=false;
boolean rightLongPress=false;
boolean bothPress=false;
boolean modeFlag=false; //First time through flag

int longPressInterval=60; //The lower the debounce, the higher this must be
int debounce=10;
int mode=1;  //set this to whatever the default should be on start

int wait = 50; // In milliseconds

//Message board setup
int spacer = 1;
int fontWidth = 5 + spacer; // The font width is 5 pixels
String message;
//Any way to set the font?

long randNumber;

//Eye setup
byte eyeBall[8]={
	B00111100,
	B01111110,
	B11111111,
	B11111111,
	B11111111,
	B11111111,
	B01111110,
	B00111100
};

byte eyePupil = B11100111;

// stores current state of eye LEDs
byte eyeCurrent[8];
int currentX;
int currentY;
int cntLoop = 0;
int cntEffect = 0;

// min and max positions
#define MIN -2
#define MAX  2

// perform an effect every # of loop iterations, 0 to disable
#define EFFECT_ITERATION 4

//Race game setup
//Setting up new screen so that if edit doesn't wipe stored image
byte track[8]={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};
int carPosition=3;
int raceScore=0;
int trackWidth=3;
int trackPos=2;
boolean inGame=false;

void wipe(boolean flag = LOW, int wipeWait = 2)
{ //fillscreen with delay 
	for ( int i = 0 ; i <= 7; i++ ) {
		for ( int j = 0 ; j <= 7; j++ ) {
			matrix.drawPixel(i, j, flag);
			matrix.write();
			delay(wipeWait);
		} 
	}
	delay(wait);
}

void setup() {
	Serial.begin(9600);
	matrix.setIntensity(7); // Use a value between 0 and 15 for brightness
	pinMode(leftButton, INPUT_PULLUP);
	pinMode(rightButton, INPUT_PULLUP);
	randomSeed(analogRead(0));
	//init
	transition();
	msgBoard("Ready");
	Serial.setTimeout(1);
}

void loop() {
	message = Serial.readString();
	delay(wait); //This has to be here and not in the if for the Serial read.
	if ( message != "") {
		//displays string from serial - doesn't work well - long string will timeout can't increase timeout because that increases loop delay for everything else - might be nice to be able to have a scrolling infinite message but I don't think people will be using this while it's plugged into a serial port somewhere anyway.
		//Serial.print("message: ");Serial.println(message);
		matrix.setRotation(0); 
		msgBoard(message);
	}
	
	getSwitch();
	
	if (leftLongPress) {
		transition();
		mode++;
		modeFlag=true;
		if (mode > 5) {
			mode=0;
		}
		inGame=false;
	} else if (rightLongPress) {
		transition();
		mode--;
		modeFlag=true;
		if (mode < 0) {
			mode=5;
		}
		inGame=false;
	}
	switch(mode)
	{
		case 0:
			matrix.setRotation(0);
			googlyEye();
			break;
		case 1:
			matrix.setRotation(0);
			if (!modeFlag && leftPress || rightPress) {
				sequence();
			}
			break;     
		case 2:
			matrix.setRotation(0);
			if (!modeFlag && leftPress || rightPress) {
				magic8ball();
			}
			break;  
		case 3:
			if (modeFlag) { //to reset the race if mode is changed during play
				byte track[8]={
					B00000000,
					B00000000,
					B00000000,
					B00000000,
					B00000000,
					B00000000,
					B00000000,
					B00000000
				};
				carPosition=3;
				raceScore=0;
				inGame=true;
			} else {
				matrix.setRotation(1); 
				matrix.drawPixel(7, carPosition, LOW);
				matrix.write();
				delay(wait);
				if (leftPress) {
					delay(debounce);
					carPosition++;
					carPosition = constrain(carPosition, 0, 7);
					//Serial.print("left: ");Serial.println(carPosition);
				}
				if (rightPress) {
					delay(debounce);
					carPosition--;
					carPosition = constrain(carPosition, 0, 7);
					//Serial.print("right: ");Serial.println(carPosition);
				}
				matrix.write();
				race();
			}
			break;  
		case 4:
			if (modeFlag) { //We only want this to execute the first time through
				matrix.setRotation(1); 
				wipe();
				for ( int s = 0 ; s <= 7; s++ ) {
					//Serial.print("s: ");Serial.print(s);Serial.print(" value:");Serial.println(screen[s]);
					setRow(0,s,screen[s]);
				} 
				matrix.write();
			}
			if (leftPress && !modeFlag) {
				cursorIndex++;
				if (cursorIndex > 63) {
					cursorIndex=0;
				}
			} else if (rightPress && !modeFlag) {
				int x;
				int y;
				int oldstate;
				x = round(cursorIndex / 8);
				y = 7 - (cursorIndex % 8);
				oldstate = bitRead(screen[x], y);
				if ( oldstate == HIGH ) {
					bitWrite(screen[x], y, 0);
				}else{
					bitWrite(screen[x], y, 1);
				}
				setRow(0,x,screen[x]);
			}
			editScreen();
			break;       
		case 5:
			if (isBlank()) { //Skip display if stored image is blank - shouldn't be followed by something that uses modeFlag because that will get stripped here if blank.
				if (leftLongPress) {
					mode=0;
				} else {
					mode--;
				}
				break;
			}
			if (modeFlag) { //We only want this to execute the first time through otherwise it flickers
				matrix.setRotation(1); 
				wipe();
				for ( int s = 0 ; s <= 7; s++ ) {
					//Serial.print("s: ");Serial.print(s);Serial.print(" value:");Serial.println(screen[s]);
					setRow(0,s,screen[s]);
				} 
				matrix.write();
			}
			break;
		default: //should never get here
			break;
	}
	//switch reset
	modeFlag=false;
	bothPress=false;
	leftPress=false;
	rightPress=false;
	leftLongPress=false;
	rightLongPress=false;
}

void getSwitch()
{
	int leftLongCount=0;
	int rightLongCount=0;
	while(digitalRead(leftButton) == LOW) { //is being pressed
		leftPress=true;
		if (digitalRead(rightButton) == LOW) {
			bothPress = true;
		}
		delay(debounce);
		leftLongCount++; //For long press
		if (digitalRead(leftButton) == HIGH) { //Lifted
			break;
		}
		if (leftLongCount > longPressInterval){
			leftLongPress=true;
			break;
		}
	}
	if (bothPress) { //don't have to check other button if this is true unless you want right long on both triggered.
		rightPress=true;
		return;
	}
	while(digitalRead(rightButton) == LOW) { //is being pressed
		rightPress=true;
		delay(debounce);
		rightLongCount++; //For long press
		if (digitalRead(rightButton) == HIGH) { //Lifted
			break;
		}
		if (rightLongCount > longPressInterval){
			rightLongPress=true;
			break;
		}
	}
}

void transition()
{ //Between modes
	matrix.fillScreen(HIGH);
	matrix.write();
	delay(500);
	matrix.fillScreen(LOW);
	matrix.write();
}

void editScreen()
{	//left button move cursor++, right button flips value - blink under cursor is min intensity if bit is not set, max if it is set
	matrix.setRotation(1); 
	int x;
	int y;
	int oldstate;
	x = round(cursorIndex / 8);
	y = 7 - (cursorIndex % 8);
	//Serial.print("x: ");Serial.print(x);Serial.print(" y: ");Serial.print(y);Serial.print(" state: ");Serial.println(oldstate);
	oldstate = bitRead(screen[x], y);
	cursorBlink(x,y,oldstate);
	matrix.write();
}

void cursorBlink(int x,int y,int state)
{ //blinks cursor - fast cursor if underlying pixel is set slow if not set
	if ( state == 1 ) { //IS ALREADY ON
		matrix.setIntensity(12);
	} else {
		matrix.setIntensity(7);
	}
	matrix.drawPixel(x, y, LOW);
	matrix.write();
	delay(10);
	matrix.drawPixel(x, y, HIGH);
	matrix.write();
	setRow(0,x,screen[x]);
}

boolean isBlank()
{ //checks to see if screen used in edit mode is blank
	int total=0;
	for ( int i = 0 ; i <= 7; i++ ) {
		total = total + screen[i];
	}
	if (total == 0) return true;
	return false;
}

void msgBoard(String message)
{ //Scrolling text message
	for ( int i = 0 ; i < fontWidth * message.length() + matrix.width() - 1 ; i++ ) {
		matrix.fillScreen(LOW);
		int letter = i / fontWidth;
		int x = (matrix.width() - 1) - i % fontWidth;
		int y = (matrix.height() - 8) / 2; // center the text vertically

		while ( x + fontWidth - spacer >= 0 && letter >= 0 ) {
			if ( letter < message.length() ) {
				matrix.drawChar(x, y, message[letter], HIGH, LOW, 1);
			}
			letter--;
			x -= fontWidth;
		}
		matrix.write(); // Send bitmap to display
		delay(wait);
	} 
}

void sequence()
{ //Doesn't have to be a die - could be any animation
	int i=0;
	int index;
	byte numbers[48]={ //This for nice numbers - not used
		B00011000,
		B00111000,
		B01111000,
		B00011000,
		B00011000,
		B00011000,
		B01111110,
		B00000000,
		B00111100,
		B01100110,
		B01000110,
		B00001100,
		B00011000,
		B00110000,
		B01111110,
		B00000000,
		B00111100,
		B01100110,
		B00000110,
		B00001100,
		B00000110,
		B01100110,
		B00111100,
		B00000000,
		B01100110,
		B01100110,
		B01100110,
		B01111110,
		B00000110,
		B00000110,
		B00000110,
		B00000000,
		B01111110,
		B01100000,
		B01100000,
		B01111100,
		B00000110,
		B01000110,
		B00111100,
		B00000000,
		B00111100,
		B01100110,
		B01100000,
		B01111100,
		B01100110,
		B01100110,
		B00111100,
		B00000000
	};
	byte pips[48]={
		B00000000,
		B00000000,
		B00000000,
		B00011000,
		B00011000,
		B00000000,
		B00000000,
		B00000000,
		B00000000,
		B00000011,
		B00000011,
		B00000000,
		B00000000,
		B11000000,
		B11000000,
		B00000000,
		B00000000,
		B00000011,
		B00000011,
		B00011000,
		B00011000,
		B11000000,
		B11000000,
		B00000000,
		B00000000,
		B11000011,
		B11000011,
		B00000000,
		B00000000,
		B11000011,
		B11000011,
		B00000000,
		B00000000,
		B11000011,
		B11000011,
		B00011000,
		B00011000,
		B11000011,
		B11000011,
		B00000000,
		B00000000,
		B11011011,
		B11011011,
		B00000000,
		B00000000,
		B11011011,
		B11011011,
		B00000000
	};
	matrix.setRotation(1);
	randNumber = random(6);
	// Serial.print("rando: ");  
	// Serial.println(randNumber);
	wipe(); 
	// for ( int i = 0 ; i <= 8; i++ ) {  /To scroll from one number to the next - bit too slow
		for ( int j = 0 ; j <= 7; j++ ) {
			index=(8*randNumber+i+j)%48;
			setRow(0, j, pips[index]);
		}
	//}
	matrix.setRotation(0);
}

void rollDice()
{ //uses drawChar to render number - not used
	randNumber = random(6);
	wipe();
	matrix.drawChar(1, 0, char(49+randNumber), HIGH, LOW, 1);
	matrix.write();
}

void setRow(int addr, int row, byte rowValue)
{
	for (int n=0; n<=7; n++)
	{
	matrix.drawPixel(row, n, bitRead(rowValue, n));
	}
	matrix.write();
}

void magic8ball()
{ 
	String engAnswer[20]={
		"It is certain",
		"It is decidedly so",
		"Without a doubt",
		"Yes, definitely",
		"You may rely on it",
		"As I see it, yes",
		"Most likely",
		"Outlook good",
		"Yes",
		"Signs point to yes",
		"Reply hazy try again",
		"Ask again later",
		"Better not tell you now",
		"Cannot predict now",
		"Concentrate and ask again",
		"Don't count on it",
		"My reply is no",
		"My sources say no",
		"Outlook not so good",
		"Very doubtful"
	};

	randNumber = random(20);
	msgBoard(engAnswer[randNumber]);
}

//EYES Start - based on code from https://github.com/michaltj/LedEyes
void googlyEye()
{
	int times=2;
	byte row = eyePupil;
	matrix.setRotation(1);
	// move to random position, wait random time
	moveEyes(random(MIN, MAX + 1), random(MIN, MAX + 1), 50);
	delay(random(5, 7) * 500);
	
	// blink time?
	if (random(0, 5) == 0)
	{
		delay(500);
		blinkEye();
		delay(500);
	}
	
	// effect time?
	if (EFFECT_ITERATION > 0)
	{
		cntLoop++;
		if (cntLoop == EFFECT_ITERATION)
		{
			cntLoop = 0;
			if (cntEffect > 2) cntEffect = 0;
			switch(cntEffect)
			{
				case 0:
					//roundSpin
					for (int i=0; i<times; i++)
					{
						displayEyes(2, -1); delay(40); if (i==0) delay(40);
						displayEyes(1, -2); delay(40); if (i==0) delay(30);
						displayEyes(0, -2); delay(40); if (i==0) delay(20);
						displayEyes(-1, -2); delay(40);if (i==0) delay(10);
						displayEyes(-2, -1); delay(40);
						displayEyes(-2, 0); delay(40);
						displayEyes(-2, 1); delay(40);if (i==(times-1)) delay(10);
						displayEyes(-1, 2); delay(40);if (i==(times-1)) delay(20);
						displayEyes(0, 2); delay(40); if (i==(times-1)) delay(30);
						displayEyes(1, 2); delay(40); if (i==(times-1)) delay(40);
						displayEyes(2, 1); delay(40); if (i==(times-1)) delay(50);
						displayEyes(2, 0); delay(40);
					}
					break;
				case 1:
					//center row Spin
					moveEyes(0, 0, 50);
					for (int t=0; t<times; t++)
					{
						// spin from center to L
						for (int i=0; i<5; i++)
						{
							row = row >> 1;
							row = row | B10000000;
							setRow(0, 3, row);  
							setRow(0, 4, row); 
							delay(50); 
							if (t == 0) 
								delay((5-i)*10); // increase delay on 1st scroll (speed up effect)
						}
						// spin from R to center
						for (int i=0; i<5; i++)
						{
							row = row >> 1;
							if (i>=2) 
								row = row | B10000000;
							setRow(0, 3, row);
							setRow(0, 4, row);
							delay(50);
							if (t == (times-1)) 
								delay((i+1)*10); // increase delay on last scroll (slow down effect)
						}
					}
					break;
				case 2:
					blinkEye();
					break;
				default: 
					break;
			}
			delay(1000);
			cntEffect++;
		}
	}
	matrix.setRotation(0);
}

void blinkEye()
{
	// close eyelids
	for (int i=0; i<=3; i++)
	{
		setRow(0, i, 0);
		setRow(0, 7-i, 0);
		delay(wait);
	}
	// open eyelids
	for (int i=3; i>=0; i--) 
	{
		setRow(0, i, eyeCurrent[i]);
		setRow(0, 7-i, eyeCurrent[7-i]);
		delay(wait);
	}
}

void displayEyes(int offsetX, int offsetY) 
{
	// ensure offsets are  in valid ranges
	offsetX = constrain(offsetX, MIN, MAX);
	offsetY = constrain(offsetY, MIN, MAX);
	
	// calculate indexes for pupil rows (perform offset Y)
	int row1 = 3 - offsetY;
	int row2 = 4 - offsetY;
	
	// define pupil row
	byte pupilRow = eyePupil;
	
	// perform offset X
	// bit shift and fill in new bit with 1 
	if (offsetX > 0) {
		for (int i=1; i<=offsetX; i++)
		{
			pupilRow = pupilRow >> 1;
			pupilRow = pupilRow | B10000000;
		}
	}
	else if (offsetX < 0) {
		for (int i=-1; i>=offsetX; i--)
		{
			pupilRow = pupilRow << 1;
			pupilRow = pupilRow | B1;
		}
	}
	
	// pupil row cannot have 1s where eyeBall has 0s
	byte pupilRow1 = pupilRow & eyeBall[row1];
	byte pupilRow2 = pupilRow & eyeBall[row2];
	
	// display on LCD matrix, update to eyeCurrent
	for(int r=0; r<8; r++)
	{
		if (r == row1)
		{
			setRow(0, r, pupilRow1);
			eyeCurrent[r] = pupilRow1;
		}
		else if (r == row2)
		{
			setRow(0, r, pupilRow2);
			eyeCurrent[r] = pupilRow2;
		}
		else
		{
			setRow(0, r, eyeBall[r]);
			eyeCurrent[r] = eyeBall[r];
		}
	}
	
	// update current X and Y
	currentX = offsetX;
	currentY = offsetY;
}

void moveEyes(int newX, int newY, int stepDelay)
{
	// set current position as start position
	int startX = currentX;
	int startY = currentY;
	
	newX = constrain(newX, MIN, MAX);
	newY = constrain(newY, MIN, MAX);
	
	// eval steps
	int stepsX = abs(currentX - newX);
	int stepsY = abs(currentY - newY);
	
	// need to change at least one position
	if ((stepsX == 0) && (stepsY == 0))
		return;
	
	// eval direction of movement, # of steps, change per X Y step, perform move
	int dirX = (newX >= currentX) ? 1 : -1;
	int dirY = (newY >= currentY) ? 1 : -1;
	int steps = (stepsX > stepsY) ? stepsX : stepsY;
	int intX, intY;
	float changeX = (float)stepsX / (float)steps;
	float changeY = (float)stepsY / (float)steps;
	for (int i=1; i<=steps; i++)
	{
		intX = startX + round(changeX * i * dirX);
		intY = startY + round(changeY * i * dirY);
		displayEyes(intX, intY);
		delay(stepDelay);
	}
}
//EYES end

void race()
{
	byte randomTrack;
	byte trackChunk;
	int oldTrackPos;
	int oldWidth;
	int minPos;
	int maxPos;
	int randomPos;
	matrix.setRotation(1);
	//Serial.println("in race");

	//Shift screen down
	for ( int i = 7 ; i >= 1; i-- ) {
		track[i]=track[i-1];
	}

	//car in wall now? boom otherwise increment score
	if (bitRead(track[7], carPosition) == HIGH ) {
		matrix.setRotation(0);
		msgBoard("Score: "+ (String) raceScore);
		for ( int i = 0 ; i <= 7; i++ ) {
			track[i]=0;
		}
		raceScore=0;
		inGame=false;
		return;
	} else {
		raceScore++;
	}
	oldWidth = trackWidth;
	trackWidth = constrain(random(2) + 2 ,2,4);//Should be between 2 and 4

	oldTrackPos = trackPos;
	minPos=constrain(oldTrackPos-round(min(trackWidth,oldWidth)/2-.5),1,7-trackWidth);
	maxPos=oldTrackPos+round(min(trackWidth,oldWidth)/2+.5);
	randomPos=random(maxPos-minPos)+minPos;
	trackPos = constrain(randomPos,minPos,maxPos);
	trackPos = constrain(trackPos,1,8-trackWidth);
	
	//Position new track opening
	trackChunk= 1 << trackWidth;
	trackChunk--;
	trackChunk = trackChunk << constrain(trackPos,1,7-trackWidth);  
	track[0] = trackChunk ^ B11111111;
	//draw screen
	for ( int i = 0 ; i <= 7; i++ ) {
		setRow(0,i,track[i]);
	}
	//draw car position
	matrix.drawPixel(7, carPosition, HIGH);
	matrix.write();
	delay(500-constrain(raceScore,0,500)); //Get faster the longer it's played
}