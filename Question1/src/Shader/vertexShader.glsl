#version 420

layout (location = 0) in vec3 vPosition ;
layout (location = 1) in vec3 vNormal ;		
layout (location = 2) in vec4 vColor ;
layout (location = 3) in vec2 vTextureUV;

// Going "out" to the fragment shader
out vec4 fColor;		
out vec4 fNormal;
out vec4 fVertWorldLocation;	// using only matModel
out vec2 fTextureUV;

//uniform
uniform mat4 mModel;
uniform mat4 mModelInverseTranspose;		// mModel with Only Rotation;
uniform mat4 mView;
uniform mat4 mProjection;

void main()
{
    vec3 vertPosition = vPosition.xyz;
    //vec3 vertNormal = vNormal.xyz;
    mat4 mMVP = mProjection * mView * mModel;

    gl_Position = mMVP * vec4(vertPosition, 1.0); 

    fVertWorldLocation.xyz = (mModel * vec4(vertPosition, 1.0f)).xyz;
	fVertWorldLocation.w = 1.0f;

    fNormal.xyz = normalize(mModelInverseTranspose * vec4(vNormal, 1.0f)).xyz;
	fNormal.w = 1.0f;
    //fNormal = vec4(vNormal.xyz, 1.0);

    fColor = vec4(vColor.rgb/0xff,1.0f);
    fTextureUV = vTextureUV;

}