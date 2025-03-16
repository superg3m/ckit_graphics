#include <ckit.h>

typedef struct OBJ_Model {
    CKIT_Vector3* verts;
    int** faces;
} OBJ_Model;

// f v/vt/vn

OBJ_Model obj_model_create(const char *filename) {
    OBJ_Model ret;
    ret.verts = NULL;
    ret.faces = NULL;

    size_t file_size = 0;
    u8* file_data = ckit_os_read_entire_file(filename, &file_size);

    String* lines = ckit_str_split(file_data, "\n");

    for (u32 i = 0; i < ckit_vector_count(lines); i++) {
        String line = lines[i];
        String* line_values = ckit_str_split(line, " ");
    
        if (line_values[0][0] == 'v') {
            float v0 = atof(line_values[0]);
            float v1 = atof(line_values[1]);
            float v2 = atof(line_values[2]);

            CKIT_Vector3 vector_point = {v0, v1, v2};
            ckit_vector_push(ret.verts, vector_point);
        } else if (line_values[0][0] == 'f') {
            int* face = NULLPTR;

            for (int i = 0; i < 3; i++) {
                String* indices = ckit_str_split(line_values[i + 1], "/");
                // LOG_DEBUG("i: %d | INDEX0: %s\n", i, indices[0]);
                // LOG_DEBUG("i: %d | INDEX1: %s\n", i, indices[1]);
                // LOG_DEBUG("i: %d | INDEX2: %s\n", i, indices[2]);

                ckit_vector_push(face, atoi(indices[0]) - 1);
                ckit_vector_push(face, atoi(indices[1]) - 1);
                ckit_vector_push(face, atoi(indices[2]) - 1);

                ckit_vector_free(indices);
            }
    
            ckit_vector_push(ret.faces, face);
        }

        ckit_vector_free(line_values);
    }

    ckit_vector_free(lines);
    ckit_free(file_data);

    return ret;
}

void obj_model_free(OBJ_Model* model) {
    for (u32 i = 0; i < ckit_vector_count(model->faces); i++) {
        int* face = model->faces[i];
        ckit_vector_free(face);
    }

    ckit_vector_free(model->faces);
    ckit_vector_free(model->verts);
}