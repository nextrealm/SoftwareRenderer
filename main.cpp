#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Vector2 {
public:
    Vector2() : values{0, 0, 1} {
    }

    Vector2(std::initializer_list<float> values) {
        int i{0};
        for(auto v : values) {
            this->values[i++] = v;
        }
    }

    Vector2(const Vector2& other)
    {
        SetValue(0, other.GetValue(0));
        SetValue(1, other.GetValue(1));
        SetValue(2, other.GetValue(2));
    }

    ~Vector2() {
        //delete[] values;
    }

    float GetValue(const int index) const {
        return values[index];
    }

    void SetValue(const int index, const float value) {
        values[index] = value;
    }

    Vector2& operator=(const Vector2& rhs)
    {
        // Only do the assignment if we are not assigning
        // to ourselves
        if (this != &rhs)
        {
            SetValue(0, rhs.GetValue(0));
            SetValue(1, rhs.GetValue(1));
            SetValue(2, rhs.GetValue(2));
        }
        return *this;
    }

    bool operator==(const Vector2& rhs) const
    {
        return (x == rhs.x && y == rhs.y && w == rhs.w);
    }

    const Vector2 operator+(const Vector2& rhs) const
    {
        return Vector2({x + rhs.x, y + rhs.y, w + rhs.w});
    }
private:
    //float values[3];
public:
    union {
        float values[3];
        struct {
            float x, y, w;
        };
        struct {
            float width, height;
        };
    };
};

class Vector3 {
public:
    Vector3() : values{0, 0, 0, 1} {
    }

    Vector3(std::initializer_list<float> values) {
        int i{0};
        for(auto v : values) {
            this->values[i++] = v;
        }
    }

    Vector3(const Vector2& other)
    {
        SetValue(0, other.GetValue(0));
        SetValue(1, other.GetValue(1));
        SetValue(2, other.GetValue(2));
        SetValue(3, other.GetValue(3));
    }

    ~Vector3() {
        //delete[] values;
    }

    float GetValue(const int index) const {
        return values[index];
    }

    void SetValue(const int index, const float value) {
        values[index] = value;
    }

    Vector3& operator=(const Vector3& rhs)
    {
        // Only do the assignment if we are not assigning
        // to ourselves
        if (this != &rhs)
        {
            SetValue(0, rhs.GetValue(0));
            SetValue(1, rhs.GetValue(1));
            SetValue(2, rhs.GetValue(2));
            SetValue(3, rhs.GetValue(3));
        }
        return *this;
    }

    bool operator==(const Vector3& rhs) const
    {
        return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
    }

    const Vector3 operator+(const Vector3& rhs) const
    {
        return Vector3({x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w});
    }

    void dehomogenize(){
        x /= w;
        y /= w;
        z /= w;
        w /= w;
    }
private:
    //float values[4];
public:
    union {
        float values[4];
        struct {
            float x, y, z, w;
        };
        struct {
            float width, height, depth;
        };
    };
};

class Matrix2 {
public:
    Matrix2() : values{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}} {
    }

    Matrix2(std::initializer_list<std::initializer_list<float>> values) {
        int i{0};
        for(auto row : values) {
            int j{0};
            for(auto v : row) {
                this->values[i][j++] = v;
            }
            ++i;
        }
    }

    Matrix2(const Matrix2& other) {
        for(int i = 0;i <= 2;i++) {
            for(int j = 0;j <= 2;j++) {
                SetValue(i, j, other.GetValue(i, j));
            }
        }
    }

    ~Matrix2() {
        //delete[] values;
    }

    Matrix2& operator=(const Matrix2 &rhs) {
        for(int i = 0;i <= 2;i++) {
            for(int j = 0;j <= 2;j++) {
                SetValue(i, j, rhs.GetValue(i, j));
            }
        }
        return *this;
    }

    float GetValue(const int row, const int column) const {
        return values[row][column];
    }

    void SetValue(const int row, const int column, const float value) {
        values[row][column] = value;
    }

    // Multiply two matrices together
    const Matrix2 operator*(const Matrix2 &other) const {
        return Matrix2({{x0 * other.x0 + y0 * other.x1 + w0 * other.x2, x0 * other.y0 + y0 * other.y1 + w0 * other.y2, x0 * other.w0 + y0 * other.w1 + w0 * other.w2},
        {x1 * other.x0 + y1 * other.x1 + w1 * other.x2, x1 * other.y0 + y1 * other.y1 + w1 * other.y2, x1 * other.w0 + y1 * other.w1 + w1 * other.w2},
        {x2 * other.x0 + y2 * other.x1 + w2 * other.x2, x2 * other.y0 + y2 * other.y1 + w2 * other.y2, x2 * other.w0 + y2 * other.w1 + w2 * other.w2}});
    }

    // Multiply a matrix by a vertex, returning a vertex
    const Vector2 operator*(const Vector2 &other) const {
        /*std::cout << "matrix = " << x0 << "," << y0 << "," << w0 << std::endl;
        std::cout << "matrix = " << x1 << "," << y1 << "," << w1 << std::endl;
        std::cout << "matrix = " << x2 << "," << y2 << "," << w2 << std::endl;
        std::cout << "other = " << other.x << "," << other.y << "," << other.w << std::endl;*/
        return Vector2({other.x * x0 + other.y * y0 + other.w * w0,
        other.x * x1 + other.y * y1 + other.w * w1,
        other.x * x2 + other.y * y2 + other.w * w2});
    }
private:
    //float values[3][3];
public:
    union {
        float values[3][3];
        struct {
            float x0, y0, w0;
            float x1, y1, w1;
            float x2, y2, w2;
        };
    };
};

class Matrix3 {
public:
    Matrix3() : values{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}} {
    }

    Matrix3(std::initializer_list<std::initializer_list<float>> values) {
        int i{0};
        for(auto row : values) {
            int j{0};
            for(auto v : row) {
                this->values[i][j++] = v;
            }
            ++i;
        }
    }

    Matrix3(const Matrix3& other) {
        for(int i = 0;i <= 3;i++) {
            for(int j = 0;j <= 3;j++) {
                SetValue(i, j, other.GetValue(i, j));
            }
        }
    }

    ~Matrix3() {
        //delete[] values;
    }

    Matrix3& operator=(const Matrix3 &rhs) {
        for(int i = 0;i <= 3;i++) {
            for(int j = 0;j <= 3;j++) {
                SetValue(i, j, rhs.GetValue(i, j));
            }
        }
        return *this;
    }

    float GetValue(const int row, const int column) const {
        return values[row][column];
    }

    void SetValue(const int row, const int column, const float value) {
        values[row][column] = value;
    }

    // Multiply two matrices together
    const Matrix3 operator*(const Matrix3 &other) const {
        return Matrix3({
            {
                x0 * other.x0 + y0 * other.x1 + z0 * other.x2 + w0 * other.x3,
                x0 * other.y0 + y0 * other.y1 + z0 * other.y2 + w0 * other.y3,
                x0 * other.z0 + y0 * other.z1 + z0 * other.z2 + w0 * other.z3,
                x0 * other.w0 + y0 * other.w1 + z0 * other.w2 + w0 * other.w3
            },
            {
                x1 * other.x0 + y1 * other.x1 + z1 * other.x2 + w1 * other.x3,
                x1 * other.y0 + y1 * other.y1 + z1 * other.y2 + w1 * other.y3,
                x1 * other.z0 + y1 * other.z1 + z1 * other.z2 + w1 * other.z3,
                x1 * other.w0 + y1 * other.w1 + z1 * other.w2 + w1 * other.w3
            },
            {
                x2 * other.x0 + y2 * other.x1 + z2 * other.x2 + w2 * other.x3,
                x2 * other.y0 + y2 * other.y1 + z2 * other.y2 + w2 * other.y3,
                x2 * other.z0 + y2 * other.z1 + z2 * other.z2 + w2 * other.z3,
                x2 * other.w0 + y2 * other.w1 + z2 * other.w2 + w2 * other.w3
            },
            {
                x3 * other.x0 + y3 * other.x1 + z3 * other.x2 + w3 * other.x3,
                x3 * other.y0 + y3 * other.y1 + z3 * other.y2 + w3 * other.y3,
                x3 * other.z0 + y3 * other.z1 + z3 * other.z2 + w3 * other.z3,
                x3 * other.w0 + y3 * other.w1 + z3 * other.w2 + w3 * other.w3
            }});
    }

    // Multiply a matrix by a vertex, returning a vertex
    const Vector3 operator*(const Vector3 &other) const {
        /*std::cout << "matrix = " << x0 << "," << y0 << "," << z0 << "," << w0 << std::endl;
        std::cout << "matrix = " << x1 << "," << y1 << "," << z1 << "," << w1 << std::endl;
        std::cout << "matrix = " << x2 << "," << y2 << "," << z2 << "," << w2 << std::endl;
        std::cout << "matrix = " << x3 << "," << y3 << "," << z3 << "," << w3 << std::endl;
        std::cout << "other = " << other.x << "," << other.y << "," << other.z << "," << other.w << std::endl;*/
        return Vector2({other.x * x0 + other.y * y0 + other.z * z0 + other.w * w0,
        other.x * x1 + other.y * y1 + other.z * z1 + other.w * w1,
        other.x * x2 + other.y * y2 + other.z * z2 + other.w * w2,
        other.x * x3 + other.y * y3 + other.z * z3 + other.w * w3});
    }
private:
    //float values[4][4];
public:
    union {
        float values[4][4];
        struct {
            float x0, y0, z0, w0;
            float x1, y1, z1, w1;
            float x2, y2, z2, w2;
            float x3, y3, z3, w3;
        };
    };
};

class Camera {
public:
    Camera(const Vector3& position, const Vector3& rotation){
        this->position = position;
        this->rotation = rotation;
    }
    Vector3 position;
    Vector3 rotation;
};

class Vertex {
public:
    Vertex(){
        
    }
    Vertex(Vector3 position, Vector3 normal, Vector2 tex_coords){
        this->position = position;
        this->normal = normal;
        this->tex_coords = tex_coords;
    }
    Vector3 position;
    Vector3 normal;
    Vector2 tex_coords;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
        this->vertices = vertices;
        this->indices = indices;
    }
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

Vector2 size{800, 600};
sf::Image image;
std::vector<Mesh> meshes;

void setPixel(const int x, const int y, const sf::Color color) {
    if(x < 0 || y < 0 || x > size.width || y > size.height) return;
    image.setPixel(x, y, color);
}

void clear() {
    for(int x = 0; x < size.width; x++)
    {
        for(int y = 0; y < size.height; y++)
        {
            setPixel(x, y, sf::Color(255, 255, 255, 255));
        }
    }
}

void drawLine(const Vector2 start, const Vector2 end) {
    float x1 = start.x;
    float x2 = end.x;
    float y1 = start.y;
    float y2 = end.y;
    sf::Color color(0, 0, 0, 255);
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if (steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if (x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;

    for (int x = (int)x1; x <= maxX; x++)
    {
        if (steep)
        {
            setPixel(y, x, color);
        }
        else
        {
            setPixel(x, y, color);
        }

        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}

// TODO : Optimize
/*vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, const std::string directory){
    vector<Texture> textures;
    for(unsigned int i = 0;i < material->GetTextureCount(type);i++){
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.id = textureFromFile(str.C_Str(), directory);
        texture.type = type;
        texture.path = str;
        textures.push_back(texture);
    }
    return textures;
}*/

Mesh processMesh(aiMesh *mesh, const aiScene *scene, const std::string& directory) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    //vector<Texture> textures;
    for(unsigned int i = 0;i < mesh->mNumVertices;i++){
        Vertex vertex;

        Vector3 pos;
        pos.x = mesh->mVertices[i].x;
        pos.y = mesh->mVertices[i].y;
        pos.z = mesh->mVertices[i].z;
        vertex.position = pos;

        Vector3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;
        vertex.normal = normal;

        if(mesh->mTextureCoords[0]){
            Vector2 texCoords;
            texCoords.x = mesh->mTextureCoords[0][i].x;
            texCoords.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = texCoords;
        }else{
            Vector2 texCoords{0.0f, 0.0f};
            vertex.tex_coords = texCoords;
        }

        vertices.push_back(vertex);
    }
    for(unsigned int i = 0;i < mesh->mNumFaces;i++){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0;j < face.mNumIndices;j++){
            indices.push_back(face.mIndices[j]);
        }
    }
    /*if(mesh->mMaterialIndex >= 0){
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE, directory);
        textures.insert(textures.end(), diffuse.begin(), diffuse.end());
        vector<Texture> specular = loadMaterialTextures(material, aiTextureType_SPECULAR, directory);
        textures.insert(textures.end(), specular.begin(), specular.end());
    }*/
    return Mesh(vertices, indices);//, textures);
}

void processNode(aiNode *node, const aiScene *scene, const std::string & directory) {
    for(unsigned int i = 0;i < node->mNumMeshes;i++){
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, directory));
    }
    for(unsigned int i = 0;i < node->mNumChildren;i++){
        processNode(node->mChildren[i], scene, directory);
    }
}

void loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR: assimp: " << importer.GetErrorString() << std::endl;
        return;
    }
    std::string directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene, directory);
}

int main(int argc, char *argv[])
{
    sf::RenderWindow window(sf::VideoMode(size.width, size.height, 32), "Software Renderer");
    
    image.create(size.width, size.height, sf::Color());
    
    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite sprite;
    sprite.setTexture(texture);

    Camera camera(Vector3{0.0f, 0.0f, -50.0f}, Vector3{0.0f, 0.0f, 0.0f});

    loadModel("cube.md2");

    while(window.isOpen()){
        sf::Event event;
        
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float d = 1.0f;
        float a = size.width / size.height;

        Matrix3 view = {
            {1, 0, 0, -camera.position.x},
            {0, 1, 0, -camera.position.y},
            {0, 0, 1, -camera.position.z},
            {0, 0, 0, 1}};

        Matrix3 perspectiveProjection = {
            {d/a, 0, 0, 0},
            {0, d, 0, 0},
            {0, 0, d, 0},
            {0, 0, 1, 0}};

        Matrix3 screenProjection = {
            {size.width/2, 0, 0, size.width/2},
            {0, -size.height/2, 0, size.height/2},
            {0, 0, d/2, d/2},
            {0, 0, 0, 1}};

        clear();
        
        static float degrees = 0.0f;
        degrees += 0.1f;
        float radians = degrees * M_PI / 180.0f;
        Matrix3 zr{
            {cosf(radians), -sinf(radians), 0, 0},
            {sinf(radians), cosf(radians), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };
        Matrix3 xr{
            {1, 0, 0, 0},
            {0, cosf(radians), -sinf(radians), 0},
            {0, sinf(radians), cosf(radians), 0},
            {0, 0, 0, 1}
        };
        Matrix3 yr{
            {cosf(radians), 0, sinf(radians), 0, 0},
            {0, 1, 0, 0},
            {-sinf(radians), 0, cosf(radians), 0},
            {0, 0, 0, 1}
        };
        Matrix3 s{
            {10, 0, 0, 0},
            {0, 2, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };
        Matrix3 t{
            {1, 0, 0, 200},
            {0, 1, 0, 50},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };

        /*for(Vertex vertex : meshes[0].vertices){
            std::cout << "vertex = " << vertex.position.x << "," << vertex.position.y << "," << vertex.position.z << std::endl;
        }*/

        std::vector<Vector3> vertices;
        vertices.reserve(meshes[0].vertices.size());
        for(Vertex vertex : meshes[0].vertices){
            Vector3 v = yr * vertex.position;
            vertices.push_back(v);
        }

        /*for(Vector3 vertex : vertices){
            std::cout << "rotated vertex = " << vertex.x << "," << vertex.y << "," << vertex.z << std::endl;
        }*/

        std::vector<Vector3> projectedVertices;
        projectedVertices.reserve(vertices.size());
        for(Vector3 v : vertices){
            v = view * v;
            v = perspectiveProjection * v;
            v.dehomogenize();

            v = screenProjection * v;

            projectedVertices.push_back(v);
        }

        vertices = projectedVertices;

        /*for(Vector3 vertex : vertices){
            std::cout << "projected vertex = " << vertex.x << "," << vertex.y << "," << vertex.z << std::endl;
        }*/

        for(unsigned int i = 0;i < meshes[0].indices.size();i+=3){
            unsigned int index1 = meshes[0].indices[i];
            unsigned int index2 = meshes[0].indices[i+1];
            unsigned int index3 = meshes[0].indices[i+2];
            drawLine(Vector2({vertices[index1].x, vertices[index1].y}), Vector2({vertices[index2].x, vertices[index2].y}));
            drawLine(Vector2({vertices[index2].x, vertices[index2].y}), Vector2({vertices[index3].x, vertices[index3].y}));
            drawLine(Vector2({vertices[index3].x, vertices[index3].y}), Vector2({vertices[index1].x, vertices[index1].y}));
            
        }
        
        texture.loadFromImage(image);
        window.draw(sprite);
        window.display();
    }
    return 0;
}