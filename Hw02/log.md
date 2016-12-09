## 20161209
### 待修改
Block 要能夠自由決定長寬，因問尺寸有不同才出問題

### 問題
`Block` 結構內的指標是沒有const的結果發生問題了

```cpp
imgraw::ImrBlock::ImrBlock(const imgraw& img, imint pos){
    this->pos_y=((pos/64)*4);
    this->pos_x=((pos%64)*4);
    img_p.resize(16);
    for (int j=0, c=0; j < 4; ++j){
        for (int i = 0; i < 4; ++i){
            img_p[c++]=&img.at2d(j+pos_y, i+pos_x);
        }
    }
}
```

for最裡面的那行 `img_p` 沒有 `const` 
可是輸入的 `img` 有 `const` 這不知道該怎麼解決

暫時先把 `void get_org(const imgraw &sou);`
這個中間的const拿掉了回頭在處理


##20161208
### inner class解決
利用inner class解決了結構交互使用的問題，並成功實現了第二個operator功能

> imgraw img1, img2;
> img1=img2;
> img1.blockCopy()=img2.blockCopy();

這兩個等於居然可以是不同的方式
一個把全部點複製過去
一個是只複製特定區塊

作法：

`Block` 結構建立在 `imgraw` 裡面
`Block` 結構儲存imgraw的區塊內的指標並重載等於

做法是在 `blockCopy(){..}` 裡面建立 `Block` 並將需要複製的點指標
全部儲存到裡面，然後重載[]返回第幾個指標

在 `Block` operator=() 裡面將指標一個一個取值複製過去。