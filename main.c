/************** ECE2049 Lab #1 *********************/
/*************  07 September 2022  *****************/
/***************************************************/

#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include "peripherals.h"

typedef struct Alien //struct for aliens to be drawn
{
    unsigned char key;
    int x;
    int y;
    bool visible;
} Alien;

typedef enum State //case states
{
    Welcome,
    Startgame,
    Countdown,
    Countdownscreen,
    Nextlevelscreen,
    Playlevel,
    Gameplay,
    Lossscreen
} State;

// Function Prototypes
void drawWelcome();
void drawNextLevel(int);
bool drawCountdown(int, int);
void drawAliens(Alien*, int);
void drawLoss();



// Main function
void main(void)
{
    int i;
    int levelBottom = 90; // The lowest the top of the alien numerical character can go before you lose the level
    unsigned char keyPressed = 0, lastKeyPressed = 0;
    unsigned long int mainCounter = 0, auxCounter = 0, auxCounter2 = 0;
    Alien aliens[50];
    int alienCount = 0;
    int level = 1;
    State state = Welcome;

    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer

    initLeds();
    configDisplay();
    configKeypad();

    while (1)
    {
        keyPressed = getKey();
        switch (state)
        {

        case Welcome:
        {
            drawWelcome();
            state = Startgame;
            break;
        }

        case Startgame:
        {
            if (keyPressed == '*')
            {
                state = Countdown;
                BuzzerOff();
            }
            break;
        }
        case Countdown:
        {
                    auxCounter = mainCounter;
                    state = Countdownscreen;
                }
                case Countdownscreen:
                {
                    if (drawCountdown(mainCounter, auxCounter))
                    {
                        state = Playlevel;
                    }
                    break;
                }
                case Nextlevelscreen:
                {
                    drawNextLevel(level);
                    state = Startgame;
                    break;
                }
                case Playlevel:
                {
                    BuzzerOff();
                    // Create aliens based on level number, minimum of 1 alien, maximum of 30 aliens
                    alienCount = ((rand() % (level * level)) + 1) % 30;

                    // Height tracker is used to tell if there is already an alien in the row
                    int heightTracker[5] = { 0, 0, 0, 0, 0 };

                    int currentRow = 0;

                    // Determine where aliens will be drawn
                    for (i = 0; i < alienCount; i++)
                    {
                        int currentColumn = rand() % 5;

                        if (heightTracker[currentColumn])
                        {
                            // There's already an alien in this row of this column
                            currentRow++;
                            int j;
                            for (j = 0; j < 5; j++)
                            {
                                heightTracker[j] = 0;
                            }
                        }
                        heightTracker[currentColumn] = heightTracker[currentColumn] + 1;  // Assign aliens their number and space them out
                        aliens[i].x = 15 * (currentColumn + 1) + 3;
                        aliens[i].y = 10 - (20 * currentRow);
                        aliens[i].key = "01234"[currentColumn];
                        aliens[i].visible = true;
                    }
                    state = Gameplay;
                    break;
                }
                case Gameplay:
                {
                    Graphics_clearDisplay(&g_sContext);

                    // Check if key is pressed to destroy each alien
                    // If yes, destroy the lowest one with the pressed key
                    // If all aliens are destroyed, increment level counter and move to Playlevel
                    if (keyPressed != 0 && lastKeyPressed != keyPressed)
                    {
                        for (i = 0; i < alienCount; i++)
                        {
                            Alien currentAlien = aliens[i];
                            if (currentAlien.visible && currentAlien.key == keyPressed)
                            {
                                //direct access to the struct in the array
                                aliens[i].visible = false;
                                auxCounter2 = 5;

                                bool anyVisible;

                                for (i = 0, anyVisible = false; i < alienCount; i++)
                                {
                                    if (aliens[i].visible)
                                    {
                                        anyVisible = true;
                                        break;
                                    }
                                }
                                // Advance the level once all aliens have been destroyed
                                if (!anyVisible)
                                {
                                    level++;
                                    state = Nextlevelscreen;
                                }
                                break;
                            }
                        }
                    }

                    // If an alien reaches the bottom change state to lossscreen
                    for (i = 0; i < alienCount; i++)
                    {
                        Alien currentAlien = aliens[i];
                        if (currentAlien.visible)
                        {
                            if (currentAlien.y >= levelBottom)
                            {
                                state = Lossscreen;
                            }
                            aliens[i].y += level / 2 + 1; // Aliens decend faster depending on level
                        }
                    }

                    drawAliens(aliens, alienCount);
                    break;
                }
                case Lossscreen:
                {
                    drawLoss();
                    BuzzerOn();
                    state = Startgame;
                    level = 1; // Restart game at level 1
                    break;
                }
                }
                mainCounter++;
                if (auxCounter2 > 0)
                {
                    auxCounter2--;
                    if(!auxCounter2)
                        BuzzerOff();
                }
                lastKeyPressed = keyPressed;

    }
}

void drawWelcome() //Intro Screen
{

    Graphics_clearDisplay(&g_sContext);                // Clear the display

    Graphics_drawStringCentered(&g_sContext, "Space", AUTO_STRING_LENGTH, 48,
                                25, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Invaders", AUTO_STRING_LENGTH, 48,
                                35, TRANSPARENT_TEXT);

    Graphics_drawStringCentered(&g_sContext, "Press *", AUTO_STRING_LENGTH, 48,
                                70, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "to Start", AUTO_STRING_LENGTH, 48,
                                80, TRANSPARENT_TEXT);

    Graphics_Rectangle box = { .xMin = 5, .xMax = 91, .yMin = 5, .yMax = 91 };
    Graphics_drawRectangle(&g_sContext, &box);


    Graphics_flushBuffer(&g_sContext);//Flush display to show message
}

void drawLoss() //Draw loss screen when alien reaches bottom
{
    Graphics_clearDisplay(&g_sContext);                // Clear the display

    Graphics_drawStringCentered(&g_sContext, "Game Over", AUTO_STRING_LENGTH,
                                48, 25, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, ":'-(", AUTO_STRING_LENGTH, 48, 35,
    TRANSPARENT_TEXT);

    Graphics_drawStringCentered(&g_sContext, "Press *", AUTO_STRING_LENGTH, 48,
                                70, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "to play again", AUTO_STRING_LENGTH,
                                48, 80, TRANSPARENT_TEXT);

    Graphics_Rectangle box = { .xMin = 5, .xMax = 91, .yMin = 5, .yMax = 91 };
    Graphics_drawRectangle(&g_sContext, &box);

    Graphics_flushBuffer(&g_sContext);
}

void drawNextLevel(int level)
{

    Graphics_clearDisplay(&g_sContext);

    char buffer[10];
    snprintf(buffer, 9, "Level %d", level);

    Graphics_drawStringCentered(&g_sContext, buffer, AUTO_STRING_LENGTH, 48, 35,TRANSPARENT_TEXT);

    Graphics_drawStringCentered(&g_sContext, "Press *", AUTO_STRING_LENGTH, 48,
                                70, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "to Begin", AUTO_STRING_LENGTH, 48,
                                80, TRANSPARENT_TEXT);

    // Draw a box around everything because it looks nice
    Graphics_Rectangle box = { .xMin = 5, .xMax = 91, .yMin = 5, .yMax = 91 };
    Graphics_drawRectangle(&g_sContext, &box);

    Graphics_flushBuffer(&g_sContext);
}

bool drawCountdown(int mainCounter, int auxCounter) //function for intial countdown screen
{
    if (auxCounter == mainCounter)
    {
        Graphics_clearDisplay(&g_sContext);
        Graphics_drawStringCentered(&g_sContext, "3", AUTO_STRING_LENGTH, 48,
                                    50, TRANSPARENT_TEXT);
        Graphics_flushBuffer(&g_sContext);
    }
    else if (mainCounter == auxCounter + 4000)
    {
        Graphics_clearDisplay(&g_sContext);
        Graphics_drawStringCentered(&g_sContext, "2", AUTO_STRING_LENGTH, 48,
                                    50, TRANSPARENT_TEXT);
        Graphics_flushBuffer(&g_sContext);
    }
    else if (mainCounter == auxCounter + 8000)
    {
        Graphics_clearDisplay(&g_sContext);
        Graphics_drawStringCentered(&g_sContext, "1", AUTO_STRING_LENGTH, 48,
                                    50, TRANSPARENT_TEXT);
        Graphics_flushBuffer(&g_sContext);
    }
    else if (mainCounter == auxCounter + 12000)
    {
        Graphics_clearDisplay(&g_sContext);
        Graphics_flushBuffer(&g_sContext);
        return true;
    }
    return false;
}

void drawAliens(Alien* aliens, int alienCount) //Draws aliens based on the parameters entered when the function is called
{
    int i;
    for (i = 0; i < alienCount; i++)
    {
        Alien currentAlien = aliens[i];
        if (currentAlien.visible && currentAlien.y >= 0)
        {
            unsigned char* str = &(currentAlien.key);
            Graphics_drawStringCentered(&g_sContext, str, 1, currentAlien.x,
                                        currentAlien.y, TRANSPARENT_TEXT);
        }
    }
    Graphics_flushBuffer(&g_sContext);
}
