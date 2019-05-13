#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
using namespace std;

enum Color { Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White };
enum { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, Esc = 27 };
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void showConsoleCursor(bool showFlag);
void setColor(Color text, Color background)
{
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
void setCursor(int x, int y)
{
	COORD myCoords = { x,y };
	SetConsoleCursorPosition(hStdOut, myCoords);
}

struct piece {
	int x;
	int y;
	Color color;
	int model[4][4];
	int num;
	int rotation;
};

const int pieces[7][4][4][4] = {
	{{{0, 0, 0, 0}, // i - block
	{0, 0, 0, 0},
	{1, 1, 1, 1},
	{0, 0, 0, 0}},

	{{0, 0, 1, 0},
	{0, 0, 1, 0},
	{0, 0, 1, 0},
	{0, 0, 1, 0}},

	{{0, 0, 0, 0},
	{0, 0, 0, 0},
	{1, 1, 1, 1},
	{0, 0, 0, 0}},

	{{0, 1, 0, 0},
	{0, 1, 0, 0},
	{0, 1, 0, 0},
	{0, 1, 0, 0}}},

	{{{0, 0, 0, 0}, // J-block
	{1, 0, 0, 0},
	{1, 1, 1, 0},
	{0, 0, 0, 0}},

	{{0, 1, 1, 0},
	{0, 1, 0, 0},
	{0, 1, 0, 0},
	{0, 0, 0, 0}},

	{{0, 0, 0, 0},
	{0, 1, 1, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 0}},

	{{0, 0, 0, 0},
	{0, 0, 1, 0},
	{0, 0, 1, 0},
	{0, 1, 1, 0}}},

	{{{0, 0, 0, 0}, //L-block
	{0, 0, 0, 1},
	{0, 1, 1, 1},
	{0, 0, 0, 0}},

	{{0, 0, 0, 0},
	{0, 1, 0, 0},
	{0, 1, 0, 0},
	{0, 1, 1, 0}},

	{{0, 0, 0, 0},
	{0, 0, 0, 0},
	{1, 1, 1, 0},
	{1, 0, 0, 0}},

	{{0, 1, 1, 0},
	{0, 0, 1, 0},
	{0, 0, 1, 0},
	{0, 0, 0, 0}}},

	{{{0, 0, 0, 0}, //O-block
	{0, 1, 1, 0},
	{0, 1, 1, 0},
	{0, 0, 0, 0}},

	{{0, 0, 0, 0},
	{0, 1, 1, 0},
	{0, 1, 1, 0},
	{0, 0, 0, 0}},

	{{0, 0, 0, 0},
	{0, 1, 1, 0},
	{0, 1, 1, 0},
	{0, 0, 0, 0}},

	{{0, 0, 0, 0},
	{0, 1, 1, 0},
	{0, 1, 1, 0},
	{0, 0, 0, 0}}},

	{{{0, 0, 0, 0}, //S-block
	{0, 1, 1, 0},
	{1, 1, 0, 0},
	{0, 0, 0, 0}},

	{{0, 1, 0, 0},
	{0, 1, 1, 0},
	{0, 0, 1, 0},
	{0, 0, 0, 0}},

	{{0, 0, 0, 0},
	{0, 0, 1, 1},
	{0, 1, 1, 0},
	{0, 0, 0, 0}},

	{{0, 0, 0, 0},
	{0, 1, 0, 0},
	{0, 1, 1, 0},
	{0, 0, 1, 0}}},

	{{{0, 0, 0, 0}, //T-block
	{0, 1, 0, 0},
	{1, 1, 1, 0},
	{0, 0, 0, 0}},

	{{0, 1, 0, 0},
	{0, 1, 1, 0},
	{0, 1, 0, 0},
	{0, 0, 0, 0}},

	{{0, 0, 0, 0},
	{0, 1, 1, 1},
	{0, 0, 1, 0},
	{0, 0, 0, 0}},

	{{0, 0, 0, 0},
	{0, 0, 1, 0},
	{0, 1, 1, 0},
	{0, 0, 1, 0}}},

	{{{0, 0, 0, 0}, //Z-block
	{1, 1, 0, 0},
	{0, 1, 1, 0},
	{0, 0, 0, 0}},

	{{0, 0, 1, 0},
	{0, 1, 1, 0},
	{0, 1, 0, 0},
	{0, 0, 0, 0}},

	{{0, 0, 0, 0},
	{0, 1, 1, 0},
	{0, 0, 1, 1},
	{0, 0, 0, 0}},

	{{0, 0, 0, 0},
	{0, 0, 1, 0},
	{0, 1, 1, 0},
	{0, 1, 0, 0}}}
};
bool paused;

//сохранение
void saveGame(int width, int height, int **pole, piece current, piece next, int score);
void loadGame(int &width, int &height, int** &pole, piece &current, piece &next, int &score);
bool checkForSave();
bool loadMenu();

//Отрисовка прямоугольника
void drawRect(int x, int y, int w, int h);

//Отрисовка прямоугольника на основе массива
void drawArRect(int x, int y, int** mas, int width, int height);

//Отрисовка детали
void drawPiece(piece piece);
void clearPiece(piece piece);
void drawNext(piece piece, int width);

//Работа с деталью
void setPiece(piece & piece);
void initPiece(piece & piece, int width);
bool checkCollision(piece piece, int** mas, int dir, int width, int height);
void mergePiece(piece piece, int** mas);
void rotatePiece(piece &piece, int** mas, int width, int height);

//Очистка внутри прямоугольника
void clearInRect(int x, int y, int w, int h);

//Главное меню
void customDim(int& width, int& height);
void menu(int& width, int& height);
bool gameOver();
void printHelp(int width);
void clearHelp(int width);
void printPause(int width);
void clearPause(int width);

//Работа с массивом поля
void setAr(int** mas, int width, int height);
void checkLines(int** mas, int width, int height, int& score);
void animateLine(int** mas, int width, int height, int line);
void copyAr(int** a, int** b, int width, int height);

void pause(bool state, chrono::time_point<chrono::steady_clock> &next);

Color getRand();

const char* logo = R"(       _            _          _            _            _         _        
      /\ \         /\ \       /\ \         /\ \         /\ \      / /\      
      \_\ \       /  \ \      \_\ \       /  \ \        \ \ \    / /  \     
      /\__ \     / /\ \ \     /\__ \     / /\ \ \       /\ \_\  / / /\ \__  
     / /_ \ \   / / /\ \_\   / /_ \ \   / / /\ \_\     / /\/_/ / / /\ \___\ 
    / / /\ \ \ / /_/_ \/_/  / / /\ \ \ / / /_/ / /    / / /    \ \ \ \/___/ 
   / / /  \/_// /____/\    / / /  \/_// / /__\/ /    / / /      \ \ \       
  / / /      / /\____\/   / / /      / / /_____/    / / /   _    \ \ \      
 / / /      / / /______  / / /      / / /\ \ \  ___/ / /__ /_/\__/ / /      
/_/ /      / / /_______\/_/ /      / / /  \ \ \/\__\/_/___\\ \/___/ /       
\_\/       \/__________/\_\/       \/_/    \_\/\/_________/ \_____\/        
                
                                                            )";

int main() {
	srand(time(NULL));
	int width, height, score = 0;
	piece pieceCurrent;
	piece pieceNext;
	paused = false;
	bool saved = false;

	int** pole=new int*[1];
	if (checkForSave() && loadMenu()) {
		saved = true;
		delete[]pole;
		loadGame(width, height, pole, pieceCurrent, pieceNext, score);
		showConsoleCursor(false);
		remove("save");
	}

	if (!saved) {
		menu(width, height);
		delete[]pole;
		pole = new int* [height];
		for (size_t i = 0; i < height; i++)
		{
			pole[i] = new int[width];
		}
		setAr(pole, width, height);
	}
	
	drawArRect(2, 2, pole, width, height);
	drawRect(10 + width, 2, 8, 6);
	setCursor(25 + width, 3);
	setlocale(LC_ALL, "rus");
	cout << "Счет: " << score;
	setCursor(25 + width, 5);
	cout << "F1 - помощь";
	setlocale(LC_ALL, "С");

	if (!saved) {
		initPiece(pieceCurrent, width);
		initPiece(pieceNext, width);
	}
	drawNext(pieceNext, width);

	using Framerate = chrono::duration<chrono::steady_clock::rep, ratio<1, 8>>;
	auto next = chrono::steady_clock::now() + Framerate{ 4 };
	while (1)
	{
		while (chrono::steady_clock::now() < next) {
			//------------------GAME CYCLE ------------------
			if (GetAsyncKeyState(VK_RIGHT) == -32767 && !paused) {
				if (checkCollision(pieceCurrent, pole, 2, width, height)) {
					clearPiece(pieceCurrent);
					pieceCurrent.x++;
					drawPiece(pieceCurrent);
				}
			}
			else if (GetAsyncKeyState(VK_LEFT) == -32767 && !paused) {
				if (checkCollision(pieceCurrent, pole, 1, width, height)) {
					clearPiece(pieceCurrent);
					pieceCurrent.x--;
					drawPiece(pieceCurrent);
				}
			}
			if (GetAsyncKeyState(VK_DOWN) == -32767 && !paused) {
				next -= Framerate{ 2 };
			}
			if (GetAsyncKeyState(VK_UP) == -32767 && !paused) {
				rotatePiece(pieceCurrent, pole, width, height);
				drawArRect(2, 2, pole, width, height);
				drawPiece(pieceCurrent);
			}
			if (GetAsyncKeyState(VK_F1) == -32767) {
				if (!paused) {
					pause(true, next);
					printHelp(width);
				}
				else {
					clearHelp(width);
					pause(false, next);
				}
			}
			if (GetAsyncKeyState(VK_ESCAPE) == -32767) { 
				if (!paused) {
					saveGame(width, height, pole, pieceCurrent, pieceNext, score);
					pause(true, next);	
					printPause(width);
				}
				else {
					clearPause(width);
					pause(false, next);
				}
			}
			//------------------GAME CYCLE ------------------
		}

		if (checkCollision(pieceCurrent, pole, 0, width, height)) {
			clearPiece(pieceCurrent);
			pieceCurrent.y += 1;
		}
		else {
			mergePiece(pieceCurrent, pole);
			drawArRect(2, 2, pole, width, height);

			if (checkCollision(pieceNext, pole, 3, width, height)) {
				pieceCurrent = pieceNext;
				initPiece(pieceNext, width);
				drawNext(pieceNext, width);
			}
			else {
				if (gameOver()) {
					return 0;
				}
				else { 
					setlocale(LC_ALL, "C");
					system("cls");
					score = 0;
					setColor(White, Black);
					setAr(pole, width, height);
					drawArRect(2, 2, pole, width, height);
					drawRect(10 + width, 2, 8, 6);
					initPiece(pieceCurrent, width);
					initPiece(pieceNext, width);
					drawNext(pieceNext, width);
					next = chrono::steady_clock::now();
				}
			}
		}
		checkLines(pole, width, height, score);
		drawPiece(pieceCurrent);
		next += Framerate{ 4 };
		if (score >= 700) {
			next -= Framerate{ 1 };
		}
	}


	system("pause");
	return 0;
}

bool loadMenu() {
	drawRect(25, 13, 25, 8);
	setlocale(LC_ALL, "rus");
	setCursor(27, 15);
	cout << "Обнаружено сохранение!";
	setCursor(27, 17);
	setColor(Black, White);
	cout << "Загрузиться";
	setColor(White, Black);
	setCursor(27, 18);
	cout << "Играть заново";

	setCursor(0, 0);

	int pos = 0;

	while (1) {
		if (_kbhit()) {
			switch (_getch())
			{
			case Up:
			case Down:
				if (pos) {
					pos = 0;
				}
				else {
					pos = 1;
				}
				break;
			case Enter:
				if (!pos)
				{//load
					system("cls");
					setlocale(LC_ALL, "C");
					return true;
				}
				else { //create new
					system("cls");
					setlocale(LC_ALL, "C");
					return false;
				}
				break;
			default:
				break;
			}


			clearInRect(25, 13, 25, 8);

			setCursor(27, 15);
			cout << "Обнаружено сохранение!";
			setCursor(27, 17);
			if (!pos)
			{
				setColor(Black, White);
			}
			else {
				setColor(White, Black);
			}
			cout << "Загрузиться";
			setCursor(27, 18);
			if (pos)
			{
				setColor(Black, White);
			}
			else {
				setColor(White, Black);
			}
			cout << "Играть заново";
			setColor(White, Black);
			setCursor(0, 0);
		}
	}
}
bool checkForSave() {
	bool isPresent;
	FILE* f;
	if (fopen_s(&f, "save", "rb")) {
		isPresent = false;
	}
	else {
		isPresent = true;
		fclose(f);
	}
	return isPresent;
}
void saveGame(int width, int height, int** pole, piece current, piece next, int score) {
	FILE* f;
	fopen_s(&f, "save", "wb");
	fwrite(&width, sizeof(width), 1, f);
	fwrite(&height, sizeof(height), 1, f);
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			fwrite(&pole[y][x], sizeof(pole[y][x]), 1, f);
		}	
	}
	fwrite(&current, sizeof(current), 1, f);
	fwrite(&next, sizeof(current), 1, f);
	fwrite(&score, sizeof(score), 1, f);

	fclose(f);
}
void loadGame(int &width, int &height, int** &pole, piece &current, piece &next, int &score) {
	FILE* f;
	fopen_s(&f, "save", "rb");

	fread(&width, sizeof(width), 1, f);
	fread(&height, sizeof(height), 1, f);

	pole = new int* [height];
	for (size_t i = 0; i < height; i++)
	{
		pole[i] = new int[width];
	}

	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			fread(&pole[y][x], sizeof(pole[y][x]), 1, f);
		}
	}
	fread(&current, sizeof(current), 1, f);
	fread(&next, sizeof(next), 1, f);
	fread(&score, sizeof(score), 1, f);
	fclose(f);
}
void copyAr(int** a, int** b, int width, int height) {
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			b[y][x] = a[y][x];
		}
	}
}
Color getRand() {
	return (Color)(1 + (rand() % 15));
}
void pause(bool state, chrono::time_point<chrono::steady_clock> &next) {
	using Framerate = chrono::duration<chrono::steady_clock::rep, ratio<10, 1>>;
	if (!paused) {
		next += Framerate{ MAXINT };
		paused = true;
	}
	else {
		next = chrono::steady_clock::now();
		paused = false;
	}
}
void printPause(int width) {
	drawRect(10 + width, 10, 25, 5);
	setlocale(LC_ALL, "rus");
	setCursor(11 + width, 11);
	cout << "Вы приостановили игру!";
	setCursor(11 + width, 12);
	cout << "Текущее состояние игры";
	setCursor(11 + width, 13);
	cout << "было сохранено";
	setlocale(LC_ALL, "C");
}
void clearPause(int width) {
	clearInRect(9 + width, 9, 27, 7);
}
void printHelp(int width) {
	drawRect(10 + width, 10, 40, 6);
	setlocale(LC_ALL, "rus");
	setCursor(11 + width, 11);
	cout << "Стрелки вправо, влево - перемещение";
	setCursor(11 + width, 12);
	cout << "Вверх - поворот";
	setCursor(11 + width, 13);
	cout << "Вниз - ускорение";
	setCursor(11 + width, 14);
	cout << "Esc - пауза";


	setlocale(LC_ALL, "C");
}
void clearHelp(int width) {
	clearInRect(9 + width, 9, 42, 8);
}
void drawRect(int x, int y, int w, int h) {
	const char TB = '\xCD'; // 205
	const char LR = '\xBA'; // 186
	const char TL = '\xC9'; // 201
	const char TR = '\xBB'; // 187
	const char BL = '\xC8'; // 200
	const char BR = '\xBC'; // 188

	for (size_t i = 0; i < h; i++)
	{
		setCursor(x, y + i);
		for (size_t g = 0; g < w; g++)
		{
			if (i == 0 && g == 0)
			{
				cout << TL;
			}
			else if (i == 0 && g == w - 1) {
				cout << TR;
			}
			else if (i == h - 1 && g == 0) {
				cout << BL;
			}
			else if (i == h - 1 && g == w - 1) {
				cout << BR;
			}
			else if (i == 0 || i == h - 1) {
				cout << TB;
			}
			else if (g == 0 || g == w - 1) {
				cout << LR;
			}
			else {
				cout << " ";
			}
		}
	}
}
void clearInRect(int x, int y, int w, int h) {
	for (size_t i = 1; i < h - 1; i++)
	{
		setCursor(x + 1, y + i);
		for (size_t g = 1; g < w - 1; g++)
		{
			cout << " ";
		}
	}
}
void customDim(int& width, int& height) {
	system("cls");
	cout << logo;
	setlocale(LC_ALL, "rus");
	setCursor(27, 15);
	cout << "Размер поля:";
	setCursor(27, 17);
	setColor(Black, White);
	cout << "Ширина: " << width;
	setColor(White, Black);
	setCursor(27, 18);
	cout << "Высота: " << height;

	int pos = 0;
	while (1) {
		if (_kbhit()) {
			switch (_getch())
			{
			case Up:
			case Down:
				if (pos) {
					pos = 0;
				}
				else {
					pos = 1;
				}
				break;
			case Left:
				if (!pos && width > 10)
				{
					width--;
				}
				else if (pos && height > 20) {
					height--;
				}
				break;
			case Right:
				if (!pos && width < 50) {
					width++;
				}
				else if (pos && height < 100) {
					height++;
				}
				break;
			case Enter:
				setlocale(LC_ALL, "C");
				return;
				break;
			default:
				break;
			}


			clearInRect(25, 13, 25, 8);

			setCursor(27, 15);
			cout << "Размер поля:";
			setCursor(27, 17);
			if (!pos)
			{
				setColor(Black, White);
			}
			else {
				setColor(White, Black);
			}
			cout << "Ширина: " << width;
			setCursor(27, 18);
			if (pos)
			{
				setColor(Black, White);
			}
			else {
				setColor(White, Black);
			}
			cout << "Высота: " << height;
			setColor(White, Black);
			setCursor(0, 0);
		}
	}
	setlocale(LC_ALL, "С");
}
void menu(int& width, int& height) {
	showConsoleCursor(false);
	SetConsoleTitle(L"Tetris");
	setColor(Red, Black);
	cout << logo;
	setColor(White, Black);

	drawRect(25, 13, 25, 8);
	setlocale(LC_ALL, "rus");
	setCursor(27, 15);
	cout << "Выберите размер поля:";
	setCursor(27, 17);
	setColor(Black, White);
	cout << "По умолчанию (10x20)";
	setColor(White, Black);
	setCursor(27, 18);
	cout << "Свой";

	setCursor(0, 0);

	int pos = 0;

	while (1) {
		if (_kbhit()) {
			switch (_getch())
			{
			case Up:
			case Down:
				if (pos) {
					pos = 0;
				}
				else {
					pos = 1;
				}
				break;
			case Enter:
				width = 10;
				height = 20;
				if (!pos)
				{
					system("cls");
					setlocale(LC_ALL, "C");
					return;
				}
				else {
					customDim(width, height);
					system("cls");
					setlocale(LC_ALL, "C");
					return;
				}
				break;
			default:
				break;
			}


			clearInRect(25, 13, 25, 8);

			setCursor(27, 15);
			cout << "Выберите размер поля:";
			setCursor(27, 17);
			if (!pos)
			{
				setColor(Black, White);
			}
			else {
				setColor(White, Black);
			}
			cout << "По умолчанию (10x20)";
			setCursor(27, 18);
			if (pos)
			{
				setColor(Black, White);
			}
			else {
				setColor(White, Black);
			}
			cout << "Свой";
			setColor(White, Black);
			setCursor(0, 0);
		}
	}
}
bool gameOver() {
	system("cls");
	const char* logo = R"(  /$$$$$$   /$$$$$$  /$$      /$$ /$$$$$$$$        /$$$$$$  /$$    /$$ /$$$$$$$$ /$$$$$$$ 
 /$$__  $$ /$$__  $$| $$$    /$$$| $$_____/       /$$__  $$| $$   | $$| $$_____/| $$__  $$
| $$  \__/| $$  \ $$| $$$$  /$$$$| $$            | $$  \ $$| $$   | $$| $$      | $$  \ $$
| $$ /$$$$| $$$$$$$$| $$ $$/$$ $$| $$$$$         | $$  | $$|  $$ / $$/| $$$$$   | $$$$$$$/
| $$|_  $$| $$__  $$| $$  $$$| $$| $$__/         | $$  | $$ \  $$ $$/ | $$__/   | $$__  $$
| $$  \ $$| $$  | $$| $$\  $ | $$| $$            | $$  | $$  \  $$$/  | $$      | $$  \ $$
|  $$$$$$/| $$  | $$| $$ \/  | $$| $$$$$$$$      |  $$$$$$/   \  $/   | $$$$$$$$| $$  | $$
 \______/ |__/  |__/|__/     |__/|________/       \______/     \_/    |________/|__/  |__/
                                                                                          
                                                                                          
                                                                                          )";
	setColor(Red, Black);
	cout << logo;

	int pos = 0;
	setlocale(LC_ALL, "rus");
	setCursor(27, 10);
	cout << "Играть снова?";
	setCursor(27, 12);
	setColor(Black, Red);
	cout << "Да";
	setColor(Red, Black);
	setCursor(32, 12);
	cout << "Нет";

	while (1) {
		if (_kbhit()) {
			switch (_getch())
			{
			case Right:
				if (!pos) {
					pos = 1;
				}
				break;
			case Left:
				if (pos) {
					pos = 0;
				}
				break;
			case Enter:
				if (!pos)
				{
					system("cls");
					setlocale(LC_ALL, "С");
					return false;
				}
				else {
					cout << "\n";
					return true;
				}
				break;
			default:
				break;
			}

			setCursor(27, 10);
			setColor(Red, Black);
			cout << "Играть снова?";
			setCursor(27, 12);
			if (!pos)
			{
				setColor(Black, Red);
			}
			else {
				setColor(Red, Black);
			}
			cout << "Да";
			setCursor(32, 12);
			if (pos)
			{
				setColor(Black, Red);
			}
			else {
				setColor(Red, Black);
			}
			cout << "Нет";
			setColor(White, Black);
		}
	}
	return false;
}
void showConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; 
	SetConsoleCursorInfo(out, &cursorInfo);
}
void drawArRect(int x, int y, int** mas, int width, int height) {
	drawRect(x, y, width + 2, height + 2);
	for (size_t i = 0; i < height; i++)
	{
		setCursor(x + 1, y + i + 1);
		for (size_t g = 0; g < width; g++)
		{
			//mas[i][g]
			setColor((Color)mas[i][g], (Color)mas[i][g]);
			cout << " ";
			setColor(White, Black);
		}
	}
}
void setAr(int** mas, int width, int height) {
	for (size_t y = 0; y < width; y++)
	{
		for (size_t x = 0; x < height; x++)
		{
			mas[x][y] = 0;
		}
	}
}
void drawPiece(piece piece) {
	for (size_t y = 0; y < 4; y++)
	{
		for (size_t x = 0; x < 4; x++)
		{
			if (!piece.model[y][x]) {
				continue;
			}
			setCursor(piece.x + 3 + x, piece.y + 3 + y);
			setColor((Color)piece.model[y][x], (Color)piece.model[y][x]);
			cout << " ";
		}
	}
	setColor(White, Black);
}
void clearPiece(piece piece) {
	for (size_t y = 0; y < 4; y++)
	{
		for (size_t x = 0; x < 4; x++)
		{
			if (!piece.model[y][x]) {
				continue;
			}
			setCursor(piece.x + 3 + x, piece.y + 3 + y);
			setColor(Black, Black);
			cout << " ";
		}
	}
	setColor(White, Black);
}
void drawNext(piece piece, int width) {
	clearInRect(10 + width, 2, 8, 6);
	for (int y = 0; y < 4; y++)
	{
		setCursor(12 + width, 3 + y);
		for (int x = 0; x < 4; x++)
		{
			setColor((Color)piece.model[y][x], (Color)piece.model[y][x]);
			cout << " ";
		}
	}
	setColor(White, Black);
}
void setPiece(piece & piece) {
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t g = 0; g < 4; g++)
		{
			if (!pieces[piece.num][piece.rotation][i][g])
			{
				piece.model[i][g] = 0;
			}
			else {
				piece.model[i][g] = piece.color;
			}
		}
	}
}
void initPiece(piece & piece, int width) {
	piece.x = (width / 2) - 4;
	piece.y = 0;
	piece.color = getRand();
	piece.num = rand() % 7;
	piece.rotation = rand() % 4;

	setPiece(piece);
}
bool checkCollision(piece piece, int** mas, int dir, int width, int height) {
	//true - все найс, false - столкновение
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (piece.model[y][x])
			{
				switch (dir) // 0 - вниз, 1 - влево, 2 - вправо
				{
				case 0: // all ok
					if ((piece.y + y + 1) >= height) {
						return false;
					}
					if (mas[piece.y + y + 1][piece.x + x] != 0)
					{
						return false;
					}
					break;
				case 1: //all ok
					if ((piece.x + x - 1) < 0) {
						return false;
					}
					if (mas[piece.y + y][piece.x + x - 1] != 0)
					{
						return false;
					}
					break;
				case 2: //all ok
					if ((piece.x + x + 1) >= width) {
						return false;
					}
					if (mas[piece.y + y][piece.x + x + 1] != 0)
					{
						return false;
					}
					break;
				case 3:
					if (mas[piece.y + y][piece.x + x] != 0)
					{
						return false;
					}
					break;
				}
				if (mas[piece.y + y][piece.x + x] != 0)
				{
					return false;
				}
			}
		}
	}
	return true;
}
void mergePiece(piece piece, int** mas) {
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (piece.model[y][x]) {
				mas[piece.y + y][piece.x + x] = piece.model[y][x];
			}
		}
	}
}
void rotatePiece(piece & piece, int** mas, int width, int height) {
	bool isPermited = true;

	int rotate = piece.rotation + 1;
	if (rotate == 4)
	{
		rotate = 0;
	}

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (pieces[piece.num][rotate][y][x])
			{
				if (mas[piece.y + y][piece.x + x] != 0) // тут странный баг с выходом за границы
				{
					isPermited = false;
					break;
				}
			}
		}
		if (!isPermited) {
			break;
		}
	}

	if (isPermited) {
		piece.rotation = rotate;
		setPiece(piece);
	}
}
void checkLines(int** mas, int width, int height, int& score) {
	for (int y = height - 1; y >= 0; y--)
	{
		bool line = true;
		bool oneColor = true;
		for (int x = 0; x < width; x++)
		{
			if (!mas[y][x])
			{
				line = false;
				break;
			}
			if (x != width - 1 && mas[y][x] != mas[y][x + 1]) {
				oneColor = false;
			}
		}
		if (line) {
			animateLine(mas, width, height, y);

			for (int u = y; u >= 1; u--)
			{
				for (int c = 0; c < width; c++)
				{
					mas[u][c] = mas[u - 1][c];
				}
			}
			score += 100;
			drawArRect(2, 2, mas, width, height);
		}
		if (oneColor && line) {
			score += 100;
		}
		setCursor(25 + width, 3);
		setlocale(LC_ALL, "rus");
		cout << "Счет: " << score;
		setlocale(LC_ALL, "C");
	}
}
void animateLine(int** mas, int width, int height, int line) {
	setColor(Black, Black);
	for (size_t i = 0; i < width; i++)
	{
		setCursor(3 + i, 3 + line);
		cout << " ";
		mas[line][i] = 0;
		Sleep(25);
	}
	setColor(White, Black);
}