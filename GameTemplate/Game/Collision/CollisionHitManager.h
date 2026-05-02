/**
 * @file CollisionHitManager.h
 * @brief 当たり判定とその結果処理を管理するクラス定義
 */
#pragma once

 /** * @class CollisionHitManager
  * @brief コリジョンヒット監視クラス
  * @note シングルトンパターンで実装され、特定のゲームオブジェクト間（プレイヤー対敵、プレイヤー対トリガーなど）の
  * 衝突判定と、衝突時のイベント処理（ダメージ、ステージ生成など）を一括管理します。
  */
class CollisionHitManager
{
private:
    CollisionHitManager() {}
    ~CollisionHitManager() {}

public:
    /**
     * @brief 更新処理
     * @details 毎フレーム呼び出され、オブジェクト間の衝突チェックを実行します。
     */
    void Update();

    /**
     * シングルトン用インスタンス
     */
private:
    static CollisionHitManager* m_instance;

public:
    /**
     * @brief インスタンスを取得する
     * @return CollisionHitManager* インスタンス
     */
    static CollisionHitManager* GetInstance()
    {
        return m_instance;
    }

    /**
     * @brief インスタンスを生成する
     */
    static void CreateInstance()
    {
        if (!m_instance)
            m_instance = new CollisionHitManager();
    }

    /**
     * @brief インスタンスを削除する
     */
    static void DeleteInstance()
    {
        if (m_instance)
        {
            delete m_instance;
            m_instance = nullptr;
        }
    }
};


/**
 * @class CollisionHitManagerObject
 * @brief CollisionHitManagerをゲームオブジェクトとして動作させるためのラッパークラス
 * @details IGameObjectを継承し、ゲームのUpdateループ内でManagerのUpdateを呼び出します。
 * また、自身の生存期間に合わせてManagerの生成・破棄（Create/DeleteInstance）を管理します。
 */
class CollisionHitManagerObject : public IGameObject
{
public:
    CollisionHitManagerObject() {}
    ~CollisionHitManagerObject()
    {
        // オブジェクト破棄時にマネージャも破棄
        CollisionHitManager::DeleteInstance();
    }

    /**
     * @brief 開始処理
     * @details マネージャのインスタンスを生成します。
     */
    bool Start() override
    {
        CollisionHitManager::CreateInstance();
        return true;
    }

    /**
     * @brief 更新処理
     * @details マネージャのUpdateを実行します。
     */
    void Update() override
    {
        CollisionHitManager::GetInstance()->Update();
    }

    void Render(RenderContext& rc) override {}
};