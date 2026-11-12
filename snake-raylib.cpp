#include <raylib.h>

struct Snake_Cell
{
	int Col_Snake; // khi chạy rắn thay đổi toạ độ nên không dùng const, lưu rắn thành ô
	int Row_Snake;
};

int main() {
	const int Cell_Size = 20; // kích thước ô
	const int Col = 60; // số cột
	const int Row = 34; // số hàng 
	const int Width = Col * Cell_Size;
	const int Height = Row * Cell_Size;
	Snake_Cell head;
	/* Truy về ô gần giữa.Với số chẵn sẽ là ô đầu của nửa sau (vd 10 / 2 = 5 = > chỉ số của ô thứ 6 (vẫn là 1 ô giữa vì số ô là chẵn sẽ ko có ô giữa tuyệt đối, 
	không cần - 1 cho chỉ số vì vẫn truy ra ô giữa (chỉ số 4 cho ô thứ 5). Số lẻ vì theo kiểu int sẽ làm tròn xuống.VD: 5 / 2 = 2.5 -> 2 => chỉ số của 
	ô giữa tuyệt đối. Chỉ số ô chạy từ 0 -> số ô - 1*/
	head.Col_Snake = Col / 2;
	head.Row_Snake = Row / 2;

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

		// Note kiến thức: Tọa độ của ô = chỉ số cột (bắt đầu từ 0) => tính bằng cột số bao nhiêu - 1, ví dụ cột thứ 4 có chỉ số là 3.
		DrawRectangle(head.Col_Snake * Cell_Size, head.Row_Snake * Cell_Size, Cell_Size, Cell_Size, GREEN); // vẽ đầu rắn từ vị trí ô đầu rắn, quy đổi pixel 
		EndDrawing();
	}


	CloseWindow();
	return 0;
}