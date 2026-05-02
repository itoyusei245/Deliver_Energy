/**
 * @file GetItem.h
 * @brief 収集アイテム（コイン）のクラス定義
 */
#pragma once

 /**
  * @class GetItem
  * @brief プレイヤーが収集可能なコインクラス
  * @details
  * 出現後に重力落下し、レイキャストで地面に着地します。
  * プレイヤーとの接触判定を行い、取得されるとスコア（静的メンバ）を加算して消滅します。
  * また、出現から一定時間経過すると点滅を開始し、最終的に消滅するロジックを持ちます。
  */
class GetItem : public IGameObject
{
public:
	GetItem();
	~GetItem();

	/**
	 * @brief 初期化処理
	 * @param[in] pos 出現させる初期座標
	 */
	void Init(const Vector3& pos);

	/**
	 * @brief 更新処理
	 * @details 重力落下、接地判定、プレイヤーとの当たり判定、回転アニメーション、点滅・消滅処理を行います。
	 */
	void Update()override;

	/**
	 * @brief 描画処理
	 * @details 点滅処理により、m_isVisibleがtrueのときのみ描画します。
	 */
	void Render(RenderContext& rc)override;


public:
	/**
	 * @brief 現在のコイン獲得総数を取得する
	 * @return int 獲得枚数
	 */
	static int GetCoinCount() { return m_totalCoinCount; }

	/**
	 * @brief コイン獲得総数をリセットする
	 * @details ゲーム開始時などに呼び出してください。
	 */
	static void ResetCoinCount() { m_totalCoinCount = 0; }


private:
	/** コインの3Dモデル */
	ModelRender m_coin;

	/** 当たり判定用オブジェクト */
	CollisionObject* m_collision = nullptr;

	/** 現在の座標 */
	Vector3 m_position = Vector3::Zero;

	/** 現在の速度（落下用） */
	Vector3 m_velocity = Vector3::Zero;

	/** * @brief 獲得したコインの総数
	 * @note 全インスタンスで共有される静的変数です。
	 */
	static int m_totalCoinCount;

private:
	/** 回転アニメーション用角度 */
	float m_rotation = 0.0f;

	/** 重力加速度 */
	float m_gravity = 9.8f;

	/** 接地フラグ */
	bool m_isOnGround = false;


private:
	/**タイマー・点滅管理*/

	/** 出現してからの経過時間 */
	float m_spawnTime = 0.0f;

	/** 点滅制御用タイマー（表示・非表示の切り替え間隔計測） */
	float m_blinkTimer = 0.0f;

	/** 点滅フェーズに入ったかどうか */
	bool m_blinkStarted = false;

	/** 現在の表示状態（点滅中に切り替わる） */
	bool m_isVisible = true;

	//多重削除を防ぐためのフラグ
	bool m_isDead = false;
};