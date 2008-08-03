// Cat_LoadImage test code
// infcat-banner.pngを読み込んで表示する
// 
// 実行するときは、実行ファイルと同じフォルダにinfcat-banner.pngをおくこと
//

#include "Cat_PspCallback.h"
#include "Cat_ImageLoader.h"
#include "Cat_StreamFile.h"
#include "Cat_Render.h"
#include <stdlib.h>
#include <string.h>

#include <pspdebug.h>
#include <pspkernel.h>
#include <pspgu.h>

#define TRACE(x) pspDebugScreenPrintf x
#define HALT() sceKernelSleepThreadCB()

// 描画
void
RenderSprite( float x, float y, float z, float w, float h )
{
	struct vertex_format {
		short   u,v;
		short	x,y,z;
	} __attribute__((packed));
	struct vertex_format* vert;
	vert = (struct vertex_format*)sceGuGetMemory( 32 );
	vert[0].u     = 0;
	vert[0].v     = 0;
	vert[0].x     = (short)x;
	vert[0].y     = (short)y;
	vert[0].z     = (short)z;
	vert[1].u     = (short)w;
	vert[1].v     = (short)h;
	vert[1].x     = (short)(x + w);
	vert[1].y     = (short)(y + h);
	vert[1].z     = (short)z;
	sceGuDrawArray( GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vert );
}

int
main()
{
	Cat_Stream* pStream;
	Cat_Texture* pTexture;

	Cat_SetupCallbacks();
	pspDebugScreenInit();

	TRACE(( "Cat_LoadImage test code\n" ));

	pStream = Cat_StreamFileReadOpen( "infcat-banner.png" );
	if(pStream == 0) {
		TRACE(( "Error:Cat_StreamFileReadOpen" ));
		HALT();
	}

	pTexture = Cat_LoadImage( pStream );
	Cat_StreamClose( pStream );

	if(pTexture == 0) {
		TRACE(( "Error:Cat_LoadImage" ));
		HALT();
	}

	// 読み込んだテクスチャを描画してみる
	const float x = 0.0f;
	const float y = 0.0f;
	const float z = 0.0f;
	const float w = (float)Cat_TextureGetWidth( pTexture );
	const float h = (float)Cat_TextureGetHeight( pTexture );
	Cat_RenderInit( CAT_RENDER_DEFAULT );		// 初期化処理
	for(;;) {
		Cat_RenderBegin(); {					// 描画開始
			Cat_TextureSetTexture( pTexture );	// テクスチャを設定して描画
			RenderSprite( x, y, z, w, h );		// 
		} Cat_RenderEnd();						// 描画終了
		Cat_RenderScreenUpdate();				// 画面更新
	}
	Cat_TextureRelease( pTexture );				// テクスチャ解放
	Cat_RenderTerm();							// 終了処理

	HALT();
	return 0;
}
