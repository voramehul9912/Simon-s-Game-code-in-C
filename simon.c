#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <DAQlib.h>
#include <windows.h>

// #include <DAQlib.h>

#define ON 1
#define OFF 0

#define GreenLED 0
#define RedLED 1
#define YellowLED 2
#define BlueLED 3

#define GreenButton 0
#define RedButton 1
#define YellowButton 2
#define BlueButton 3
//sleeping time
#define delay_time 1000
//maximum length of the sequence
#define MAX_LENGTH 5
//functions to be implemented
//function to generate a random number between lower and upper limit
int randInt(int lower, int upper);
//function to generate a sequence of random integers of length = len and to store in data.
void generateSequence(int len, int data[]);
//function to catch an input of the button pressed
int getButtonPressed(void);
//function to run the simulation of the Simon's game.
void runSimon(void);
//function to blink an LED three times.
void zzz(int LED);
//main function.
int main(void) {
	//to call setupNum=6 from DAQlib
	int setupNum = 6;
	//Printing the instructions to play the game.
	printf("This is simon's game.\n");
	printf("Please follow these instructons to play the game:\n");
	printf("1.first all the LEDs will blink once.\n");
	printf("2.Then the sequence of LEDs will blink. You have to remember the sequence.\n");
	printf("3.Then input the sequence by pressing the corresponding buttons.\n");
	printf("4.If you insert the sequence correctly, the GREEN LED will blink 3 times and the length of the sequence will increase by one.\n");
	printf("5.If you insert incorrectly, RED LED will blink 3 times and the game will restart i.e, the sequence will start from 1.\n");
	printf("If you insert the correct sequence %d times, you WIN the series and then the game restarts.\n", MAX_LENGTH);
	printf("PRESS ANY KEY TO PLAY THE GAME.\n");
	getchar();
	if (setupDAQ(setupNum) == TRUE) {
		Sleep(delay_time);
		runSimon();
	}
	system("PAUSE");
	return 0;
}

void runSimon(void) {
	boolean won = TRUE;
	//exit_val==0 means incorrect sequence, exit_val==1 means correct sequence
	boolean play = TRUE;
	while (continueSuperLoop() == TRUE) {
		/* Test that it does enter the Super Loop, and turns each light on, and then off after 1 second, one by one. */
		for (int count = 0; count <= 3; count++) {
			digitalWrite(count, ON);
			Sleep(delay_time);
			digitalWrite(count, OFF);
		}
		while (play = TRUE) {
			int len = 1;
			while (len <= MAX_LENGTH) {
				int exit_val = 0;
				//generating a sequence of length MAX_LENGTH
				int data[MAX_LENGTH];
				generateSequence(MAX_LENGTH, data);
				//Blinking the LEDs one by one according to the integers generated in the sequence.
				for (int i = 0; i < len; i++) {
					digitalWrite(data[i], ON);
					Sleep(delay_time);
					digitalWrite(data[i], OFF);
					Sleep(delay_time / 2);
				}
				//Getting the input of the sequence.
				for (int i = 0; i < len; i++) {
					int pressed = getButtonPressed();
					digitalWrite(pressed, ON);
					Sleep(delay_time / 2);
					digitalWrite(pressed, OFF);
					if (pressed != data[i]) {
						exit_val = 1;
					}
				}
				//Blinking GreenLED if user inserts the sequence correctly.
				if (exit_val == 0) {
					Sleep(delay_time / 2);
					zzz(GreenLED);
					Sleep(delay_time);
				}
				//Blinking RedLED 3 times if user inserts the sequence incorrectly.
				else {
					Sleep(delay_time / 2);
					zzz(RedLED);
					Sleep(delay_time);
					len = 0;
					won = FALSE;
				}
				//Incrementing the length of the sequence.
				len++;
			}
			if (won == TRUE) {
				play = TRUE;
			}
			else {
				play = FALSE;
			}
		}
	}
}

int randInt(int lower, int upper) {
	return rand() % upper + lower;
}

void generateSequence(int len, int data[]) {
	srand(clock());
	for (int i = 0; i < len; i++)
		data[i] = randInt(0, 4);
}

void zzz(int LED) {
	digitalWrite(LED, ON);
	Sleep(delay_time / 3);
	digitalWrite(LED, OFF);
	Sleep(delay_time / 3);
	digitalWrite(LED, ON);
	Sleep(delay_time / 3);
	digitalWrite(LED, OFF);
	Sleep(delay_time / 3);
	digitalWrite(LED, ON);
	Sleep(delay_time / 3);
	digitalWrite(LED, OFF);
	Sleep(delay_time / 3);
}

int getButtonPressed(void) {
	int count;
	int returnVal;
	for (count = 0; count < 50; count++) {
		if (digitalRead(GreenButton) == TRUE) {
			returnVal = GreenButton;
			count = 100;
		}
		else if (digitalRead(RedButton) == TRUE) {
			returnVal = RedButton;
			count = 100;
		}
		else if (digitalRead(YellowButton) == TRUE) {
			returnVal = YellowButton;
			count = 100;
		}
		else if (digitalRead(BlueButton) == TRUE) {
			returnVal = BlueButton;
			count = 100;
		}
		else
			returnVal = 4;
	}
	return returnVal;
}