
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 20
#define MAX_SNAKE_LENGTH 100
#define TABLE_SIZE 10

// Y�lan�n g�vdesi i�in Dizi (Array) yap�s�
typedef struct Node {
    int x;
    int y;
} Node;

Node snake[MAX_SNAKE_LENGTH]; // Y�lan�n g�vdesi i�in dinamik dizi
int snakeLength = 1; // Y�lan�n uzunlu�u
int foodX, foodY;
int score = 0;
int gameOver = 0;

// Hash Tablosu yap�s�
typedef struct Player {
    char name[50];
    int score;
    struct Player* next; // Zincirleme y�ntemi
} Player;

typedef struct HashTable {
    Player* table[TABLE_SIZE];
} HashTable;

HashTable ht;  // Oyuncu bilgilerini saklayan hash tablosu

// Y�lan�n ba�lang�� y�n�
enum Direction { UP, DOWN, LEFT, RIGHT };
enum Direction currentDirection = RIGHT;  // Ba�lang�� y�n� sa�

// Zorluk seviyesi (1: Easy, 2: Medium, 3: Hard)
int difficulty = 2;  // Orta zorluk seviyesini varsay�lan olarak belirleyelim

// Hash fonksiyonu
int hash_function(char* name) {
    int hash = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        hash += name[i]; // Karakterlerin ASCII de�erlerini topluyoruz
    }
    return hash % TABLE_SIZE; // Hash de�erini tablo boyutuna g�re mod al�yoruz
}

// Hash tablosuna oyuncu ekleme
void insert(HashTable* ht, char* name, int score) {
    int index = hash_function(name);
    Player* new_player = (Player*)malloc(sizeof(Player));
    strcpy(new_player->name, name);
    new_player->score = score;
    new_player->next = ht->table[index]; // Yeni oyuncuyu ba�a ekliyoruz
    ht->table[index] = new_player;
}

// Skoru alma
int get_score(HashTable* ht, char* name) {
    int index = hash_function(name);
    Player* current = ht->table[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->score; // Skoru d�nd�r�yoruz
        }
        current = current->next;
    }
    return -1; // Oyuncu bulunamazsa -1 d�nd�r�yoruz
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

// Oyunun ba�lang�� ayarlar�n� yap
void initializeGame() {
    snake[0].x = WIDTH / 2;
    snake[0].y = HEIGHT / 2;
    snakeLength = 1; // Ba�lang��ta y�lan sadece bir segmentten olu�uyor

    foodX = rand() % WIDTH;
    foodY = rand() % HEIGHT;

    // Hash tablosu ba�lat�l�yor
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht.table[i] = NULL;
    }
}

// Ekran� �iz
void draw() {
    system("cls"); // Ekran� temizle
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    // Y�lan�n, �er�evenin ve yemin �izimi
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) printf("#");
            if (i == snake[0].y && j == snake[0].x) printf("O"); // Y�lan�n ba��
            else if (i == foodY && j == foodX) printf("F"); // Yem
            else {
                int found = 0;
                for (int k = 1; k < snakeLength; k++) {
                    if (snake[k].x == j && snake[k].y == i) {
                        printf("o"); // Y�lan�n g�vdesi
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

// Y�lan�n hareketini g�ncelle
void update() {
    // Y�lan�n ba�� i�in yeni koordinat
    int newX = snake[0].x;
    int newY = snake[0].y;

    // Kullan�c�n�n y�n de�i�tirip de�i�tirmedi�ini kontrol et
    if (_kbhit()) {
        switch (_getch()) {
            case 'w': if (currentDirection != DOWN) currentDirection = UP; break;   // Yukar�
            case 's': if (currentDirection != UP) currentDirection = DOWN; break;   // A�a��
            case 'a': if (currentDirection != RIGHT) currentDirection = LEFT; break; // Sol
            case 'd': if (currentDirection != LEFT) currentDirection = RIGHT; break; // Sa�
            case 'x': gameOver = 1; break; // ��k��
        }
    }

    // Y�lan�n ba�� y�n�ne g�re ilerlesin
    switch (currentDirection) {
        case UP: newY--; break;
        case DOWN: newY++; break;
        case LEFT: newX--; break;
        case RIGHT: newX++; break;
    }

    // Duvara �arpma durumu
    if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT) {
        gameOver = 1; // Y�lan duvara �arpt�
    }

    // Y�lan�n ba��n� bir ileriye ta��yoruz
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];  // G�vde segmentleri bir ad�m kayar
    }

    snake[0].x = newX;  // Yeni ba�� yerle�tir
    snake[0].y = newY;

    // E�er y�lan yemi yediyse
    if (snake[0].x == foodX && snake[0].y == foodY) {
        score++;

        // Zorluk seviyesine g�re y�lan�n b�y�me h�z�n� ayarl�yoruz
        if (difficulty == 1) {
            // Kolay zorlukta y�lan yava� b�y�r
            if (snakeLength < MAX_SNAKE_LENGTH) snakeLength++;
        } else if (difficulty == 2) {
            // Orta zorlukta normal h�zda b�y�r
            if (snakeLength < MAX_SNAKE_LENGTH) snakeLength++;
        } else if (difficulty == 3) {
            // Zor zorlukta y�lan h�zl� b�y�r
            if (snakeLength < MAX_SNAKE_LENGTH) snakeLength += 2;
        }

        // Yem yeni bir yere yerle�iyor
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;
    }
}

// Zorluk seviyesini se�
void setDifficulty() {
    printf("Select difficulty level (1: Easy, 2: Medium, 3: Hard): ");
    scanf("%d", &difficulty);

    if (difficulty == 1) {
        printf("You selected Easy. Y�lan�n hareket h�z� d���k ve b�y�me h�z� yava�.\n");
    } else if (difficulty == 2) {
        printf("You selected Medium. Orta zorluk.\n");
    } else if (difficulty == 3) {
        printf("You selected Hard. Y�lan h�zl� b�y�r ve hareket eder.\n");
    } else {
        printf("Invalid choice. Defaulting to Medium.\n");
        difficulty = 2;
    }
}

// Oyunun ba�lang�c�nda oyuncu ismi girilmesini sa�la
void start_game() {
    char playerName[50];
    printf("Enter your name: ");
    scanf("%s", playerName);

    setDifficulty(); // Zorluk seviyesini se�

    initializeGame();
    while (!gameOver) {
        draw();
        update();

        // Zorluk seviyesine g�re oyun h�z�n� ayarla
        if (difficulty == 1) {
            Sleep(150); // Kolayda yava� hareket
        } else if (difficulty == 2) {
            Sleep(100); // Orta h�zda
        } else if (difficulty == 3) {
            Sleep(50);  // Zor seviyede h�zl� hareket
        }
    }

    printf("Game Over! Your final score is: %d\n", score);

    // Oyuncunun ad�n� ve skorunu hash tablosuna ekle
    insert(&ht, playerName, score);
    printf("Player %s has a score of %d.\n", playerName, get_score(&ht, playerName));
}

int main() {
    start_game();
    free_table(&ht);  // Hash tablosunu serbest b�rak
    return 0;
}
