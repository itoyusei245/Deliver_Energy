#pragma once

/**
 * @class DebugCamera
 * @brief 座標取り用の自由飛行カメラ
 * @details
 * - 左スティック: 前後左右移動
 * - LB/RBボタン: 上下移動
 * - 右スティック: 視点回転
 * - Aボタン: 現在の座標をコンソールに表示（コピペ用）
 */
class DebugCamera : public IGameObject
{
public:
    DebugCamera();
    ~DebugCamera();

    bool Start() override;
    void Update() override;

private:
    void Move();
    void Rotation();
};