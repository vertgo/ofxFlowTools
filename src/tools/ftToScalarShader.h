
#pragma once

#include "ofMain.h"
#include "ftShader.h"


namespace flowTools {
	
	class ftToScalarShader : public ftShader {
	public:
		ftToScalarShader() {
			
			ofLogVerbose("init ftToScalarShader");
			if (isProgrammableRenderer)
				glThree();
			else
				glTwo();
		}
		
	protected:
		void glTwo() {
			fragmentShader = GLSL120(
								  uniform sampler2DRect ScalarTexture;
								  uniform float Scale;
								  void main(){
									  vec4	velocity = texture2DRect(normalTexture, gl_TexCoord[0].st);
									  velocity.xyz -= vec3(Scale * 0.5);
									  velocity.xyz *= vec3(Scale);
									  velocity.w = 0.0;
									  gl_FragColor = velocity;
								  }
								  );
			
			shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			shader.linkProgram();
		}
		
		void glThree() {
			fragmentShader = GLSL150(
								  uniform sampler2DRect ScalarTexture;
								  uniform float Scale;
								  
								  in vec2 texCoordVarying;
								  out vec4 fragColor;
								  
								  void main(){
									  vec4	velocity = texture(normalTexture, texCoordVarying);
									  velocity.xyz -= vec3(Scale * 0.5);
									  velocity.xyz *= vec3(Scale);
									  velocity.w = 0.0;
									  fragColor = velocity;
								  }
								  );
			
			
			shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
			shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			shader.bindDefaults();
			shader.linkProgram();
		}
		
	public:
		
		void update(ofFbo& _buffer, ofTexture& _scalarTexture, float _scale){
			_buffer.begin();
			shader.begin();
			shader.setUniformTexture("ScalarTexture", _scalarTexture, 0);
			shader.setUniform1f("Scale", _scale);
			renderFrame(_buffer.getWidth(), _buffer.getHeight());
			shader.end();
			_buffer.end();
		}
	};
}