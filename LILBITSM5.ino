//The Game of Life, also known simply as Life, is a cellular automaton
//devised by the British mathematician John Horton Conway in 1970.
// https://en.wikipedia.org/wiki/Conway's_Game_of_Life

#include <M5StickC.h>

//#define GRIDX 80
//#define GRIDY 60
//#define CELLXY 4
#define TFT_GREY 0x1010
#define TFT_ORANGE 0xFFA5
#define GRIDX 80
#define GRIDY 40
#define CELLXY 2

#define GEN_DELAY 60

//Current grid
uint8_t grid[GRIDX][GRIDY];

//The new grid for the next generation
uint8_t newgrid[GRIDX][GRIDY];

//Number of generations
#define NUMGEN 1000

uint16_t genCount = 0;

                      void setup()   {
                      
                        //Set up the display
                        M5.begin();
                        M5.Lcd.setRotation(3);
                        M5.Lcd.fillScreen(TFT_RED);
                        M5.Lcd.setTextSize(1);
                        M5.Lcd.setTextColor(TFT_WHITE);
                        M5.Lcd.setCursor(0, 0);
                       M5.Axp.EnableCoulombcounter();
                      }

double vbat = 0.0;
int discharge,charge;
double temp = 0.0;
double bat_p = 0.0;
extern const unsigned char gImage_logo[];
void loop() {
 M5.Lcd.fillScreen(WHITE);
  M5.Lcd.drawBitmap(0, 0, 80, 160,(uint16_t *)gImage_logo);
  M5.Lcd.setCursor(35, 27);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.println(F("M5-STICKC"));
  delay(2000);
  //Display a simple splash screen
  M5.Lcd.fillRect(0, 0, 160, 80, 0xFFFF);
  M5.Lcd.fillRect(4, 4, 150, 75, 0x0100);
  M5.Lcd.fillScreen(BLUE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_WHITE,BLUE);
  M5.Lcd.setCursor(45, 7);
  M5.Lcd.println(F(" LIL' "));
  M5.Lcd.setCursor(35, 27);
  M5.Lcd.println(F("  BITS  "));
  M5.Lcd.setCursor(35, 47);
  M5.Lcd.println(F("Automata"));
  M5.Lcd.setCursor(20, 65);
  M5.Lcd.println(F("tEaM-kIsMeT"));
  delay(3000);
 
  M5.Lcd.fillScreen(random(0xFFFF));

  initGrid();

  genCount = NUMGEN;

  drawGrid();

  //Compute generations
  for (int gen = 0; gen < genCount; gen++)
  {
    if(digitalRead(M5_BUTTON_HOME) == LOW){initGrid();gen = 0; M5.Lcd.setTextSize(2); M5.Lcd.fillScreen(TFT_BLACK); M5.Lcd.setTextColor(TFT_WHITE,TFT_GREY); M5.Lcd.setCursor(35, 27);
    M5.Lcd.println(F("  NUKE  "));delay(GEN_DELAY*random(20)); M5.Lcd.fillScreen(random(0xFFFF));}
    if(digitalRead(M5_BUTTON_RST) == LOW){statusSys();M5.Lcd.setRotation(3);}
  vbat      = M5.Axp.GetVbatData() * 1.1 / 1000;
  M5.Lcd.setTextColor(TFT_WHITE,TFT_GREY);
    M5.Lcd.setCursor(65, 0);
    M5.Lcd.setTextSize(1);
    M5.Lcd.println(gen);
     M5.Lcd.setCursor(60, 70);
    M5.Lcd.setTextSize(1);
    M5.Lcd.printf("%.3fV\r\n",vbat);
    if (vbat <= 3.8 ){M5.Lcd.setCursor(40, 30);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("FEED ME");}
    
    
    delay(GEN_DELAY+random(40));
    
    computeCA();
    drawGrid();
    
    for (int16_t x = 1; x < GRIDX-1; x++) {
      for (int16_t y = 1; y < GRIDY-1; y++) {
        grid[x][y] = newgrid[x][y];
      }
    }

  }
}

//Draws the grid on the display
void drawGrid(void) {

  uint16_t color = TFT_RED;
  for (int16_t x = 1; x < GRIDX - 1; x++) {
    for (int16_t y = 1; y < GRIDY - 1; y++) {
      if ((grid[x][y]) != (newgrid[x][y])) {
        if (newgrid[x][y] == 1) color = random(0xFFFF); //random(0xFFFF);
        else color = 0;
        M5.Lcd.fillRect(CELLXY * x, CELLXY * y, CELLXY, CELLXY, color);
      }
    }
  }
}

//Initialise Grid
void initGrid(void) {
  M5.Lcd.setRotation(3);
  for (int16_t x = 0; x < GRIDX; x++) {
    for (int16_t y = 0; y < GRIDY; y++) {
      newgrid[x][y] = 0;

      if (x == 0 || x == GRIDX - 1 || y == 0 || y == GRIDY - 1) {
        grid[x][y] = 0;
      }
      else {
        if (random(3) == 1)
          grid[x][y] = 1;
        else
          grid[x][y] = 0;
      }

    }
  }
}

//Compute the CA. Basically everything related to CA starts here
void computeCA() {
  for (int16_t x = 1; x < GRIDX; x++) {
    for (int16_t y = 1; y < GRIDY; y++) {
      int neighbors = getNumberOfNeighbors(x, y);
      if (grid[x][y] == 1 && (neighbors == 2 || neighbors == 3 ))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 1)  newgrid[x][y] = 0;
      if (grid[x][y] == 0 && (neighbors == 3))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 0) newgrid[x][y] = 0;
    }
  }
}

// Check the Moore neighborhood
int getNumberOfNeighbors(int x, int y) {
  return grid[x - 1][y] + grid[x - 1][y - 1] + grid[x][y - 1] + grid[x + 1][y - 1] + grid[x + 1][y] + grid[x + 1][y + 1] + grid[x][y + 1] + grid[x - 1][y + 1];
              }
              
              void statusSys() {
                M5.Lcd.setTextSize(1);
                M5.Lcd.fillScreen(BLUE);
               M5.Lcd.setTextColor(TFT_WHITE,TFT_GREY);
              while(digitalRead(M5_BUTTON_HOME) == HIGH){
                
                
                vbat      = M5.Axp.GetVbatData() * 1.1 / 1000;
                temp      =  -144.7 + M5.Axp.GetTempData() * 0.1;
                bat_p     =  M5.Axp.GetPowerbatData() * 1.1 * 0.5 /1000;
                M5.Lcd.setRotation(3);
                M5.Lcd.setCursor(0, 15, 1);
                M5.Lcd.printf("batt:%.3fV\r\n",vbat);  //battery voltage
                M5.Lcd.printf("temp:%.1fC\r\n",temp);  //axp192 inside temp
                M5.Lcd.printf("power:%.3fmW\r\n",bat_p);  //battery power
                
                }
  
  delay(200);
}
