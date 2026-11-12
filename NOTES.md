# Ghi chú học lập trình game (C++ / raylib)

## 1. Cài đặt môi trường
- Cài raylib bằng vcpkg:
  ```
  .\bootstrap-vcpkg.bat
  .\vcpkg integrate install
  .\vcpkg install raylib:x64-windows
  ```
- **Vì sao phải có `.\`?** PowerShell không tự chạy file ở thư mục hiện tại, nên phải chỉ rõ.
- Visual Studio phải build ở **x64** vì đã cài `raylib:x64-windows`.
- IntelliSense có thể báo lỗi `cannot open raylib.h` dù build vẫn chạy: đổi bộ lọc Error List sang *Build Only* để xem lỗi thật.

## 2. Khung chương trình raylib
```cpp
InitWindow(w, h, "title");
SetTargetFPS(60);
while (!WindowShouldClose()) {   // game loop
    BeginDrawing();
    ClearBackground(BLACK);
    // vẽ ở đây
    EndDrawing();
}
CloseWindow();
```
- **Game loop**: mỗi vòng là 1 frame. Vòng lặp tự quay ~60 lần/giây, **không phụ thuộc phím bấm**; phím chỉ quyết định kết quả các câu `if` trong mỗi vòng.
- **Thứ tự trong vòng lặp**: input → cập nhật → chặn/va chạm → **vẽ (cuối cùng)**. Vẽ sau cùng để người chơi chỉ thấy trạng thái đã xử lý xong. (Nếu vẽ trước khi chặn viền, hình sẽ lẹm ra ngoài đúng bằng 1 bước di chuyển.)
- Thứ vẽ sau sẽ **đè lên** thứ vẽ trước (nền → lưới → rắn).

## 3. Hệ tọa độ
- Gốc (0, 0) ở **góc trên trái**; x tăng sang phải, **y tăng đi xuống**.
- Đơn vị là pixel.
- `DrawRectangle(x, y, rộng, cao, màu)`: (x, y) là **góc trên trái** của hình, không phải tâm.
- Căn giữa: `x = screenWidth / 2 - size / 2`.
- **Không gõ cứng số**: dùng biến/hằng số để đổi kích thước cửa sổ mà mọi thứ tự khớp.

## 4. C++ cơ bản đã gặp
- `const`: giá trị không đổi sau khi khai báo. Bảo vệ khỏi sửa nhầm, code dễ đọc, thay cho "số ma thuật".
- `struct`: gom dữ liệu liên quan, truy cập từng biến bằng dấu `.` (ví dụ `head.Col_Snake`).
- Chia `int` cắt phần lẻ: `5 / 2 = 2`.
- `switch` hợp với **một biến** có nhiều giá trị rời rạc (ví dụ trạng thái game). Không hợp để kiểm tra nhiều phím độc lập, vì `switch` chỉ chọn một nhánh mà ta cần nhiều `if` độc lập (đi chéo = giữ 2 phím).
- Biến khai báo trong `for` chỉ sống trong vòng `for` đó.
- `struct` và `class` gần như giống nhau (khác mặc định public/private). Bắt đầu bằng `struct`, khi có nhiều hàm xoay quanh cùng dữ liệu thì refactor thành `class`.

## 5. Cách máy chạy code
- Chạy từng dòng **từ trên xuống**, hết vòng thì quay lại đầu.
- Máy không nhớ "ý định", chỉ biết giá trị biến **tại thời điểm đọc tới dòng đó**.
- Muốn một giá trị ảnh hưởng đoạn nào thì phải gán **trước** đoạn đó (như chỉnh lửa trước khi nấu).
- `IsKeyDown(k)`: true ở **mọi frame** khi đang giữ. `IsKeyPressed(k)`: true **đúng 1 frame** khi vừa nhấn.

## 6. FPS và delta time
- Nếu mỗi frame cộng cố định `speed`, thì tốc độ thực = `speed × FPS`. Đổi FPS là đổi tốc độ, máy yếu chạy chậm, máy mạnh chạy nhanh.
- Giải pháp: `GetFrameTime()` trả số giây của frame vừa rồi; để `speed` tính theo pixel/giây rồi nhân với nó.
- Snake dùng **bộ đếm giờ**: cộng dồn `GetFrameTime()`, đủ `moveInterval` thì tiến 1 ô rồi trừ lại.

## 7. Quyết định thiết kế: lưu vị trí theo Ô, không theo Pixel
**Quyết định:** đầu rắn (và thân, mồi) lưu bằng chỉ số ô `(col, row)`. Chỉ khi vẽ mới đổi sang pixel: `x = col * Cell_Size`.

**Vì sao:**
1. **So sánh chính xác.** Ăn mồi chỉ là `head.col == food.col && head.row == food.row`. Nếu lưu pixel, rắn và mồi có thể lệch vài pixel và không bao giờ "trùng" đúng, phải viết thêm logic khoảng cách rất rối.
2. **Di chuyển đơn giản.** Rắn Nokia nhảy từng ô: tiến 1 ô là `col += 1`, không phải `x += Cell_Size`.
3. **Tách logic khỏi hiển thị.** Luật game chạy trên lưới; pixel chỉ là chuyện vẽ. Đổi `Cell_Size` (zoom to/nhỏ) thì logic không phải sửa.
4. **Dễ kiểm tra biên.** Hợp lệ khi `0 <= Col_Snake < Col` và `0 <= Row_Snake < Row`. Không cần nhớ kích thước pixel.

**Lỗi đã gặp:** lúc đầu gán `x_Snake = (Col/2 - 1) * Cell_Size`, tức lưu pixel trong biến "ô". Vẽ vẫn đúng nhưng sẽ rối khi di chuyển và so sánh. Sửa: biến chỉ lưu chỉ số ô, phép nhân đặt trong `DrawRectangle`.

**Chỉ số ô:**
- Chạy từ `0` đến `Col - 1` (không phải `Col`). Ô `(Col, Row)` nằm ngoài cửa sổ.
- Kiểm tra: `(0, 0)` = góc trên trái; `(Col-1, Row-1)` = góc dưới phải.
- Số cột chẵn thì không có ô giữa tuyệt đối; `Col / 2` chọn ô ngay sau tâm.

## 8. Vẽ lưới
- Đường dọc: `DrawLine(x, 0, x, Height, màu)`, `x` chạy từ 0 đến `Width`, bước `Cell_Size`.
- Đường ngang: `DrawLine(0, y, Width, y, màu)`, `y` chạy từ 0 đến `Height`.
- Mẹo nhớ: đường chạy theo hướng nào thì dùng kích thước của hướng đó (dọc = `Height`, ngang = `Width`).
- Dùng `<=` để có đường ở mép phải/dưới. Lưới chỉ để nhìn, không ảnh hưởng logic.
- Cửa sổ tính từ `Cell_Size × Col/Row` nên luôn vừa khít lưới. 96x54 ô 20px = 1920x1080 bị taskbar che; tạm dùng 60x34.

## 9. Git
- `git init` phải đứng **đúng thư mục project** (lỗi Permission denied là do đứng ở System32).
- `.gitignore`: `.vs/`, `x64/`, `Debug/`, `Release/`, `*.user`
- Quy trình mỗi bước nhỏ: `git add .` → `git commit -m "..."` → `git push`
- Message commit ngắn, đúng việc vừa làm.
- Lỗi 403 khi push: Windows lưu sai tài khoản GitHub. Xóa `git:https://github.com` trong Credential Manager rồi đăng nhập lại.

## 10. Cách học (nguyên tắc)
- Học tới đâu làm tới đó, ~20% học / 80% code.
- Làm game nhỏ và **hoàn thành**, không sa đà dự án to.
- Tự viết, tự suy luận trước, bí mới hỏi.
- Làm xong Snake mới làm Pong.

## 11. Tiến độ Snake
- [x] Cửa sổ, hằng số lưới
- [x] Vẽ lưới
- [x] Đầu rắn lưu theo ô, vẽ ở giữa
- [ ] Di chuyển bằng bộ đếm giờ + đổi hướng bằng phím
- [ ] Thân rắn (`std::vector<Snake_Cell>`)
- [ ] Mồi và ăn mồi
- [ ] Thua cuộc (đụng tường, đụng thân)
- [ ] Điểm số, Game Over, chơi lại (`enum GameState` + `switch`)

## NOTE:
- Truy về ô gần giữa: Với số chẵn sẽ là ô đầu của nửa sau (vd 10 / 2 = 5 = > chỉ số của ô thứ 6 => vẫn là 1 ô giữa vì số ô là chẵn sẽ ko có ô giữa tuyệt đối), không cần - 1 cho chỉ số vì vẫn truy ra ô giữa (chỉ số 4 cho ô thứ 5). Số lẻ vì theo kiểu int sẽ làm tròn xuống.VD: 5 / 2 = 2.5 -> 2 => chỉ số của ô giữa tuyệt đối. Chỉ số ô chạy từ 0 -> số ô - 1.
- Tọa độ của ô = chỉ số cột (bắt đầu từ 0) => tính bằng cột số bao nhiêu - 1, ví dụ cột thứ 4 có chỉ số là 3. 