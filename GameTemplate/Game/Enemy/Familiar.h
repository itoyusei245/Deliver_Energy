/**
 * @file Familiar.h
 * @brief ボスの眷属（Familiar）クラス定義
 */
#pragma once
#include "EnemyBase.h"
#include "EnemyStatePattern.h"

 /**
  * @enum enFamiliarType
  * @brief 眷属のバリエーションタイプ
  * @note タイプによって見た目（モデル）が異なります。
  */
enum enFamiliarType
{
	enFamiliarType_A, //!< タイプA
	enFamiliarType_B, //!< タイプB
};


/**
 * @class Familiar
 * @brief ボスが召喚する眷属クラス
 * @note EnemyBaseを継承しています。
 * ステートパターンで行動し、プレイヤーに倒されることでボス本体にダメージを与える役割を持ちます。
 */
class Familiar : public EnemyBase
{
	// NOTE: Familiar用のステートパターンでアクセスしたいので例外的にfriendクラスにする
	// これにより、FamiliarIdleStateからこのクラスのprivateメンバへアクセス可能にします。
	friend class FamiliarIdleState;


public:
	/**
	 * @brief コンストラクタ
	 */
	Familiar();

	/**
	 * @brief デストラクタ
	 * @details ステータスとステートマシンのメモリを解放します。
	 */
	virtual ~Familiar();

	/**
	 * @brief 初期化処理
	 * @details タイプに応じたモデルのロード、ステートの生成、物理・コリジョンの設定を行います。
	 * @return bool 成功時true
	 */
	virtual bool Start() override;

	/**
	 * @brief 更新処理
	 * @details ステートの更新と、モデル・物理・コリジョンの座標同期を行います。
	 */
	virtual void Update() override;

	/**
	 * @brief 描画処理
	 * @param[in,out] rc 描画コンテキスト
	 */
	virtual void Render(RenderContext& rc);


public:
	/**
	 * @brief 眷属のタイプを設定する
	 * @param[in] type 設定するタイプ(A or B)
	 * @note Start()が呼ばれる前に設定する必要があります。
	 */
	void Setup(const enFamiliarType type);


public:
	/**
	 * @brief 眷属用ステータスを取得する
	 * @return FamiliarStatus* 眷属ステータスへのポインタ
	 */
	FamiliarStatus* GetStatus() { return dynamic_cast<FamiliarStatus*>(m_status); }


private:
	/** 現在のステートID */
	EnFamiliarStateType m_currentState = enFamiliarStateType_Idle;

	// NOTE:ボスの分身体の状態の箱を作る
	// 配列の中をEnFamiliarStateType_MaxにすることでEnFamiliarStateTypeの中に状態を追加するだけで箱の数を追加できる
	/** ステートインスタンスのリスト */
	IFamiliarState* m_stateList[EnFamiliarStateType_Max];


private:
	/** コア部分のモデル（※現在未使用の可能性あり） */
	ModelRender m_coreModel;
	/** 自身のタイプ（A or B） */
	enFamiliarType m_familiaType;
};