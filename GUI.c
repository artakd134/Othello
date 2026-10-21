#include "GUI.h"

int current = 1; // huidige kleur aan de beurt: 1 = zwart , 2 = wit

/*
 * De renderer wordt gebruikt om figuren of sprites te tekenen in het venster.
 * De renderer wordt geïnitialiseerd in de initialize_window-functie.
 */

static SDL_Renderer *renderer;

/*
 * Deze texture verwijst naar een van de BMP afbeeldingen die getekend kunnen worden.
 */
static SDL_Texture *black_stone_texture = NULL;
static SDL_Texture *white_stone_texture = NULL;
static SDL_Texture *empty_stone_texture = NULL;

/*
 * Dit is het venster dat getoond zal worden en waarin het speelveld wordt weergegeven.
 * Dit venster wordt aangemaakt bij het initialiseren van de GUI en wordt weer afgebroken
 * wanneer het spel ten einde komt.
 */
static SDL_Window *window;


/*
 * Onderstaande twee lijnen maken deel uit van de minimalistische voorbeeldapplicatie:
 * ze houden de laatste positie bij waar de gebruiker geklikt heeft.
 */
int mouse_x = 0;
int mouse_y = 0;

/*
 * Geeft aan of de applicatie moet verdergaan.
 * Dit is waar zolang de gebruiker de applicatie niet wilt afsluiten door op het kruisje te klikken.
 */
int should_continue = 1;

/*
 * SDL genereert heel veel gebruikersevents terwijl de applicatie wordt uitgevoerd,
 * maar veel van die events zijn helemaal niet belangrijk voor de rest van de applicatie.
 * Deze functie filtert de belangrijke van de niet-belangrijke events, zodat het behandelen
 * van de niet-belangrijke events de applicatie niet vertraagt.
 *
 * Controleert of het gegeven SDL event van belang is voor deze applicatie of niet.
 * Geeft een 1 terug als het gegeven event een muisklik-, een keypress,
 * of een quit-event (gebruiker sluit het venster af) is. Geeft een 0 terug als de
 * gegeven pointer NULL is of als het event waarnaar verwezen wordt niet belangrijk
 * is voor de applicatie.
 */
int is_relevant_event(SDL_Event *event) {
	if (event == NULL) {
		return 0;
	}
	return  (event->type == SDL_MOUSEBUTTONDOWN) ||
			(event->type == SDL_KEYDOWN) ||
			(event->type == SDL_QUIT);
}
/*
 * Vangt de input uit de GUI op. Deze functie is al deels geïmplementeerd, maar je moet die zelf
 * nog afwerken. Je mag natuurlijk alles aanpassen aan deze functie, inclusief return-type en argumenten.
 */
void read_input() {
	SDL_Event event;

	/*
	 * Handelt alle input uit de GUI af.
	 * Telkens de speler een input in de GUI geeft (bv. een muisklik, muis bewegen, toets indrukken enz.)
	 * wordt er een 'event' (van het type SDL_Event) gegenereerd dat hier wordt afgehandeld.
	 *
	 * Niet al deze events zijn relevant voor jou: als de muis bv. gewoon wordt bewogen, hoef
	 * je niet te reageren op dit event.
	 * We gebruiken daarom de is_relevant_event-functie die niet-gebruikte events wegfiltert.
	 *
	 * Zie ook https://wiki.libsdl.org/SDL_PollEvent
	 */

	while (!SDL_PollEvent(&event) || !is_relevant_event(&event)) {}
	switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_o:
				printf("Turn skipped!\n");
				if (current == 1) {current = 2;}
						else {current = 1;}
				break;
			case SDLK_q:
				printf("Game exited\n");
				should_continue = 0;
				break;
			}
			break;
		case SDL_QUIT:
			/* De gebruiker heeft op het kruisje van het venster geklikt om de applicatie te stoppen. */
			should_continue = 0;
			break;

		case SDL_MOUSEBUTTONDOWN:
			/*
			 * De speler heeft met de muis geklikt: met de onderstaande lijn worden de coördinaten in het
			 * het speelveld waar de speler geklikt heeft bewaard in de variabelen mouse_x en mouse_y.
			 */
			mouse_x = event.button.x;
			mouse_y = event.button.y;
			printf("Clicked on position (%i,%i)\n", mouse_x, mouse_y);
			break;
		}
}

void clear_window() {
	/*
	 * Maakt van wit de achtergrondkleur.
	 */
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	/*
	 * Reset het venster en verandert het naar de kleur die op de lijn hierboven werd vastgelegd.
	 */
	SDL_RenderClear(renderer);
}

/*
 * Tekent de gegeven afbeelding, met als dimensies de gegeven breedte en hoogte, op de gegeven x-y positie.
 */
void draw_rectangle(SDL_Texture *image, int x, int y, int width, int height) {
	SDL_Rect rectangle = { x, y, width, height };
	/* Tekent de afbeelding op die plaats. */
	SDL_RenderCopy(renderer, image, NULL, &rectangle);
}

/*
 Tekent het volledige venster.
*/

// Maak een array om alle zwarte/witte tiles op te slaan, samen met hun coordinaten
typedef struct {
    int x;
    int y;
    int color;
} Tile;

typedef struct {
    Tile *tiles;
    size_t size;
} TileArray;

void makeTileArray (TileArray* array) {
	array->tiles = malloc(width * height * sizeof(Tile));
	array->size = 0;
}

void freeTileArray (TileArray* array) {
	free(array->tiles);
}

void addTile(TileArray* array, int x, int y, int color) {
    if (array->size < (width * height)) {
    	 array->tiles[array->size++] = (Tile){x, y, color};
    }
}
// Loop iteratief door de array en teken alle tiles in de juiste kleur, totdat de array volledig doorlopen is.
void drawTileArray(const TileArray* array) {
    size_t i = 0;
    while (i < array->size) {
        if (array->tiles[i].color == 1)
            draw_rectangle(black_stone_texture, array->tiles[i].x * IMAGE_WIDTH, array->tiles[i].y * IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT);
        else
            draw_rectangle(white_stone_texture, array->tiles[i].x * IMAGE_WIDTH, array->tiles[i].y * IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT);
        i++;
    }
}

// Loop iteratief door de array en check of er een bepaald x- en y- coordinaat in voorkomt. Als ja, geef de kleur (1 of 2) weer
int memberArray (const TileArray* array, int x, int y) {
	size_t i = 0;
    while (i < array->size) {
    	if (x == array->tiles[i].x && y == array->tiles[i].y)
    			return array->tiles[i].color;
    	else i++;
    }
    return 0;
}

TileArray myTileArray = {0};

int ChangeTiles(TileArray* array, int color) { // Check of een tile geplaatst mag worden, indien wel verander de tiles in de juiste rijen.
    int dx = -1;
    int Changed = 0;

    while (dx <= 1) {
        int dy = -1;
        while (dy <= 1) {
            if (dx == 0 && dy == 0) {
                dy++;
                continue;
            }

            int newX = mouse_x / IMAGE_WIDTH + dx, newY = mouse_y / IMAGE_HEIGHT + dy;

            // Kijk voor elke positie na of er een tile vd andere kleur op staat
            if (memberArray(&myTileArray, newX, newY) != 0 && memberArray(&myTileArray, newX, newY) !=  color) {
                int currX = newX, currY = newY;
                while (memberArray(&myTileArray, newX, newY) != 0 && memberArray(&myTileArray, newX, newY) != color) {
                    newX = newX + dx, newY = newY + dy;
                }

                if (memberArray(&myTileArray, newX, newY) == color) {
                    // Ga terug in omgekeerde richting (-dx -dy) en verander alles naar de omgekeerde kleur, totdat je bij de oorspronkelijke positie komt
                    while (!(newX == currX && newY == currY)) {
                        newX = newX - dx;
                        newY = newY - dy;
                        size_t i;
                        for (i = 0; i < array->size; ++i) {
                            if (array->tiles[i].x == newX && array->tiles[i].y == newY) {
                                array->tiles[i].color = color;
                            }
                        }
                        Changed = 1;
                    }
                }
            }
            dy++;
        }
        dx++;
    }
    return Changed;
}

void CountTiles (TileArray* array) // Tel aantal zwarte en witte tiles, en als het spel gedaan is geef een bericht terug in de console
{ size_t i = 0;
int BlackTiles = 0, WhiteTiles = 0;
while (i < array->size) {
		if (array->tiles[i].color == 1) BlackTiles++;
		else WhiteTiles++;
	 i++; }
	 printf ("Black tiles = %i\n", BlackTiles);
	 printf ("White tiles = %i\n", WhiteTiles);
	 if (BlackTiles == 0) printf ("BLACK WINS: No white tiles remaining\n");
	 if (WhiteTiles == 0) printf ("WHITE WINS: No black tiles remaining\n");
	 if (BlackTiles + WhiteTiles == width * height && BlackTiles > WhiteTiles) printf ("BLACK WINS: Board is full\n");
	 if (BlackTiles + WhiteTiles == width * height && WhiteTiles > BlackTiles) printf ("WHITE WINS: Board is full\n");
	 if (BlackTiles + WhiteTiles == width * height && BlackTiles == WhiteTiles) printf ("TIE: Board is full\n");
}

int write(TileArray* array) // Schrijf de huidige toestand en spelbord neer in het textbestand field.txt
{   FILE *file = fopen("field.txt", "w");

    fprintf(file, "%i\n", width); fprintf(file, "%i\n", height);
    if (current == 1) {fprintf(file, "B\n");}
    else {fprintf(file, "W\n");}
    {	int x = 0, y = 0;
    		    while (height > y) {
    		    	x = 0;
    				while (width > x) {
    					if (memberArray (&myTileArray, x, y) == 0) {fprintf(file, "X");}
    					if (memberArray (&myTileArray, x, y) == 1) {fprintf(file, "B");}
    					if (memberArray (&myTileArray, x, y) == 2) {fprintf(file, "W");}
    					x++;
    				}
    				fprintf (file, "\n"),
    				y++;
    		    }
    	}
    fclose(file);
}

void read (TileArray* array) // Lees de huidige toestand en sla het op op het spelbord.
{    int bufferLength = height + 3;
	 char buffer [bufferLength];
	 FILE *pointer = fopen("field.txt", "r");
	 while (fgets(buffer, bufferLength, pointer))
		 printf("%s", buffer);
	 fclose(pointer);

}

void draw_window() {
	clear_window();
	// Een iteratieve functie om de background tiles te tekenen, en de 2 middenste witte en zwarte tiles.
	{
			int x = 0, y = 0;
		    while (height > y) {
		    	x = 0;
				while (width > x) {
					draw_rectangle(empty_stone_texture, (x * IMAGE_WIDTH), (y * IMAGE_HEIGHT), IMAGE_WIDTH, IMAGE_HEIGHT);
					x++;
				}
				y++;
		    }
	}
	if (!memberArray (&myTileArray, width / 2, height / 2)) {
		addTile (&myTileArray, width / 2 - 1, height / 2 - 1, 2);
		addTile (&myTileArray, width / 2, height / 2, 2);
		addTile (&myTileArray, width / 2 - 1, height / 2, 1);
		addTile (&myTileArray, width / 2 , height / 2 - 1, 1); }
  // Teken de tile van de juiste kleur, en wissel van current (kleur)
	if (current == 3) current = 2;
	if (memberArray (&myTileArray, mouse_x / IMAGE_WIDTH, mouse_y / IMAGE_HEIGHT) == 0)// je mag een tile niet op een andere tile plaatsen
	{if (current == 1 && ChangeTiles(&myTileArray, 1))
	{
	addTile(&myTileArray, mouse_x / IMAGE_WIDTH, mouse_y / IMAGE_HEIGHT, 1);
	current = 3;
	printf("Current player: White\n");
	}
	if (current == 2 && ChangeTiles (&myTileArray, 2))
	{
	addTile(&myTileArray, mouse_x / IMAGE_WIDTH, mouse_y / IMAGE_HEIGHT, 2);
	current = 1;
	printf("Current player: Black\n");
	}
	CountTiles (&myTileArray);
	write(&myTileArray);
	}

	/*
	 * De volgende lijn moet zeker uitgevoerd worden op het einde van de functie.
	 * Wanneer aan de renderer gevraagd wordt om iets te tekenen, wordt het venster
	 * pas aangepast wanneer de SDL_RenderPresent-functie wordt aangeroepen.
	 */

	drawTileArray (&myTileArray);

	SDL_RenderPresent(renderer);
}


/*
 * Initialiseert het venster en alle andere structuren die gebruikt worden door de GUI.
 */
void initialize_window(const char *title) {
	/*
	 * Code o.a. gebaseerd op http://lazyfoo.net/tutorials/SDL/02_getting_an_image_on_the_screen/index.php
	 */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error while initializing SDL: %s\n", SDL_GetError());
		exit(1);
	}

	/*
	 * Maakt het venster aan met gevraagde dimensies.
	 * De titel van het venster zal overeenkomen met de waarde van de title-string.
	 */
	window = SDL_CreateWindow(title, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Error: %s\n", SDL_GetError());
		exit(1);
	}

	/* Initialiseert de renderer. */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
}

/*
 * Laadt alle afbeeldingen die getoond moeten worden in.
 */
void initialize_textures() {
	/*
	 * Laadt de afbeeldingen in. In deze minimalistische applicatie laden we slechts 1 afbeelding in.
	 * Indien de afbeelding niet kon geladen worden (bv. omdat het pad naar de afbeelding verkeerd is),
	 * geeft SDL_LoadBMP een NULL-pointer terug.
	 */
	SDL_Surface *black_stone_surface = SDL_LoadBMP("Images/black.bmp");
	SDL_Surface *white_stone_surface = SDL_LoadBMP("Images/white.bmp");
	SDL_Surface *empty_stone_surface = SDL_LoadBMP("Images/empty.bmp");


	/*
	 * Zet deze afbeelding om naar een texture die getoond kan worden in het venster.
	 * Indien de texture niet kon omgezet worden, geeft de functie een NULL-pointer terug.
	 */
	black_stone_texture = SDL_CreateTextureFromSurface(renderer, black_stone_surface);
	white_stone_texture = SDL_CreateTextureFromSurface(renderer, white_stone_surface);
	empty_stone_texture = SDL_CreateTextureFromSurface(renderer, empty_stone_surface);

	/* Dealloceer het SDL_Surface dat werd aangemaakt. */
	SDL_FreeSurface(black_stone_surface);
	SDL_FreeSurface(white_stone_surface);
	SDL_FreeSurface(empty_stone_surface);
}

/*
 * Initialiseert onder het venster waarin het speelveld getoond zal worden, en de texture van de afbeelding die getoond zal worden.
 * Deze functie moet aangeroepen worden aan het begin van het spel, vooraleer je de spelwereld begint te tekenen.
 */
void initialize_gui() {
	initialize_window("Othello");
	initialize_textures();
}

/*
 * Dealloceert alle SDL structuren die dynamisch gealloceerd werden.
 */
void free_gui() {
	/* Dealloceert de ingeladen afbeelding. */
	SDL_DestroyTexture(black_stone_texture);
	SDL_DestroyTexture(white_stone_texture);
	SDL_DestroyTexture(empty_stone_texture);
	/* Dealloceert de renderer. */
	SDL_DestroyRenderer(renderer);
	/* Dealloceert het venster. */
	SDL_DestroyWindow(window);
	/* Stopt SDL. */
	SDL_Quit();
}

int main(int argc, char *argv[]) {
	freeTileArray (&myTileArray);
	makeTileArray (&myTileArray);
	// Om het spel te runnen met bepaalde argumenten: argc is het aantal argumenten, argv is de value van een argument
	if (argc == 3) {
		if (strcmp(argv[1], "-w" == 0)) {width = atoi(argv[2]);} // -w x
		if (strcmp(argv[1], "-h" == 0)) {height = atoi(argv[2]);} } // -h y
	//	if (strcmp(argv[1], "-f" == 0))
		//{read (&myTileArray);}

	if (argc == 5 && strcmp(argv[1], "-w") == 0 && strcmp(argv[3], "-h") == 0) { // -w x h y
	        width = atoi(argv[2]), height = atoi(argv[4]);
	} else {printf("Invalid arguments\n");}

	initialize_gui();

	while (should_continue) {
		draw_window();
		read_input();
	}

	/* Dealloceert al het geheugen dat werd aangemaakt door SDL zelf. */
	free_gui();
	return 0;
}
