Dưới đây là lời giải của nhóm mình cho bài toán Flow-game.

Thuật toán của nhóm mình gồm 2 bước chính:
1. Tối ưu số lượng cặp nối được
2. Tối ưu số ô đi qua. 

## 1. Tối ưu số lượng cặp nối được

Ở bước này, bọn mình sẽ cố nối nhiều cặp màu nhất có thể. Thay vì nối với thứ tự tùy ý, bọn mình sẽ ưu tiên nối các cặp ô cùng màu mà nằm gần nhau [[1](#1)] trước. Lý do bọn mình ưu tiên các cặp ô nằm gần nhau trước là vì nó sẽ giúp bọn mình còn lại nhiều ô trống cho các cặp sau. 

Cách nối thì với mỗi cặp ô cùng màu, bọn mình sẽ tìm đường đi giữa 2 ô bằng thuật toán BFS.

Do test của BTC có phần khá lạ đó là có những test tất cả các ô đều có màu (không có ô trống nào), điều này khiến bước 1 này bọn mình dùng 2 cách code khác nhau để dành do 2 đối tượng test khác nhau [[2](#2)]:
- Với các test mà các ô nằm rời rạc nhau (nghĩa là số lượng ô màu không quá nhiều), thì khi tìm đường đi giữa 2 ô bằng BFS, bọn mình KHÔNG cho phép đường đi đó đi qua các ô có màu khác.
- Với các test mà phần lớn các ô đều có màu, thì khi tìm đường đi giữa 2 ô, bọn mình cho phép đường đi đó đi qua các ô có màu khác (nhưng không cho phép đi qua các ô đã tìm được đường đi)

## 2. Tối ưu số lượng ô đi qua

Sau khi hoàn thành bước 1, ở bước 2 này thì đơn giản hơn, bọn mình chỉ dùng 1 thuật toán cho mọi loại test:
- Với mỗi đường đi tìm được, bọn mình sẽ đi trên đường đi đó, với 2 cặp ô kề nhau, bọn mình sẽ nhìn sang 4 hướng xem các hướng đó có là ô trống hay không. Nếu hướng đó là ô trống thì bọn mình sẽ "nở" đường đi đó sang ô đó. 

## Note

### 1
Gần nhau ở đây bọn mình có dùng 2 cách:
- Theo [khoảng cách Euclid](https://en.wikipedia.org/wiki/Euclidean_distance)
- Theo [khoảng cách Manhattan](https://en.wikipedia.org/wiki/Taxicab_geometry)

### 2
Bọn mình không code cả 2 thứ này vào cùng 1 file, mà ban đầu tụi mình chạy hết các test với thuật toán 1 trước, sau đó lưu kết quả lại, rồi code đè lên thuật toán 2.

### 3
Trong code thì ở một số nơi bọn mình có dùng random để hi vọng rằng kết quả sẽ tốt hơn, ví dụ ở bước 2, bước mở rộng các đường đi thì bọn mình random thứ tự duyệt các hướng. Hoặc ban đầu thì thứ tự nối các cặp ô của tụi mình ở bước 1 là random chứ không phải là sắp xếp theo khoảng cách tăng dần. 

### 4
Ở lần thử đầu tiên, số điểm bọn mình đạt được là khoảng 11,7 điểm (Đủ để vô địch), nhưng do bọn mình không biết được bảng xếp hạng nên cố gắng optimize thêm. 

Thuật toán ở bước 2 của tụi mình khá tốt, mình thấy các test thì bọn mình đều phủ được hơn 90% số lượng các ô. Do đó, bọn mình tập trung vào tối ưu bước 1 nhiều hơn, còn bước 2 gần như không thay đổi gì.

Dù vậy, bước 1 vẫn là chỗ bottleneck lớn nhất của thuật toán, vì với những test mà số lượng màu quá nhiều thì chỉ nối được không quá 1% của số lượng cặp. Dù đã thử nhiều cách, random các thứ nhưng cũng không lên được quá nhiều. 

Sau khi thay đổi nhiều thứ, thì cũng chỉ tăng được điểm từ 11,7 lên khoảng 13,2 điểm.