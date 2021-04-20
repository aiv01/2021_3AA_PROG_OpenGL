#pragma once
// #include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct{
    float x;
    float y;
    float z;
}wobj_float3;

typedef struct{
    float x;
    float y;
}wobj_float2;

typedef struct{
    wobj_float3 position;
    wobj_float2 uv;
    wobj_float3 normal;
}wobj_vertex;

typedef struct{
    wobj_vertex v1;
    wobj_vertex v2;
    wobj_vertex v3;
}wobj_triangle;


typedef struct
{
    float *vertices;
    float *uvs;
    float *normals;
    int *faces;

    int vertices_size;
    int vertices_count;
    int uvs_size;
    int uvs_count;
    int normals_size;
    int normals_count;
    int faces_size;
    int faces_count;

    wobj_triangle* triangles;
    int triangles_count;

} wobj;

static wobj* wobj_from_file(const char* file_path)
{
    wobj *w = (wobj*)malloc(sizeof(wobj));
    w->vertices_count = 0;
    w->uvs_count = 0;
    w->normals_count = 0;
    w->faces_count = 0;
    w->triangles_count = 0;
    FILE* file = fopen(file_path, "r");
    if(file == NULL){
        return NULL;
    }

    //read to populate array size (vertex, uv...)
    char buffer[1024];

    while(fgets(buffer, sizeof(buffer), file)){
        if(strncmp(buffer, "v ", 2) ==0){
            w->vertices_count += 1;
        }else if(strncmp(buffer, "vt ", 3)==0){
            w->uvs_count += 1;
        }else if(strncmp(buffer, "vn ", 3)==0){
            w->normals_count += 1;
        }
        else if(strncmp(buffer, "f ", 2)==0){
            w->faces_count += 1;
        }
    }

    rewind(file);

    w->vertices_size = w->vertices_count*3;
    w->uvs_size = w->uvs_count*2;
    w->normals_size = w->normals_count*3;
    w->faces_size = w->faces_count*9;

    w->vertices = (float*)malloc(sizeof(float) * w->vertices_size);
    w->uvs = (float*)malloc(sizeof(float)*w->uvs_size);
    w->normals = (float*)malloc(sizeof(float)*w->normals_size);
    w->faces = (int*)malloc(sizeof(int)*w->faces_size);
    int vert_index = 0;
    int uv_index= 0;
    int normal_index= 0;
    int faces_index= 0;
    while(fgets(buffer, sizeof(buffer), file)){
        char* next_token;
        char* token = strtok_s(buffer, " ", &next_token); //v 1.0000 -1.0000 0.0000\n
        if(strcmp(token, "v") == 0){
            //vertex 0
            char* vert;
            vert = strtok_s(NULL, " ", &next_token);
            float vert_val = atof(vert);
            w->vertices[vert_index + 0] = vert_val;
            // vert 1
            vert = strtok_s(NULL, " ", &next_token);
            vert_val = atof(vert);
            w->vertices[vert_index + 1] = vert_val;

            //vert 2
            vert = strtok_s(NULL, " ", &next_token);
            vert_val = atof(vert);
            w->vertices[vert_index + 2] = vert_val;

            vert_index += 3;
        }else if (strcmp(token, "vt") == 0 ){
            char* uv; 
            uv = strtok_s(NULL, " ", &next_token);
            float uv_val = atof(uv);
            w->uvs[uv_index + 0] = uv_val;

            uv = strtok_s(NULL, " ", &next_token);
            uv_val = atof(uv);
            w->uvs[uv_index + 1] = uv_val;

            uv_index +=2;
        }else if(strcmp(token, "vn") == 0){
            //vertex 0
            char* norm;
            norm = strtok_s(NULL, " ", &next_token);
            float norm_val = atof(norm);
            w->normals[normal_index + 0] = norm_val;
            // vert 1
            norm = strtok_s(NULL, " ", &next_token);
            norm_val = atof(norm);
            w->normals[normal_index + 1] = norm_val;

            //vert 2
            norm = strtok_s(NULL, " ", &next_token);
            norm_val = atof(norm);
            w->normals[normal_index + 2] = norm_val;

            normal_index += 3;
        }else if(strcmp(token, "f") ==0){
            char* face;
            for (int i = 0; i < 3; i++)
            {
                face = strtok_s(NULL, "/", &next_token);
                int face_val= atoi(face);
                w->faces[faces_index + 0]= face_val;

                face = strtok_s(NULL, "/", &next_token);
                face_val= atoi(face);
                w->faces[faces_index + 1]= face_val;

                face = strtok_s(NULL, " ", &next_token);
                face_val= atoi(face);
                w->faces[faces_index + 2]= face_val;

                faces_index +=3;
            }
        }
    }

    fclose(file);

    w->triangles_count = w->faces_count;
    w->triangles = (wobj_triangle*)malloc(sizeof(wobj_triangle)*w->triangles_count);

    for (int i = 0; i < w->triangles_count; i++)
    {
        wobj_triangle t;
        int vertex_index, uv_index, normal_index, face;

        //v1
        vertex_index = (w->faces[i*9 +0] - 1) * 3;
        t.v1.position.x = w->vertices[vertex_index + 0];
        t.v1.position.y = w->vertices[vertex_index + 1];
        t.v1.position.z = w->vertices[vertex_index + 2];

        uv_index = (w->faces[i*9 +1]-1) * 2;
        t.v1.uv.x = w->uvs[uv_index];
        t.v1.uv.y = w->uvs[uv_index + 1];

        normal_index = (w->faces[i*9 +2] -1) * 3;
        t.v1.normal.x = w->normals[normal_index + 0];
        t.v1.normal.y = w->normals[normal_index + 1];
        t.v1.normal.z = w->normals[normal_index + 2];

        //v2
        vertex_index = (w->faces[i*9 +3] - 1) * 3;
        t.v2.position.x = w->vertices[vertex_index + 0];
        t.v2.position.y = w->vertices[vertex_index + 1];
        t.v2.position.z = w->vertices[vertex_index + 2];

        uv_index = (w->faces[i*9 +4] -1) * 2;
        t.v2.uv.x = w->uvs[uv_index];
        t.v2.uv.y = w->uvs[uv_index + 1];

        normal_index = (w->faces[i*9 +5] -1) * 3;
        t.v2.normal.x = w->normals[normal_index + 0];
        t.v2.normal.y = w->normals[normal_index + 1];
        t.v2.normal.z = w->normals[normal_index + 2];

        //v3
        vertex_index = (w->faces[i*9 +6] - 1) * 3;
        t.v3.position.x = w->vertices[vertex_index + 0];
        t.v3.position.y = w->vertices[vertex_index + 1];
        t.v3.position.z = w->vertices[vertex_index + 2];

        uv_index = (w->faces[i*9 +7] -1) * 2;
        t.v3.uv.x = w->uvs[uv_index];
        t.v3.uv.y = w->uvs[uv_index + 1];

        normal_index = (w->faces[i*9 +8] -1) * 3;
        t.v3.normal.x = w->normals[normal_index + 0];
        t.v3.normal.y = w->normals[normal_index + 1];
        t.v3.normal.z = w->normals[normal_index + 2];

        w->triangles[i] = t;
    }
    
    return w;
}

static void wobj_destroy(wobj* obj){

    free(obj->faces);
    free(obj->vertices);
    free(obj->normals);
    free(obj->uvs);
    free(obj->triangles);
    free(obj);
}