/**
 * @file GameStateBase.h
 * @brief ゲームステート（状態）の基底クラス定義
 */
#pragma once
#include <memory>

 /**
  * @class GameStateBase
  * @brief ゲームの各状態（シーンやフェーズ）の共通基底クラス
  * @details IGameObjectを継承し、ステートのライフサイクル（Enter, Update, Exit）と
  * 状態遷移（ChangeState）の機能を提供します。
  * 派生クラスでは OnEnter, OnUpdate, OnExit を実装して具体的な振る舞いを記述します。
  */
class GameStateBase : public IGameObject
{
public:
	/**
	 * @brief コンストラクタ
	 */
	GameStateBase() = default;
	virtual ~GameStateBase() = default;

	/**
	 * @brief 開始処理
	 * @details 初期状態では非アクティブ(Deactivate)に設定します。
	 * @return bool 常にtrue
	 */
	bool Start() override;

	/**
	 * @brief 描画処理
	 * @note 必要であれば派生クラスでオーバーライドします。
	 */
	void Render(RenderContext& rc) override;

	/**
	 * @brief ステート遷移が必要か判定する
	 * @return bool 遷移すべきならtrue
	 * @note 派生クラスで遷移条件（ボタン入力など）を実装してください。
	 */
	virtual bool ShouldChangeState() = 0;

	/**
	 * @brief ステートを変更する
	 * @details 現在のステートを非アクティブにし、次のステートを検索してアクティブにします。
	 * @return GameStateBase* 次のステートへのポインタ
	 */
	virtual GameStateBase* ChangeState() final;

	virtual void Update() final;
	virtual void OnUpdate() = 0;

	virtual void Enter() final;
	virtual void OnEnter() = 0;

	virtual void Exit() final;
	virtual void OnExit() = 0;

	/**
	 * @brief 次に遷移するステート名を設定する
	 * @param[in] nextStateName 遷移先オブジェクトの名前
	 */
	virtual void SetNextName(const char* nextStateName) final;

	/**
	 * @brief 次のステートを取得する（所有権移動）
	 * @note unique_ptrを移動させて返す実装になっています。
	 */
	virtual  std::unique_ptr<GameStateBase> NextState() final;


	/**
	 * @brief アクティブ状態を設定する
	 * @param[in] flag trueならアクティブ、falseなら非アクティブ
	 */
	void SetActive(bool flag);

	/** 次のステート（スマートポインタ管理用） */
	std::unique_ptr<GameStateBase> m_nextState = nullptr;

	/** 次のステートの名前（FindGOによる検索用） */
	const char* m_nextStateName = nullptr;

private:
	/** アクティブフラグ（Updateを実行するかどうかの制御） */
	bool m_isActive = false;
};