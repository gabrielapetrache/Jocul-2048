Listă de piese:

    Modul microcontroler universal Arduino Uno R3
    Afișaj grafic 128x160px
    Modul Joystick
    Modul cu buton
    Buzzer pasiv

 
 Descrierea functiilor implementate

    setup(): Funcție specială în Arduino care este apelată o singură dată la pornirea plăcii pentru a inițializa ecranul și matricea de joc.

    loop(): Funcție specială în Arduino care este apelată în mod repetat după ce setup() se termină, este folosită pentru a gestiona logica jocului și a actualiza ecranul.

    initializeGrid(): Funcție care inițializează matricea de joc.

    addRandomTile(): Funcție care adaugă o nouă “plăcuță” într-o poziție aleatoare pe tabla de joc. O nouă plăcuță cu valoarea 2 sau 4 este adăugată într-o poziție aleatoare după fiecare mutare, cu probabilitate de 90%, respectiv 10%.

    drawGrid(): Funcție care desenează grila de joc pe ecranul TFT.

    drawTile(int x, int y, int value): Funcție care desenează o “plăcuță” cu o anumită valoare într-o anumită poziție pe grila de joc.

    moveLeft()/moveRight()/moveUp()/moveDown(): Funcții care mută toate “plăcuțele” de pe grila de joc la direcția specificată, conform regulilor jocului.
    
    bool isGameOver(): Funcție care verifică dacă s-a terminat jocul, adică dacă nu mai sunt mutări posibile.
    
    void drawGameOverScreen(): Funcție care desenează ecranul de game over și face buzzer-ul să scoată un sunet.
