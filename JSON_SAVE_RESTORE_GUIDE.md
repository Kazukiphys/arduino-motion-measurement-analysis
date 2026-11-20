# JSON保存・復元機能ガイド

## 概要

このアプリケーションにJSONファイルを使用して、実験データと作業状態を保存・復元する機能を追加しました。これにより、測定を中断して後日続けることができます。

## 新しいボタン

### 1. 「JSONを保存」ボタン
- **場所**: 右側のパネルの MEASUREMENT セクション
- **機能**: 現在の実験データと UI状態をJSONファイルとして保存
- **有効化条件**: サンプルデータが1つ以上ある場合のみ有効

### 2. 「JSONから復元」ボタン
- **場所**: 右側のパネルの MEASUREMENT セクション
- **機能**: 以前保存したJSONファイルを読み込んで復元
- **制限事項**: 測定中（capturing = true）の場合は読み込み不可

## 保存されるデータ

JSONファイルには以下の情報が含まれます：

```json
{
  "version": 1,
  "timestamp": "2026-11-20T01:06:02.574Z",
  "experiments": [
    {
      "id": 1,
      "name": "実験1",
      "color": "#146b50",
      "graphVisible": true,
      "samples": [
        {
          "time": 0,
          "timeMicros": 0,
          "distance": 50,
          "velocity": 0.5
        },
        ...
      ]
    }
  ],
  "state": {
    "nextExperimentNumber": 3,
    "selectedExperimentId": null,
    "graphMode": "xt",
    "interactionMode": "select",
    "timeSpan": 10,
    "velocityLimit": 5,
    "positionLimit": 2,
    "viewOffset": 0,
    "velocityViewOffset": 0,
    "positionViewOffset": 0
  }
}
```

### 保存される内容

#### experiments 配列
- **id**: 実験番号
- **name**: 実験の名前（例: "実験1"）
- **color**: グラフの線の色
- **graphVisible**: グラフに表示するかどうか
- **samples**: 測定データ
  - time: 経過時間（秒）
  - timeMicros: マイクロ秒単位の時間
  - distance: 距離（cm）
  - velocity: 速度（m/s）

#### state オブジェクト
- **nextExperimentNumber**: 次の実験番号
- **selectedExperimentId**: 現在選択中の実験
- **graphMode**: グラフの種類（"xt" = 位置-時間、"vt" = 速度-時間）
- **interactionMode**: インタラクションモード（"pan" = 範囲移動、"select" = 範囲指定）
- **timeSpan**: 時間軸のスケール
- **velocityLimit**: 速度軸の上限値
- **positionLimit**: 位置軸の上限値
- **viewOffset**: 水平表示オフセット
- **velocityViewOffset**: 速度軸垂直オフセット
- **positionViewOffset**: 位置軸垂直オフセット

## 使い方

### 測定結果を保存する

1. 測定を完了して、一時停止をクリック
2. 「JSONを保存」ボタンをクリック
3. JSONファイルが自動的にダウンロードされます
4. ファイル名は `motion-YYYY-MM-DDTHH-mm-ss-sssZ.json` の形式です

### 保存したデータを復元する

1. アプリケーションを開く
2. 「JSONから復元」ボタンをクリック
3. ファイル選択ダイアログが開きます
4. 以前保存したJSONファイルを選択
5. 実験データと UI状態が自動的に復元されます
6. 画面に「X 個の実験データを読み込みました。」というメッセージが表示されます

## 技術的な詳細

### 実装されたJavaScript関数

#### exportToJson()
```javascript
function exportToJson() {
  if (!experiments.length) return;
  const data = {
    version: 1,
    timestamp: new Date().toISOString(),
    experiments: experiments.map((exp) => ({
      id: exp.id,
      name: exp.name,
      color: exp.color,
      graphVisible: exp.graphVisible,
      samples: exp.samples
    })),
    state: {
      nextExperimentNumber,
      selectedExperimentId,
      graphMode,
      interactionMode,
      timeSpan,
      velocityLimit,
      positionLimit,
      viewOffset,
      velocityViewOffset,
      positionViewOffset
    }
  };
  // Blobを作成してダウンロード
}
```

#### importFromJson(file)
```javascript
function importFromJson(file) {
  const reader = new FileReader();
  reader.onload = (event) => {
    try {
      const data = JSON.parse(event.target.result);
      // データの検証と復元
      experiments.length = 0;
      experiments.push(...data.experiments);
      // UI状態の復元
      nextExperimentNumber = data.state.nextExperimentNumber ?? ...;
      // 画面の再描画
      renderExperiments();
      updateReadings();
      updateAnalysis();
      drawGraph();
    } catch (error) {
      setNotice(`JSONファイルの読み込みに失敗しました: ${error.message}`, true);
    }
  };
  reader.readAsText(file);
}
```

### イベントリスナー

```javascript
document.getElementById("export-json-button").addEventListener("click", exportToJson);
document.getElementById("import-json-button").addEventListener("click", () => {
  document.getElementById("json-import-input").click();
});
document.getElementById("json-import-input").addEventListener("change", (event) => {
  if (event.target.files?.length) {
    importFromJson(event.target.files[0]);
    event.target.value = "";
  }
});
```

## 注意事項

1. **測定中の読み込みは不可**: 測定中（capturing = true）の場合、JSONファイルを読み込むことはできません。先に「一時停止」をクリックしてください。

2. **ファイル形式**: JSONファイルの形式が正しくない場合、エラーメッセージが表示されます。

3. **バージョン確認**: JSONファイルには version フィールドが含まれており、将来のバージョン互換性確認に使用できます。

4. **大型データセット**: 非常に大量のサンプルデータがある場合、ブラウザのメモリ制限に達する可能性があります。

## トラブルシューティング

### JSONファイルが読み込めない場合

1. ファイルが実際にJSONファイルであることを確認してください
2. ファイルの形式が正しいことを確認してください（テキストエディタで開いて確認）
3. 測定中でないことを確認してください

### データが復元されない場合

1. ブラウザのコンソールを確認してエラーメッセージを確認してください
2. JSONファイルが破損していないか確認してください
3. アプリケーションをリロードしてから試してください

## 例

### JSONファイルの例

```json
{
  "version": 1,
  "timestamp": "2026-11-20T09:30:00.000Z",
  "experiments": [
    {
      "id": 1,
      "name": "実験1 - 物体の落下",
      "color": "#146b50",
      "graphVisible": true,
      "samples": [
        { "time": 0, "timeMicros": 0, "distance": 100, "velocity": 0 },
        { "time": 0.1, "timeMicros": 100000, "distance": 95, "velocity": 0.5 },
        { "time": 0.2, "timeMicros": 200000, "distance": 85, "velocity": 1.0 },
        { "time": 0.3, "timeMicros": 300000, "distance": 70, "velocity": 1.5 }
      ]
    },
    {
      "id": 2,
      "name": "実験2 - 物体の上昇",
      "color": "#dc713d",
      "graphVisible": true,
      "samples": [
        { "time": 0, "timeMicros": 0, "distance": 50, "velocity": 2.0 },
        { "time": 0.1, "timeMicros": 100000, "distance": 60, "velocity": 1.5 },
        { "time": 0.2, "timeMicros": 200000, "distance": 65, "velocity": 1.0 },
        { "time": 0.3, "timeMicros": 300000, "distance": 65, "velocity": 0 }
      ]
    }
  ],
  "state": {
    "nextExperimentNumber": 3,
    "selectedExperimentId": 1,
    "graphMode": "vt",
    "interactionMode": "select",
    "timeSpan": 10,
    "velocityLimit": 5,
    "positionLimit": 2,
    "viewOffset": 0,
    "velocityViewOffset": 0,
    "positionViewOffset": 0
  }
}
```

## まとめ

JSON保存・復元機能により、以下のことが可能になりました：

✅ 測定結果をJSONファイルで保存
✅ 後日、保存したJSONファイルから復元
✅ 実験データだけでなく、UI状態（グラフモード、スケール等）も復元
✅ 複数の実験データを一度に復元
✅ CSVファイルと同様に使用可能

この機能を活用することで、長期間にわたる実験プロジェクトをより効率的に管理できます。
