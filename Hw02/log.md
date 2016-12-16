## 20171216
### 完成了
可回頭一看似乎沒必要建立block物件，太耗時了
但也因為這樣，學到了好多好多東西。
記得之後要這種轉接方式千萬要考慮好。

### 返回函式內建立的物件
`imgraw & imgraw::tra_code()`
嘗試想在函式內建構一個新`imgraw`結果發現這個不能反出
> 盡可能把需反出的東西設置在this




## 20171214
### inner結構問題
內層結構內要儲存外層結構資料，可以利用指針儲存，因為外層一定會先建好
外成結構如果要儲存內層結構資料只能用value不然就要new了，因為沒有任何地方建立過

```cpp
class out{
    class in{
        vector<out*> data;
    }
    vector<in> data;
}
```

### 重載等號的公式
http://en.cppreference.com/w/cpp/language/operators

### 優化效能2
> 沒事真不要亂重載等號，搞得很多地方要用到等號出了大問題
> 真要重載邊萬要想清楚標記清楚，出問題先找她。

幹 完成了，成功預建會用到的結構

```cpp
int imgraw::ImrBlock::dif_squ(imgraw& img){
    // 建立區塊
    // auto&& img_b=img.block(j);
    auto&& img_b=img.blk_p[j];
}
```




## 20161213
分兩段寫居然會當掉，寫一起就不會

```cpp
imgraw::ImrBlock a= idx.block(0);
// a= idx.block(0);
```

猜測可能是.block()返回的右值這個鏈接點斷了
但是如果寫在一起，相當於不用透過鏈接點，就是
`imgraw img=imgraw(0, 0);`

這個等於其實是沒作用的，優化會直接把它拿掉。

**找到了**
因為我重載了 `operator=` 得想辦法共存才行

乾脆把真正的 = 功能改成 fun?
然後把ImrBlock藏到private，這樣使用者就沒機會用等號了
- 得仔細思考會造成那些誤解，那些其他使用上直觀的問題

> 下次重載等號要非常小心標記、註解...自己採坑了

> 什麼時候需要指針：
> 當你重載等號時無法依靠等號事後建立初值時，又需要事後建初值
> 只能乾脆用指針了，事後new的時候一起建初值




## 20161212
### 優化居然差了整整5倍多

原本

```cpp
// 計算 差平方和
for (int i = 0; i < 16; ++i){
    num+=pow(((int)(*this)[i] - 
        (int)img.block(j)[i]), 2);
}
```

優化後

```cpp
// 建立區塊
auto&& img_b=img.block(j);
// 計算差平方和
for (int i = 0; i < 16; ++i){
    num+=pow(((int)(*this)[i] - 
        (int)img_b[i]), 2);
}
```

把時間壓到1.8秒了原本大概要10秒




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



