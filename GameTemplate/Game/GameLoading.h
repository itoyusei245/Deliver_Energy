/**
 * @file GameLoading.h
 * @brief ゲームロード画面を管理するクラスの定義
 */
#pragma once

class GameLoading : public IGameObject
{
public:
    GameLoading();
    ~GameLoading();

    bool Start() override;
    void Update() override;

private:
    // ロード開始までの待ち時間タイマー
    int m_timer = 0;
};