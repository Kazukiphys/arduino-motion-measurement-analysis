/*
  Ultrasonic sensor distance + velocity calculation
  x-t graph and v-t graph output version

  Board: Arduino Uno R4
*/

// ===== 超音波センサのピン設定 =====
const int echoPin = 3;
const int trigPin = 4;

void setup() {

  Serial.begin(9600);

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void loop() {

  // ===== 経過時間 =====
  static unsigned long startTime = millis();

  // ===== サンプリング時間管理 =====
  static unsigned long prevTime = 0;

  unsigned long nowTime = micros();

  float time_interval =
      (nowTime - prevTime) / 1000.0;   // ms

  prevTime = nowTime;

  // ===== 現在時刻[s] =====
  float t =
      (millis() - startTime) / 1000.0;

  // ===== 距離履歴 =====
  static float kyori1 = 0;
  static float kyori2 = 0;
  static float kyori3 = 0;

  // ===== 平滑化距離履歴 =====
  static float smoothkyori1 = 0;
  static float smoothkyori2 = 0;
  static float smoothkyori3 = 0;

  // ===== 距離取得 =====
  float range = readDistance();

  // ===== 移動平均による平滑化 =====
  float smoothrange =
      0.1 * range +
      0.2 * kyori1 +
      0.3 * kyori2 +
      0.4 * kyori3;

  // ===== 最小二乗法による速度計算 =====

  // 時間データ(ms)
  float x[] = {
    0,
    time_interval,
    time_interval * 2
  };

  // 距離データ(m)
  float y[] = {
    smoothkyori2 / 100.0,
    smoothkyori1 / 100.0,
    smoothrange / 100.0
  };

  float A00 = 0;
  float A01 = 0;
  float A02 = 0;
  float A11 = 0;
  float A12 = 0;

  for (int i = 0; i < 3; i++) {

    A00 += 1.0;
    A01 += x[i];
    A02 += y[i];
    A11 += x[i] * x[i];
    A12 += x[i] * y[i];
  }

  float denominator =
      (A00 * A11 - A01 * A01);

  float lsmVelocity = 0;

  // ===== ゼロ除算防止 =====
  if (abs(denominator) > 0.000001) {

    lsmVelocity =
        (A00 * A12 - A01 * A02)
        / denominator;

    // ms → s 変換
    lsmVelocity *= 1000.0;
  }

  // ===== シリアル出力 =====
  // 時間[s], 距離[cm], 速度[m/s]

  Serial.print(t);
  Serial.print(",");

  Serial.print(smoothrange);
  Serial.print(",");

  Serial.println(lsmVelocity);

  // ===== データ更新 =====
  kyori3 = kyori2;
  kyori2 = kyori1;
  kyori1 = range;

  smoothkyori3 = smoothkyori2;
  smoothkyori2 = smoothkyori1;
  smoothkyori1 = smoothrange;

  delay(10);
}

// ===== 距離測定関数 =====
float readDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  float distance =
      pulseIn(echoPin, HIGH) / 58.0;

  return distance;
}