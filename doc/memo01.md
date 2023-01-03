# 色々覚え書き
## ソフトリセット
### 7.7 Software reset
The Software Reset Call (SWRST Call) allows all the devices in the I2 C-bus to be reset to
the power-up state value through a specific formatted I2C-bus command. To be performed
correctly, it implies that the I2C-bus is functional and that there is no device hanging the
bus.
The maximum wait time after software reset is 1 ms.
The SWRST Call function is defined as the following:
1. A START command is sent by the I2C-bus master.
2. The reserved General Call address ‘0000 000’ with the R/W bit set to ‘0’ (write) is sent
by the I2C-bus master.
3. The PCA9956B device(s) acknowledge(s) after seeing the General Call address
‘0000 0000’ (00h) only. If the R/W bit is set to ‘1’ (read), no acknowledge is returned to
the I 2C-bus master.
4. Once the General Call address has been sent and acknowledged, the master sends
1 byte with 1 specific value (SWRST data byte 1):
a. Byte 1 = 06h: the PCA9956B acknowledges this value only. If byte 1 is not equal to
06h, the PCA9956B does not acknowledge it.
If more than 1 byte of data is sent, the PCA9956B does not acknowledge any more.
5. Once the correct byte (SWRST data byte 1) has been sent and correctly
acknowledged, the master sends a STOP command to end the SWRST function: the
PCA9956B then resets to the default value (power-up value) and is ready to be
addressed again within the specified bus free time (t BUF).

### 7.7 ソフトウェアリセット
ソフトウェア リセット コール (SWRST コール) により、I2 C バス内のすべてのデバイスをリセットすることができます。
特定のフォーマットされた I2C バス コマンドを介して電源投入時の状態値を取得します。 実行されます
正しく、I2C バスが機能しており、バスをぶら下げているデバイスがないことを意味します。
バス。
ソフトウェアリセット後の最大待ち時間は 1 ms です。
SWRST Call 関数は次のように定義されます。
1. I2C バス マスターから START コマンドが送信されます。
2. R/W ビットが「0」(書き込み) に設定された予約済みゼネラル コール アドレス「0000 000」が送信されます。
I2Cバスマスターによって。
3. PCA9956B デバイスは、ゼネラル コール アドレスを確認した後に応答します。
‘0000 0000’ (00h) のみ。 R/W ビットが「1」(読み取り) に設定されている場合、Ack は返されません。
I 2C バス マスター。
4. ゼネラル コール アドレスが送信されて確認されると、マスターは送信します。
1 つの特定の値を持つ 1 バイト (SWRST データ バイト 1):
を。 バイト 1 = 06h: PCA9956B はこの値のみを認識します。 バイト 1 が等しくない場合
06h、PCA9956B はそれを認識しません。
1 バイトを超えるデータが送信されると、PCA9956B はそれ以上確認応答しません。
5. 正しいバイト (SWRST データ バイト 1) が正しく送信されたら、
マスターは STOP コマンドを送信して SWRST 機能を終了します。
その後、PCA9956B はデフォルト値 (パワーアップ値) にリセットされ、すぐに使用できる状態になります。
指定されたバス空き時間 (t BUF) 内に再度アドレス指定されます。