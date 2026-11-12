#include <raylib.h>

struct Snake_Cell
{

};

int main() {
	const int Cell_Size = 20; // kích thước ô
	const int Col = 60; // số cột
	const int Row = 34; // số hàng 
	const int Width = Col * Cell_Size;
	const int Height = Row * Cell_Size;


	InitWindow(Width, Height, "Snake - raylib"); // full screen nhưng sẽ bị che bởi thanh taskbar, mốt dùng ToggleFullscreen().
	SetTargetFPS(60); // set cho chạy 60fps 
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		for (int x = 0; x <= Width; x += Cell_Size) {
			DrawLine(x, 0, x, Height, DARKGRAY); // kẻ hàng dọc, cú pháp trong hàm x1, y1, x2, y2, màu
		}
		for (int y = 0; y <= Height; y += Cell_Size) {
			DrawLine(0, y, Width, y, DARKGRAY); //kẻ hàng ngang 
		}
		EndDrawing();
	}


	CloseWindow();
	return 0;
}