//! @file	Cat_Render.h
// 描画関連

#ifndef INCL_Cat_Base64_h
#define INCL_Cat_Base64_h

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//! 初期化パラメータ
enum {
	CAT_RENDER_PARAM_FORMAT_RGBA5650 = 0,					/*!< 16bit					*/
	CAT_RENDER_PARAM_FORMAT_RGBA5551 = 2,					/*!< 15bit + alpha 1		*/
	CAT_RENDER_PARAM_FORMAT_RGBA8888 = 3,					/*!< 24bit + alpha 8		*/
	CAT_RENDER_PARAM_FORMAT_MASK     = (3UL << 0),

	CAT_RENDER_PARAM_BUFFER_DOUBLE   = (0UL << 2),			/*!< ダブルバッファ			*/
	CAT_RENDER_PARAM_BUFFER_SINGLE   = (1UL << 2),			/*!< シングルバッファ		*/
	CAT_RENDER_PARAM_BUFFER_MASK     = (1UL << 2),

	CAT_RENDER_DEFAULT = (CAT_RENDER_PARAM_FORMAT_RGBA8888 | CAT_RENDER_PARAM_BUFFER_DOUBLE),	/*!< デフォルト設定			*/
};

//! 描画関連を初期化する
/*!
	@param[in]	nParam		CAT_RENDER_PARAM_xxxの論理輪
*/
extern void Cat_RenderInit( uint32_t nParam );

//! 描画関連の終了処理をする
extern void Cat_RenderTerm( void );

//! 描画を開始する
extern void Cat_RenderBegin( void );

//! 描画を終了する
extern void Cat_RenderEnd( void );

//! 画面を更新する
extern void Cat_RenderScreenUpdate( void );

#ifdef __cplusplus
}
#endif

#endif // INCL_Cat_Base64_h
