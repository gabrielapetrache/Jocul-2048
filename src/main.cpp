#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>


#define TFT_CS        10
#define TFT_RST        8
#define TFT_DC         7
#define VRx			A2
#define VRy			A3
#define RESET_BUTTON 		A0
#define SW			9
#define BUZZER		3

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Definirea dimensiunilor display-ului
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 160

// Dimensiunile tablei de joc
#define GRID_SIZE 4
#define TILE_SIZE (SCREEN_WIDTH / GRID_SIZE)

// Matricea de joc
int grid[GRID_SIZE][GRID_SIZE];

void setup();
void loop();
void initializeGrid();
void addRandomTile();
void drawGrid();
void drawTile(int x, int y, int value);
bool moveLeft();
bool moveRight();
bool moveUp();
bool moveDown();
bool isGameOver();
void drawGameOverScreen();

void setup() {
  // Inițializare display
  tft.initR(INITR_GREENTAB);
  tft.fillScreen(ST7735_BLACK);
  
  // Inițializare pin pentru butonul joystick-ului
  pinMode(SW, INPUT_PULLUP);
  // Inițializare pin pentru butonul de reset
  pinMode(RESET_BUTTON, INPUT_PULLUP);
  
  // Inițializare matrice de joc
  initializeGrid();
  addRandomTile();
  addRandomTile();
  
  // Desenare grid inițial
  drawGrid();
}

void loop() {
  // Citirea poziției joystick-ului
  int xVal = analogRead(VRx);
  int yVal = analogRead(VRy);
  
  static int prevXVal = 512;
  static int prevYVal = 512;

  bool moved = false;
  
  // Procesare input
  if (xVal < 300 && prevXVal >= 300) {
    moved = moveLeft();
  } else if (xVal > 700 && prevXVal <= 700) {
    moved = moveRight();
  } else if (yVal < 300 && prevYVal >= 300) {
    moved = moveDown();
  } else if (yVal > 700 && prevYVal <= 700) {
    moved = moveUp();
  }

  prevXVal = xVal;
  prevYVal = yVal;

  // Dacă s-a efectuat o mișcare, adăugăm o piesă nouă și desenăm grid-ul
  if (moved) {
    addRandomTile();
    drawGrid();
    if (isGameOver()) {
      drawGameOverScreen();
      while (true) {
		// Așteptăm apăsarea butonului de reset
		if (digitalRead(RESET_BUTTON) == LOW) {
		  initializeGrid();
		  addRandomTile();
		  addRandomTile();
		  drawGrid();
		  break;
		}
    }
	}
  }
  
  // Verificare buton de reset
  if (digitalRead(RESET_BUTTON) == LOW) {
    // Dacă butonul de reset este apăsat, reîncepe jocul
    initializeGrid();
    addRandomTile();
    addRandomTile();
    drawGrid();
  }
  
  // Întârziere pentru a evita citirea continuă a joystick-ului
  delay(200);
}

void initializeGrid() {
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      grid[i][j] = 0;
    }
  }
}

bool isGameOver() {
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      if (grid[i][j] == 0) {
        return false;
      }
    }
  }
  
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      if ((i < GRID_SIZE - 1 && grid[i][j] == grid[i + 1][j]) ||
          (j < GRID_SIZE - 1 && grid[i][j] == grid[i][j + 1])) {
        return false;
      }
    }
  }
  
  return true;
}

void drawGameOverScreen() {
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_RED);
  tft.setCursor(10, SCREEN_HEIGHT / 2 - 20);
  tft.print("GAME OVER");
  tft.setTextSize(1.7);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(10, SCREEN_HEIGHT / 2 + 20);
  tft.print("Press RESET");
  tone(BUZZER, 500, 1000); // Redare sunet game over pentru 1 secundă
  delay(1000); // Pauză pentru a asigura redarea completă a sunetului
}

void addRandomTile() {
  int x, y;
  do {
    x = random(GRID_SIZE);
    y = random(GRID_SIZE);
  } while (grid[x][y] != 0);
  grid[x][y] = (random(10) < 9) ? 2 : 4;  // 90% șanse de a adăuga 2, 10% șanse de a adăuga 4
}

void drawGrid() {
  tft.fillScreen(ST7735_BLACK);
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      drawTile(i, j, grid[i][j]);
    }
  }
}

void drawTile(int x, int y, int value) {
  int color = ST7735_WHITE;
  switch (value) {
    case 2: color = ST7735_YELLOW; break;
    case 4: color = ST7735_ORANGE; break;
    case 8: color = ST7735_RED; break;
    case 16: color = ST7735_MAGENTA; break;
    case 32: color = ST7735_CYAN; break;
    case 64: color = ST7735_BLUE; break;
    case 128: color = ST7735_CYAN; break;
    case 256: color = ST7735_GREEN; break;
    case 512: color = ST7735_WHITE; break;
    case 1024: color = ST7735_WHITE; break;
    case 2048: color = ST7735_WHITE; break;
  }
  tft.fillRect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
  tft.setTextColor(ST7735_BLACK);

  int textSize;
  int xOffset;
  int yOffset;

  if (value < 10) {
    textSize = 2;
    xOffset = TILE_SIZE / 2 - 6;
    yOffset = TILE_SIZE / 2 - 8;
  } else if (value < 100) {
    textSize = 2;
    xOffset = TILE_SIZE / 2 - 12;
    yOffset = TILE_SIZE / 2 - 8;
  } else if (value < 1000) {
    textSize = 1;
	xOffset = (TILE_SIZE - textSize * 6) / 2;
	yOffset = TILE_SIZE / 2 - 8;
  } else {
    textSize = 1;
    xOffset = (TILE_SIZE - textSize * 6) / 2;
    yOffset = TILE_SIZE / 2 - 6;
  }

  tft.setTextSize(textSize);
  tft.setCursor(x * TILE_SIZE + xOffset, y * TILE_SIZE + yOffset);

  if (value != 0) {
    tft.print(value);
  }
}

bool moveLeft() {
  bool moved = false;
  for (int i = 0; i < GRID_SIZE; i++) {
    int currentPosition = 0;
    for (int j = 0; j < GRID_SIZE; j++) {
      if (grid[i][j] != 0) {
        if (currentPosition != j) {
          grid[i][currentPosition] = grid[i][j];
          grid[i][j] = 0;
          moved = true;
        }
        currentPosition++;
      }
    }
    for (int j = 0; j < GRID_SIZE - 1; j++) {
      if (grid[i][j] != 0 && grid[i][j] == grid[i][j + 1]) {
        grid[i][j] *= 2;
        for (int k = j + 1; k < GRID_SIZE - 1; k++) {
          grid[i][k] = grid[i][k + 1];
        }
        grid[i][GRID_SIZE - 1] = 0;
        moved = true;
      }
    }
  }
  return moved;
}

bool moveRight() {
  bool moved = false;
  for (int i = 0; i < GRID_SIZE; i++) {
    int currentPosition = GRID_SIZE - 1;
    for (int j = GRID_SIZE - 1; j >= 0; j--) {
      if (grid[i][j] != 0) {
        if (currentPosition != j) {
          grid[i][currentPosition] = grid[i][j];
          grid[i][j] = 0;
          moved = true;
        }
        currentPosition--;
      }
    }
    for (int j = GRID_SIZE - 1; j > 0; j--) {
      if (grid[i][j] != 0 && grid[i][j] == grid[i][j - 1]) {
        grid[i][j] *= 2;
        for (int k = j - 1; k > 0; k--) {
          grid[i][k] = grid[i][k - 1];
        }
        grid[i][0] = 0;
        moved = true;
      }
    }
  }
  return moved;
}

bool moveUp() {
  bool moved = false;
  for (int j = 0; j < GRID_SIZE; j++) {
    int currentPosition = 0;
    for (int i = 0; i < GRID_SIZE; i++) {
      if (grid[i][j] != 0) {
        if (currentPosition != i) {
          grid[currentPosition][j] = grid[i][j];
          grid[i][j] = 0;
          moved = true;
        }
        currentPosition++;
      }
    }
    for (int i = 0; i < GRID_SIZE - 1; i++) {
      if (grid[i][j] != 0 && grid[i][j] == grid[i + 1][j]) {
        grid[i][j] *= 2;
        for (int k = i + 1; k < GRID_SIZE - 1; k++) {
          grid[k][j] = grid[k + 1][j];
        }
        grid[GRID_SIZE - 1][j] = 0;
        moved = true;
      }
    }
  }
  return moved;
}

bool moveDown() {
  bool moved = false;
  for (int j = 0; j < GRID_SIZE; j++) {
    int currentPosition = GRID_SIZE - 1;
    for (int i = GRID_SIZE - 1; i >= 0; i--) {
      if (grid[i][j] != 0) {
        if (currentPosition != i) {
          grid[currentPosition][j] = grid[i][j];
          grid[i][j] = 0;
          moved = true;
        }
        currentPosition--;
      }
    }
    for (int i = GRID_SIZE - 1; i > 0; i--) {
      if (grid[i][j] != 0 && grid[i][j] == grid[i - 1][j]) {
        grid[i][j] *= 2;
        for (int k = i - 1; k > 0; k--) {
          grid[k][j] = grid[k - 1][j];
        }
        grid[0][j] = 0;
        moved = true;
      }
    }
  }
  return moved;
}