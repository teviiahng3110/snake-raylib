#include <raylib.h>

struct Snake_Cell
{

};

int main() {
	const int Cell_Size = 20; // kích thước ô
	const int Col = 96; // số cột
	const int Row = 54; // số hàng 
	const int Width = Col * Cell_size;
	const int Height = Row * Cell_size;


	InitWindow(Width, Height, "Snake - raylib"); // full screen nhưng sẽ bị che bởi thanh taskbar, mốt dùng ToggleFullscreen().
	SetTargetFPS(60); // set cho chạy 60fps 
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
	}


	CloseWindow();
	return 0;
}