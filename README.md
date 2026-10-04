# Arduino Motion Measurement Analysis

Arduino UNO R4 と HC-SR04 超音波センサーで距離・速度を測定し、ブラウザー上でグラフ表示・解析するプロジェクトです。

## Features

- x-t グラフと v-t グラフの切り替え
- 複数実験の記録、表示切り替え、重ね合わせ
- グラフ範囲の移動と範囲指定
- 選択範囲の近似直線と R² の表示
- v-t グラフでの変位（速度の時間積分）の表示
- 測定値の CSV 出力

## Hardware

- Arduino UNO R4
- HC-SR04

| HC-SR04 | Arduino UNO R4 |
| --- | --- |
| VCC | 5V |
| Trig | D4 |
| Echo | D3 |
| GND | GND |

## Requirements

- Arduino IDE
- Web Serial API に対応したデスクトップ版 Google Chrome または Microsoft Edge

## Getting Started

1. `ultrasonic_sensor_HC-SR04/ultrasonic_sensor_HC-SR04.ino` を Arduino IDE で開き、Arduino UNO R4 に書き込みます。
2. ブラウザーで `index.html` を開きます。
3. 「測定開始」を押し、Arduino のシリアルポートを選択します。
4. 記録を終えたら CSV を書き出します。

シリアル通信は 9600 baud です。ポートを選択できない場合は、Arduino IDE のシリアルモニターなど、同じポートを使用するアプリケーションを閉じてください。

## Measurement Data

Arduino はサンプルごとに、次の順でカンマ区切りの値を出力します。

```text
time_s,distance_cm,velocity_m_s
```

距離は平滑化したセンサーと物体間の距離です。総移動距離ではありません。CSV では距離をメートルに換算して出力します。
