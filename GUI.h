#ifndef GUI_H_
#define GUI_H_

#include <stdio.h>
#include <stdlib.h>

/*
 * Importeert de nodige functies uit SDL2.
 */
#include <SDL2/SDL.h>

/*
 * De hoogte en breedte van het venster (in pixels).
 * Deze dimensies zijn arbitrair gekozen. Deze dimensies hangen mogelijk af van de grootte van het speelveld.
 */

int height = 8, width = 8;

#define WINDOW_HEIGHT (height * IMAGE_HEIGHT)
#define WINDOW_WIDTH (width * IMAGE_WIDTH)

/*
 * De hoogte en breedte (in pixels) van de afbeeldingen voor de vakjes in het speelveld die getoond worden.
 * Als je andere afbeelding wil gebruiken in je GUI, zorg er dan voor dat deze
 * dimensies ook aangepast worden.
 */
#define IMAGE_HEIGHT 50
#define IMAGE_WIDTH 50

#endif /* GUI_H_ */
