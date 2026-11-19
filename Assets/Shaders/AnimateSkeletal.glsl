//to be included in a vertex shader, not used standalone

void skel_Animate(in vec4 localPos, out vec4 pos){
    if(isAnimated) {
        for (int i = 0; i < 4; ++i) {
            int     jointIndex  = jointIndices[i];
            float   jointWeight = jointWeights[i];

            pos += joints[jointIndex] * localPos * jointWeight;
        }
    }
    else{
        pos = localPos;
    }
}