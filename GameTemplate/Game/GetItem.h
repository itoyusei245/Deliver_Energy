#pragma once
class GetItem : public IGameObject
{
public:
	GetItem();
	~GetItem();

	void Init(const Vector3& pos);
	void Update();
	void Render(RenderContext& rc);


public:
	/**現在のコイン枚数を取得する関数*/
	static int GetCoinCount() { return m_totalCoinCount; }
	/**コイン枚数をリセットする関数*/
	static void ResetCoinCount() { m_totalCoinCount = 0; }


private:
	ModelRender m_coin;
	CollisionObject* m_collision = nullptr;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_velocity = Vector3::Zero;
	static int m_totalCoinCount;

private:
	float m_rotation = 0.0f;
	float m_radius = 20.0f;
	float m_gravity = -9.8f;
	bool m_isOnGround = false;


private:
	/**タイマー・点滅管理*/
	float m_spawnTime = 0.0f;     /**出現からの経過時間*/
	float m_blinkTimer = 0.0f;    /**点滅タイマー*/
	bool m_blinkStarted = false;  /**点滅が開始されたか*/
	bool m_isVisible = true;      /**現在表示中かどうか*/
	float m_alpha = 1.0f;         /**透明度（将来フェード用にも使える）*/
};
