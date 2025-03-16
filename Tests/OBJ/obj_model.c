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
        LOG_PRINT("%s\n", lines[i]);
    }

    /*
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            faces_.push_back(f);
        }
    }

    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
    */

    return ret;
}