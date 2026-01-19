/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[�B
 */


//
cbuffer circleCb : register(b1)
{
    float progress;
    float size;
    float scale;
};
//

cbuffer cb : register(b0){
	float4x4 mvp;		
	float4 mulColor;	
};
struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0);	
sampler Sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}

static const float PI = 3.14159265f;

float4 PSMain(PSInput In) : SV_Target0
{
    // 1. まず元の画像の色を取得
    float4 color = colorTexture.Sample(Sampler, In.uv);

    // 2. 透明な部分はそもそも描画しないならここでclip
    clip(color.a - 0.01f);

    // 3. 中心の座標 (UV座標の中心は 0.5, 0.5)
    float2 center = float2(0.5f, 0.5f);
    
    // 4. 現在のピクセルの中心からのベクトル
    float2 vectorFromCenter = In.uv - center;

    // 5. 角度を計算 (atan2を使うと一発で角度が出ます)
    // atan2(y, x) は -PI ～ +PI を返します。
    // 時計回りで、上(12時)を開始位置にするための調整： atan2(x, -y)
    float angle = atan2(-vectorFromCenter.x, -vectorFromCenter.y);

    // 6. 角度を 0.0 ～ 1.0 の範囲に変換 (0=12時, 0.5=6時, 1.0=一周)
    // atan2の結果は -PI~PI なのでそれを 0~1 にマッピング
    if (angle < 0)
    {
        angle += 2.0f * PI;
    }
    float currentRatio = angle / (2.0f * PI);

    // 7. 進捗(progress)より大きい角度の部分は描画しない
    // progress(a) が 1.0 なら満タン、0.5なら半分
    if (currentRatio > progress)
    {
        clip(-1); // 描画しない
    }

    // 色を返す（元の画像の色を使いたい場合）
    return color;
    
    // ※もし緑一色に塗りつぶしたいなら以下
    // return float4(0.0f, 1.0f, 0.0f, color.a); 
}

float4 PSMainGamma(PSInput In) : SV_Target0
{
    float4 color = colorTexture.Sample(Sampler, In.uv) * mulColor;
    color.xyz = pow(max(color.xyz, 0.0001f), 1.0f / 2.2f);
    return color;
}