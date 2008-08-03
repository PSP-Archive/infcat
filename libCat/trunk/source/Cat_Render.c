//! @file	Cat_Render.h
// 描画関連

//! @todo シングルバッファの時の処理

#include <pspgu.h>
#include <pspdisplay.h>
#include "Cat_Render.h"

//! 実スクリーンサイズ 横幅
#define CAT_SCREEN_WIDTH  (480)
//! 実スクリーンサイズ 縦幅
#define CAT_SCREEN_HEIGHT (272)

//! 描画パケットサイズ(バイト単位。4の倍数であること)
#define PACKETSIZE (1024*1024)

//! 描画パケットバッファ0
static uint32_t __attribute__((aligned(64))) disp_list0[PACKETSIZE/4];
//! 描画パケットバッファ1
static uint32_t __attribute__((aligned(64))) disp_list1[PACKETSIZE/4];
//! 作業用パケットバッファ
static uint32_t __attribute__((aligned(64))) disp_list[1024];

//! 描画パケット切り替え
static int fSW = 0;
//! パケットネストカウンタ
static int nEnter = 0;

//! 描画関連を初期化する
/*!
	@param[in]	nParam		CAT_RENDER_PARAM_xxxの論理輪
*/
void
Cat_RenderInit( uint32_t nParam )
{
	sceGuInit();
	sceDisplayWaitVblankStart();
	sceGuStart( GU_DIRECT, disp_list ); {
		switch(nParam & CAT_RENDER_PARAM_FORMAT_MASK) {
			case CAT_RENDER_PARAM_FORMAT_RGBA8888:
				sceGuDispBuffer( 480, 272, (void*)(512*4*272*0), 512 );
				if(nParam & CAT_RENDER_PARAM_BUFFER_MASK) {
					// single buffer
					sceGuDrawBuffer( GU_PSM_8888, (void*)(512*4*272*0), 512 );
				} else {
					// double buffer
					sceGuDrawBuffer( GU_PSM_8888, (void*)(512*4*272*1), 512 );
				}
				sceGuDepthBuffer( (void*)(512*4*272*2), 512 );	// シングルでもダブルでも深度バッファの位置は変えない
				break;
			case CAT_RENDER_PARAM_FORMAT_RGBA5551:
			case CAT_RENDER_PARAM_FORMAT_RGBA5650:
			default:
				sceGuDispBuffer( 480, 272, (void*)(512*2*272*0), 512 );
				if(nParam & CAT_RENDER_PARAM_BUFFER_MASK) {
					// single buffer
					sceGuDrawBuffer( nParam & CAT_RENDER_PARAM_FORMAT_MASK, (void*)(512*2*272*0), 512 );
				} else {
					// double buffer
					sceGuDrawBuffer( nParam & CAT_RENDER_PARAM_FORMAT_MASK, (void*)(512*2*272*1), 512 );
				}
				sceGuDepthBuffer( (void*)(512*2*272*2), 512 );	// シングルでもダブルでも深度バッファの位置は変えない
				break;
		}

		sceGuScissor( 0, 0, CAT_SCREEN_WIDTH, CAT_SCREEN_HEIGHT );
		sceGuEnable( GU_SCISSOR_TEST );

		sceGuDepthMask( 1 );
		sceGuDisable( GU_DEPTH_TEST );

		sceGuColor( 0xFFFFFFFF );
		sceGuEnable( GU_TEXTURE_2D );
		sceGuTexWrap( GU_CLAMP, GU_CLAMP );
		sceGuTexFilter( GU_LINEAR, GU_LINEAR );

		sceGuClearColor( 0x000000 );
		sceGuDisable( GU_ALPHA_TEST );
		sceGuTexFunc( GU_TFX_MODULATE, GU_TCC_RGBA );

		sceGuEnable( GU_BLEND );
		sceGuBlendFunc( GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0 );
	} sceGuFinish();
	sceGuSync( 0, 0 );
	sceGuDisplay( GU_TRUE );

	fSW = 0;
	nEnter = 0;
}

//! 描画関連の終了処理をする
void
Cat_RenderTerm( void )
{
	sceGuSync( 0, 0 );
	sceGuTerm();
}

//! 描画パケットの開始
static void
BeginPacket( void )
{
	if(!nEnter) {
		if(fSW == 0) {
			sceGuStart( GU_CALL, (void*)((u32)disp_list0 | 0x40000000) );
		} else {
			sceGuStart( GU_CALL, (void*)((u32)disp_list1 | 0x40000000) );
		}
	}
	nEnter++;
}

//! 描画パケットの終了
static void
EndPacket( void )
{
	if(--nEnter == 0) {
		sceGuFinish();
	}
}

//! 描画を開始する
void
Cat_RenderBegin( void )
{
	BeginPacket();

	sceGuClear( GU_COLOR_BUFFER_BIT | GU_FAST_CLEAR_BIT );
}

//! 描画を終了する
void
Cat_RenderEnd( void )
{
	EndPacket();
}

//! 画面を更新する
void
Cat_RenderScreenUpdate( void )
{
	sceGuSync( 0, 0 );
	sceDisplayWaitVblankStartCB();
	sceGuSwapBuffers();

	sceGuStart( GU_DIRECT, disp_list );
	if(fSW == 0) {
		sceGuCallList( disp_list0 );
	} else {
		sceGuCallList( disp_list1 );
	}
	sceGuFinish();
	fSW ^= 1;
}
