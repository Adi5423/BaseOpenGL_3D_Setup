#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

/* ================= MATH ================= */
struct Vec3 { float x,y,z; };

Vec3 operator+(Vec3 a,Vec3 b){ return {a.x+b.x,a.y+b.y,a.z+b.z}; }
Vec3 operator-(Vec3 a,Vec3 b){ return {a.x-b.x,a.y-b.y,a.z-b.z}; }
Vec3 operator*(Vec3 a,float s){ return {a.x*s,a.y*s,a.z*s}; }

Vec3 normalize(Vec3 v){
    float l=sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
    return {v.x/l,v.y/l,v.z/l};
}

Vec3 cross(Vec3 a,Vec3 b){
    return { a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x };
}

/* ================= CAMERA ================= */
Vec3 camPos={0,0,5}, camFront={0,0,-1}, camUp={0,1,0};
float yaw=-90,pitch=0;
bool rmb=false,firstMouse=true;
float lastX=640,lastY=360;

void mouse(GLFWwindow*,double x,double y){
    if(!rmb){ firstMouse=true; return; }
    if(firstMouse){ lastX=x; lastY=y; firstMouse=false; }

    float dx=x-lastX, dy=lastY-y;
    lastX=x; lastY=y;

    dx*=0.1f; dy*=0.1f;
    yaw+=dx; pitch+=dy;
    if(pitch>89) pitch=89;
    if(pitch<-89) pitch=-89;

    camFront=normalize({
        cosf(yaw*0.0174533f)*cosf(pitch*0.0174533f),
        sinf(pitch*0.0174533f),
        sinf(yaw*0.0174533f)*cosf(pitch*0.0174533f)
    });
}

/* ================= MATRICES ================= */
void identity(float*m){ for(int i=0;i<16;i++) m[i]=(i%5==0); }

void perspective(float*m,float f,float a,float n,float fr){
    float t=tanf(f/2);
    for(int i=0;i<16;i++) m[i]=0;
    m[0]=1/(a*t); m[5]=1/t;
    m[10]=-(fr+n)/(fr-n); m[11]=-1;
    m[14]=-(2*fr*n)/(fr-n);
}

void lookAt(float*m){
    Vec3 f=normalize(camFront);
    Vec3 r=normalize(cross(f,camUp));
    Vec3 u=cross(r,f);

    m[0]=r.x; m[4]=r.y; m[8]=r.z;  m[12]=-(r.x*camPos.x+r.y*camPos.y+r.z*camPos.z);
    m[1]=u.x; m[5]=u.y; m[9]=u.z;  m[13]=-(u.x*camPos.x+u.y*camPos.y+u.z*camPos.z);
    m[2]=-f.x;m[6]=-f.y;m[10]=-f.z;m[14]=(f.x*camPos.x+f.y*camPos.y+f.z*camPos.z);
    m[15]=1;
}

void rotateY(float*m,float a){
    identity(m);
    m[0]= cosf(a); m[8]=sinf(a);
    m[2]=-sinf(a); m[10]=cosf(a);
}

void mul(float*o,float*a,float*b){
    for(int c=0;c<4;c++)
        for(int r=0;r<4;r++){
            o[c*4+r]=0;
            for(int k=0;k<4;k++)
                o[c*4+r]+=a[k*4+r]*b[c*4+k];
        }
}

/* ================= SHADERS ================= */
const char* vs=R"(
#version 460 core
layout(location=0) in vec3 p;
uniform mat4 MVP;
void main(){ gl_Position=MVP*vec4(p,1); }
)";
const char* fs=R"(
#version 460 core
out vec4 c;
void main(){ c=vec4(1,0.6,0.2,1); }
)";

GLuint program(){
    auto sh=[&](GLenum t,const char*s){
        GLuint x=glCreateShader(t);
        glShaderSource(x,1,&s,0);
        glCompileShader(x);
        return x;
    };
    GLuint p=glCreateProgram();
    GLuint v=sh(GL_VERTEX_SHADER,vs), f=sh(GL_FRAGMENT_SHADER,fs);
    glAttachShader(p,v); glAttachShader(p,f);
    glLinkProgram(p);
    glDeleteShader(v); glDeleteShader(f);
    return p;
}

/* ================= MAIN ================= */
int main(){
    glfwInit();
    GLFWwindow*w=glfwCreateWindow(1280,720,"REAL 3D Triangle (Solid)",0,0);
    glfwMakeContextCurrent(w);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetCursorPosCallback(w,mouse);
    glEnable(GL_DEPTH_TEST);

    // 🔺 TRIANGULAR PRISM (REAL VOLUME)
    float tri3D[] = {
        // Front face
        -0.7f,-0.5f, 0.3f,
         0.7f,-0.5f, 0.3f,
         0.0f, 0.8f, 0.3f,

        // Back face
        -0.7f,-0.5f,-0.3f,
         0.0f, 0.8f,-0.3f,
         0.7f,-0.5f,-0.3f,

        // Side 1
        -0.7f,-0.5f, 0.3f,
        -0.7f,-0.5f,-0.3f,
         0.0f, 0.8f,-0.3f,

        -0.7f,-0.5f, 0.3f,
         0.0f, 0.8f,-0.3f,
         0.0f, 0.8f, 0.3f,

        // Side 2
         0.7f,-0.5f, 0.3f,
         0.0f, 0.8f, 0.3f,
         0.0f, 0.8f,-0.3f,

         0.7f,-0.5f, 0.3f,
         0.0f, 0.8f,-0.3f,
         0.7f,-0.5f,-0.3f,

        // Bottom
        -0.7f,-0.5f, 0.3f,
         0.7f,-0.5f,-0.3f,
        -0.7f,-0.5f,-0.3f,

        -0.7f,-0.5f, 0.3f,
         0.7f,-0.5f, 0.3f,
         0.7f,-0.5f,-0.3f
    };

    GLuint VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(tri3D),tri3D,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,0,0,0);
    glEnableVertexAttribArray(0);

    GLuint prog=program();
    GLint mvpLoc=glGetUniformLocation(prog,"MVP");

    float P[16],V[16],M[16],PV[16],MVP[16];
    double last=glfwGetTime();
    float angle=0;

    while(!glfwWindowShouldClose(w)){
        double now=glfwGetTime();
        float dt=now-last; last=now;
        angle+=dt;

        rmb=glfwGetMouseButton(w,GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS;
        glfwSetInputMode(w,GLFW_CURSOR,rmb?GLFW_CURSOR_DISABLED:GLFW_CURSOR_NORMAL);

        float s=dt*3*(glfwGetKey(w,GLFW_KEY_LEFT_SHIFT)?3:1);
        Vec3 right=normalize(cross(camFront,camUp));
        if(rmb){
            if(glfwGetKey(w,GLFW_KEY_W)) camPos=camPos+camFront*s;
            if(glfwGetKey(w,GLFW_KEY_S)) camPos=camPos-camFront*s;
            if(glfwGetKey(w,GLFW_KEY_A)) camPos=camPos-right*s;
            if(glfwGetKey(w,GLFW_KEY_D)) camPos=camPos+right*s;
            if(glfwGetKey(w,GLFW_KEY_Q)) camPos.y-=s;
            if(glfwGetKey(w,GLFW_KEY_E)) camPos.y+=s;
        }

        perspective(P,45*0.0174533f,1280.f/720.f,0.1f,100);
        lookAt(V);
        rotateY(M,angle);

        mul(PV,P,V);
        mul(MVP,PV,M);

        glClearColor(0.05f,0.06f,0.08f,1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glUseProgram(prog);
        glUniformMatrix4fv(mvpLoc,1,GL_FALSE,MVP);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,36);

        glfwSwapBuffers(w);
        glfwPollEvents();
    }
}
