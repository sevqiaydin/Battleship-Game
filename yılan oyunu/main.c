
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 20
#define MAX_SNAKE_LENGTH 100
#define TABLE_SIZE 10

// Yýlanýn gövdesi için Dizi (Array) yapýsý
typedef struct Node {
    int x;
    int y;
} Node;

Node snake[MAX_SNAKE_LENGTH]; // Yýlanýn gövdesi için dinamik dizi
int snakeLength = 1; // Yýlanýn uzunluðu
int foodX, foodY;
int score = 0;
int gameOver = 0;

// Hash Tablosu yapýsý
typedef struct Player {
    char name[50];
    int score;
    struct Player* next; // Zincirleme yöntemi
} Player;

typedef struct HashTable {
    Player* table[TABLE_SIZE];
} HashTable;

HashTable ht;  // Oyuncu bilgilerini saklayan hash tablosu

// Yýlanýn baþlangýç yönü
enum Direction { UP, DOWN, LEFT, RIGHT };
enum Direction currentDirection = RIGHT;  // Baþlangýç yönü sað

// Zorluk seviyesi (1: Easy, 2: Medium, 3: Hard)
int difficulty = 2;  // Orta zorluk seviyesini varsayýlan olarak belirleyelim

// Hash fonksiyonu
int hash_function(char* name) {
    int hash = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        hash += name[i]; // Karakterlerin ASCII deðerlerini topluyoruz
    }
    return hash % TABLE_SIZE; // Hash deðerini tablo boyutuna göre mod alýyoruz
}

// Hash tablosuna oyuncu ekleme
void insert(HashTable* ht, char* name, int score) {
    int index = hash_function(name);
    Player* new_player = (Player*)malloc(sizeof(Player));
    strcpy(new_player->name, name);
    new_player->score = score;
    new_player->next = ht->table[index]; // Yeni oyuncuyu baþa ekliyoruz
    ht->table[index] = new_player;
}

// Skoru alma
int get_score(HashTable* ht, char* name) {
    int index = hash_function(name);
    Player* current = ht->table[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->score; // Skoru döndürüyoruz
        }
        current = current->next;
    }
    return -1; // Oyuncu bulunamazsa -1 döndürüyoruz
}

// Hash tablosunu temizleme
void free_table(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Player* current = ht->table[i];
        while (current != NULL) {
            Player* temp = current;
            current = current->next;
            free(temp);
        }
    }
}

// Oyunun baþlangýç ayarlarýný yap
void initializeGame() {
    snake[0].x = WIDTH / 2;
    snake[0].y = HEIGHT / 2;
    snakeLength = 1; // Baþlangýçta yýlan sadece bir segmentten oluþuyor

    foodX = rand() % WIDTH;
    foodY = rand() % HEIGHT;

    // Hash tablosu baþlatýlýyor
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht.table[i] = NULL;
    }
}

// Ekraný çiz
void draw() {
    system("cls"); // Ekraný temizle
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    // Yýlanýn, çerçevenin ve yemin çizimi
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) printf("#");
            if (i == snake[0].y && j == snake[0].x) printf("O"); // Yýlanýn baþý
            else if (i == foodY && j == foodX) printf("F"); // Yem
            else {
                int found = 0;
                for (int k = 1; k < snakeLength; k++) {
                    if (snake[k].x == j && snake[k].y == i) {
                        printf("o"); // Yýlanýn gövdesi
                        found = 1;
                        break;
                    }
                }
                if (!found) printf(" ");
            }
            if (j == WIDTH - 1) printf("#");
        }
        printf("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\nScore: %d\n", score);
}

// Yýlanýn hareketini güncelle
void update() {
    // Yýlanýn baþý için yeni koordinat
    int newX = snake[0].x;
    int newY = snake[0].y;

    // Kullanýcýnýn yön deðiþtirip deðiþtirmediðini kontrol et
    if (_kbhit()) {
        switch (_getch()) {
            case 'w': if (currentDirection != DOWN) currentDirection = UP; break;   // Yukarý
            case 's': if (currentDirection != UP) currentDirection = DOWN; break;   // Aþaðý
            case 'a': if (currentDirection != RIGHT) currentDirection = LEFT; break; // Sol
            case 'd': if (currentDirection != LEFT) currentDirection = RIGHT; break; // Sað
            case 'x': gameOver = 1; break; // Çýkýþ
        }
    }

    // Yýlanýn baþý yönüne göre ilerlesin
    switch (currentDirection) {
        case UP: newY--; break;
        case DOWN: newY++; break;
        case LEFT: newX--; break;
        case RIGHT: newX++; break;
    }

    // Duvara çarpma durumu
    if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT) {
        gameOver = 1; // Yýlan duvara çarptý
    }

    // Yýlanýn baþýný bir ileriye taþýyoruz
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];  // Gövde segmentleri bir adým kayar
    }

    snake[0].x = newX;  // Yeni baþý yerleþtir
    snake[0].y = newY;

    // Eðer yýlan yemi yediyse
    if (snake[0].x == foodX && snake[0].y == foodY) {
        score++;

        // Zorluk seviyesine göre yýlanýn büyüme hýzýný ayarlýyoruz
        if (difficulty == 1) {
            // Kolay zorlukta yýlan yavaþ büyür
            if (snakeLength < MAX_SNAKE_LENGTH) snakeLength++;
        } else if (difficulty == 2) {
            // Orta zorlukta normal hýzda büyür
            if (snakeLength < MAX_SNAKE_LENGTH) snakeLength++;
        } else if (difficulty == 3) {
            // Zor zorlukta yýlan hýzlý büyür
            if (snakeLength < MAX_SNAKE_LENGTH) snakeLength += 2;
        }

        // Yem yeni bir yere yerleþiyor
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;
    }
}

// Zorluk seviyesini seç
void setDifficulty() {
    printf("Select difficulty level (1: Easy, 2: Medium, 3: Hard): ");
    scanf("%d", &difficulty);

    if (difficulty == 1) {
        printf("You selected Easy. Yýlanýn hareket hýzý düþük ve büyüme hýzý yavaþ.\n");
    } else if (difficulty == 2) {
        printf("You selected Medium. Orta zorluk.\n");
    } else if (difficulty == 3) {
        printf("You selected Hard. Yýlan hýzlý büyür ve hareket eder.\n");
    } else {
        printf("Invalid choice. Defaulting to Medium.\n");
        difficulty = 2;
    }
}

// Oyunun baþlangýcýnda oyuncu ismi girilmesini saðla
void start_game() {
    char playerName[50];
    printf("Enter your name: ");
    scanf("%s", playerName);

    setDifficulty(); // Zorluk seviyesini seç

    initializeGame();
    while (!gameOver) {
        draw();
        update();

        // Zorluk seviyesine göre oyun hýzýný ayarla
        if (difficulty == 1) {
            Sleep(150); // Kolayda yavaþ hareket
        } else if (difficulty == 2) {
            Sleep(100); // Orta hýzda
        } else if (difficulty == 3) {
            Sleep(50);  // Zor seviyede hýzlý hareket
        }
    }

    printf("Game Over! Your final score is: %d\n", score);

    // Oyuncunun adýný ve skorunu hash tablosuna ekle
    insert(&ht, playerName, score);
    printf("Player %s has a score of %d.\n", playerName, get_score(&ht, playerName));
}

int main() {
    start_game();
    free_table(&ht);  // Hash tablosunu serbest býrak
    return 0;
}
