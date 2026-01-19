/**
 * @file Countdown.h
 * @brief ゲーム開始時のカウントダウン演出クラス定義
 */
#pragma once

 /**
  * @class Countdown
  * @brief ゲーム開始前のカウントダウンUIを表示・管理するクラス
  * @details "Ready" -> "5" -> "4" -> ... -> "GO" の順で画像を切り替えながら表示します。
  * 演出が終了するとフラグを立て、自身を削除します。
  */
class Countdown : public IGameObject
{
public:
    /**
     * @brief コンストラクタ
     */
    Countdown();

    /**
     * @brief デストラクタ
     */
    ~Countdown();

    /**
     * @brief 更新処理
     * @details タイマーを進め、経過時間に応じて表示する画像（Ready, 数字, GO）を切り替えます。
     * 演出終了後は自動的に自身を削除(DeleteGO)します。
     */
    void Update() override;

    /**
     * @brief 描画処理
     * @param[in,out] rc 描画コンテキスト
     */
    void Render(RenderContext& rc) override;

    /**
     * @brief カウントダウン演出が終了したかどうかを取得する
     * @return bool 終了していればtrue
     */
    bool IsFinished() const { return m_isFinished; }

private:
    /** 操作説明などの背景画像 */
    SpriteRender m_operation;

    /** カウントダウン（数字や文字）表示用スプライト */
    SpriteRender m_countdown;

    /** 経過時間計測用タイマー */
    float m_timer = 0.0f;

    /** 演出終了フラグ */
    bool m_isFinished = false;

    /** 現在の状態ID（※現状の実装ではUpdate内でif文分岐しているため未使用の可能性あり） */
    int m_currentState = -1;

    // --- 画像パス定数 ---
    /** 操作説明用画像パス */
    const char* TEX_OPERATION = "Assets/sprite/countDownOperation.DDS";
    /** "Ready" 画像パス */
    const char* TEX_READY = "Assets/sprite/countDown_Ready.DDS";
    const char* TEX_5 = "Assets/sprite/5.DDS";
    const char* TEX_4 = "Assets/sprite/4.DDS";
    const char* TEX_3 = "Assets/sprite/3.DDS";
    const char* TEX_2 = "Assets/sprite/2.DDS";
    const char* TEX_1 = "Assets/sprite/1.DDS";
    /** "GO" 画像パス */
    const char* TEX_GO = "Assets/sprite/countDown_GO.DDS";
};