## 20160120
### 資料大小被歸零了
優化代碼後解決了。
原本會調到，變成調整

```cpp
// 檢查大小
imint new_size = ceil((img_vec.filesize*byte)/size);
    if(new_size != (imint)this->filesize) {
        cout << "畫布大小有誤" << endl;
        return;
    }
```

### 主程式不夠精確
改成

```cpp
    img_vec.code(img);
    img.uncode(img_vec);
```
