#version 130
//The vertex shader operates on each vertex

//Input vertex position data from the VBO. Each vertex is 2 floats (2D)
in vec2 vertexPosition;

//Input vertex color data from the VBO and then pass it to the fragment shader
in vec4 vertexColor;

//Input vertex UV data from the VBO and then pass it to the fragment shader
in vec2 vertexUV;

out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 orthoProjection;

void main()
{	
	//Set the x, y position on the screen
	//gl_Position is predefined GLSL identifier
	//gl_Position.xy = vertexPosition.xy;
	gl_Position.xy = (orthoProjection * vec4(vertexPosition, 0.0, 1.0)).xy;
	
	//Set the z position on the screen
	gl_Position.z = 0.0;
	
	//Indicate that the coordinates are normalized
	gl_Position.w = 1.0;
	
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
	fragmentUV.x = vertexUV.x;
	fragmentUV.y = 1.0 - vertexUV.y; //This is a way to flip the texture on its' x axis
}