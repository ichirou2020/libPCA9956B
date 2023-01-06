# libPCA9956Bライブラリ
## 次期えるいーだーのライブラリとしてなんとなく作ってみた
### とは言え次期えるいーだーはしばらく作る予定は無い
#### まあ使いたい人は使ってください

このライブラリはPCA9956Bをとにかく光らせるために作ったので、エラートラップなどが入っていません
自分で使うときはそこを調整してください

#### 開発環境について

VSCode + platform io で開発しています、そのためarduinoIDEで使用する場合は何かしら修正が必要になる可能性があります
そこは自分で調整してください

```
	_drv->start(20);//これでスタート(この例の場合だとLED毎に20mAの電流を供給する)

	led_pwn　関数でLED毎に明るさ指定
	led_on	関数は指定したLED番号をフル発光
	led_off 関数は指定したLEDを消灯

```

### その他
sda,sdcのプルアップ抵抗はこの例だと不要です
esp32のwireライブラリは内部の抵抗を使用してプルアップします