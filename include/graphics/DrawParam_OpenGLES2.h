/**
 * DrawParam_OpenGLES2.h
 *
 *  Copyright(c) Live2D Inc. All rights reserved.
 *  [[ CONFIDENTIAL ]]
 */
#ifndef __LIVE2D_DRAWPARAM_OPENGLES2_H__
#define __LIVE2D_DRAWPARAM_OPENGLES2_H__


#ifndef __SKIP_DOC__

#include "../Live2D.h"

#ifdef L2D_GRAPHICS_OPENGL_ES2

#include "DrawParam.h"
#include "../type/LDVector.h"


#ifdef L2D_TARGET_ANDROID_ES2
#include <jni.h>
#include <errno.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#ifdef L2D_TARGET_IPHONE_ES2
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

#ifdef L2D_TARGET_WIN_GL
#include <windows.h>
#include <GL/gl.h>
#endif

//------------ LIVE2D NAMESPACE ------------
namespace live2d
{
	
	class DrawParam_OpenGLES2 : public DrawParam
	{
	public:
		DrawParam_OpenGLES2();
		virtual ~DrawParam_OpenGLES2();

	public:
		virtual void drawTexture( int textureNo  , int indexCount , int vertexCount , l2d_index * indexArray
								 , l2d_pointf * vertexArray , l2d_uvmapf * uvArray , float opacity, int colorCompositionType) ;

		void setTexture( int modelTextureNo , GLuint textureNo ) ;

		//  新しく利用できるModelのテクスチャ番号を確保(Avatar用）
		virtual int generateModelTextureNo() ;

		//  Modelのテクスチャ番号を生成(Avatar用）
		virtual void releaseModelTextureNo(int no) ;

		virtual void setupDraw();



	private:
		// Prevention of copy Constructor
		DrawParam_OpenGLES2( const DrawParam_OpenGLES2 & ) ;
		DrawParam_OpenGLES2& operator=( const DrawParam_OpenGLES2 & ) ;

	private:
		static const int DUMMY_GL_TEXTURE_NO = 9999 ;	//  generateした番号に立てるフラグ

		// Shader
		static void initShader() ;
		static void disposeShader() ;
		static bool compileShader(GLuint *shader , GLenum type , const char *shaderSource ) ;
		static bool linkProgram(GLuint prog) ;
		static bool validateProgram(GLuint prog) ;
		static GLuint loadShaders(const char* vertShaderSrc, const char * fragShaderSrc ) ;

	private:
        live2d::LDVector<GLuint>		textures ;				//  Destructorでテクスチャの解放は行わない。基本的に外部で行う。

        static GLuint 				shaderProgram ;
        static bool 				first_initGLFunc ;


#ifdef L2D_TARGET_ANDROID_ES2
	public:
        static void setExtShaderMode( bool extMdoe , bool extPAMode = false ) ;
		static void reloadShader();

        static GLuint fboTextureNo;
		static void setFramebufferTexture( GLuint fbo );
		static GLuint getFramebufferTexture();
	private:
        static bool			extMode ;					// 拡張方式で描画
		static bool			extPAMode ;					// 拡張方式で描画
#endif

#ifdef L2D_TARGET_IPHONE_ES2

#endif

#ifdef L2D_TARGET_WIN_GL
	public:
        static void initGLFunc() ;
        static bool initGLFuncSuccess ;
	private:
        static void* my_wglGetProcAddress( const char* name ) ;//for Windows
		int ___checkError___(const char* str) ;
#endif

	};

}
//------------ LIVE2D NAMESPACE ------------

#endif		// L2D_GRAPHICS_OPENGL_ES2
#endif		// __SKIP_DOC__
#endif		// __LIVE2D_DRAWPARAM_OPENGLES2_H__
