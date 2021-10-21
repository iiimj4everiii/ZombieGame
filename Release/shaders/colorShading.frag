#version 130
//The fragment shader operates on each pixel in a given polygon

//Input vertex position from vertexPosition
in vec2 fragmentPosition; 

//Input fragment color passed down from vertexColor
in vec4 fragmentColor;

//Input fragment UV passed down from vertexUV
in vec2 fragmentUV;

//Declaring 4 component vector (R, G, B, alpha) that gets outputted to the screen for each pixel
out vec4 color;

//Opengl deletes variables if they're not being used
//If we try to access these variables that were not being used and were deleted automatically by Opengl,
//we will not be able to locate these variables
//uniform float time;
uniform sampler2D mySampler; //sampler2D is a texture data type in GLSL

void main()
{
	//color = fragmentColor + vec4(1.0 * (cos(time) + 1.0) / 2.0, 1.0 * (cos(time + 2.0) + 1.0) / 2.0, 1.0 * (sin(time + 1.0) + 1.0) / 2.0, 0.0);
	
	//texture is GLSL function. Returns RGB + alpha
	//color is an user defined variable
	vec4 textureColor = texture(mySampler, fragmentUV);
	
	//color = textureColor *
	//		vec4(fragmentColor.r * (cos(time + 1.0 * fragmentPosition.x) + 1.0) / 2.0,
	//			 fragmentColor.g * (cos(time + 1.5 * fragmentPosition.y) + 1.0) / 2.0,
	//			 fragmentColor.b * (cos(time + 1.0 * fragmentPosition.x) + 1.0) / 2.0,
	//			 fragmentColor.a);
	
	color = textureColor * fragmentColor;
}