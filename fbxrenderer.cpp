#include "fbxrenderer.h"

FbxRenderer::FbxRenderer(){

}

void FbxRenderer::init(QOpenGLContext *share, const std::string &filename)
{

    QOpenGLFunctions_3_3_Core* f = 0;
    f = share->versionFunctions<QOpenGLFunctions_3_3_Core>();

    m_fbxLoader.Initialize("./resources/model/hand_rig2.fbx");
    qDebug() << "initialize ?" << m_fbxLoader.GetMaterialCount() ;
    m_fbxLoader.LoadAnimation("./resources/model/hand_rig2.fbx");
    qDebug() << "load animation ? ";

    hand_program = new QOpenGLShaderProgram();

    hand_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QString("./resources/Shaders/SkinCloth.vert"));
    hand_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QString("./resources/Shaders/Skin.frag"));
    hand_program->link();
    qDebug() << "link";


    f->glUseProgram(hand_program->programId());
    qDebug() << "gluseprogram";


    uniformsSkin[UNIFORM_VS] = f->glGetUniformBlockIndex(hand_program->programId(), "UniformVs");
    uniformsSkin[UNIFORM_DIFFUSE_TEXTURE] = f->glGetUniformBlockIndex(hand_program->programId(), "diffuseTexture");
    uniformsSkin[UNIFORM_FALLOFF_TEXTURE] = f->glGetUniformBlockIndex(hand_program->programId(), "falloffTexture");
    uniformsSkin[UNIFORM_RIMLIGHT_TEXTURE] = f->glGetUniformBlockIndex(hand_program->programId(), "rimlightTexture");
    uniformsSkin[UNIFORM_SPECULAR_TEXTURE] = f->glGetUniformBlockIndex(hand_program->programId(), "specularTexture");
    qDebug() << "glgetuniformblock";


    f->glGenBuffers(1, &m_uniformBufferVs);


    /*
    for(int i = 0; i < m_fbxLoader.GetMaterialCount(); ++i)
    {
        auto& modelMaterial = m_fbxLoader.GetMaterial(i);

        AppMaterial material;
        material.modelMaterial = &modelMaterial;

        std::string home = "/Textures/";
        // read texture
        material.diffuseTexture = loadTexture(home + modelMaterial.diffuseTextureName);
        material.falloffTexture = loadTexture(home + modelMaterial.falloffTextureName);
        material.specularTexture = loadTexture(home + modelMaterial.specularTextureName);
        material.rimlightTexture = 0;

            // repatsampler
            glGenSamplers(1, &material.repeatSampler);
            glSamplerParameteri(material.repeatSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glSamplerParameteri(material.repeatSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glSamplerParameteri(material.repeatSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glSamplerParameteri(material.repeatSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // clump sampler
            glGenSamplers(1, &material.clampSampler);
            glSamplerParameteri(material.clampSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glSamplerParameteri(material.clampSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glSamplerParameteri(material.clampSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glSamplerParameteri(material.clampSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            m_materialList.push_back(material);

    }
*/
    for (int i = 0; i < m_fbxLoader.GetMeshCount(); ++i)
    {
        auto& modelMesh = m_fbxLoader.GetMesh(i);
        auto& modelVertexList = modelMesh.vertexList;
        auto& modelIndexList = modelMesh.indexList;

        AppMesh mesh;
        mesh.modelMeshId = i;
        mesh.modelMesh = &modelMesh;
        mesh.material = 0;//&m_materialList[m_fbxLoader.GetMaterialId(modelMesh.materialName)];

        f->glGenVertexArrays(1, &mesh.vertexArray);
        f->glBindVertexArray(mesh.vertexArray);

        f->glGenBuffers(1, &mesh.vertexBuffer);
        f->glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(modelVertexList[0]) * modelVertexList.size(), modelVertexList.data(), GL_STATIC_DRAW);

        f->glEnableVertexAttribArray(ATTRIB_VERTEX);
        f->glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, position)));
        f->glEnableVertexAttribArray(ATTRIB_NORMAL);
        f->glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, normal)));
        f->glEnableVertexAttribArray(ATTRIB_TEXCOORD0);
        f->glVertexAttribPointer(ATTRIB_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, uv0)));
        f->glEnableVertexAttribArray(ATTRIB_BONE_INDEX);
        f->glVertexAttribIPointer(ATTRIB_BONE_INDEX, 4, GL_UNSIGNED_BYTE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, boneIndex)));
        f->glEnableVertexAttribArray(ATTRIB_BONE_WEIGHT);
        f->glVertexAttribPointer(ATTRIB_BONE_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, boneWeight)));

        f->glGenBuffers(1, &mesh.indexBuffer);
        f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);
        f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(modelIndexList[0]) * modelIndexList.size(), modelIndexList.data(), GL_STATIC_DRAW);

        f->glBindVertexArray(0);


        m_meshlist.push_back(mesh);
    }

}

void FbxRenderer::update( UniformVs uniformvs)
{


    m_uniformVs.lightDirection = uniformvs.lightDirection;
    m_uniformVs.normalMatrix = uniformvs.normalMatrix;
    m_uniformVs.modelViewMatrix = uniformvs.modelViewMatrix;
    m_uniformVs.projectionMatrix = uniformvs.projectionMatrix;


}


void FbxRenderer::render(QOpenGLContext *share, HandInfo *handinfo , UniformVs uniformvs)
{

    this->update(uniformvs);

    QOpenGLFunctions_3_3_Core* f = 0;
    f = share->versionFunctions<QOpenGLFunctions_3_3_Core>();

    auto drawFunc = [=](const std::vector<AppMesh>& meshlist)
    {
        for (int i = 0; i< meshlist.size(); ++i)
        {
            auto& mesh = meshlist[i];

            GLint* uniforms = nullptr;
            uniforms = uniformsSkin;

            hand_program->bind();

            m_fbxLoader.GetMeshMatrix(m_frame, mesh.modelMeshId, m_uniformVs.meshMatrix);
            m_fbxLoader.GetBoneMatrix(m_frame, mesh.modelMeshId, m_uniformVs.boneMatrixList, MAX_BONE_COUNT, handinfo);

            // uniform buffer copy
            f->glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferVs);
            f->glBufferData(GL_UNIFORM_BUFFER, sizeof(m_uniformVs), &m_uniformVs, GL_DYNAMIC_DRAW);
            f->glBindBufferBase(GL_UNIFORM_BUFFER, uniforms[UNIFORM_VS], m_uniformBufferVs);

            f->glBindVertexArray(mesh.vertexArray);
            /*
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mesh.material->diffuseTexture);
                glBindSampler(0, mesh.material->repeatSampler);
                glUniform1i(uniforms[UNIFORM_DIFFUSE_TEXTURE], 0);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, mesh.material->falloffTexture);
                glBindSampler(1, mesh.material->clampSampler);
                glUniform1i(uniforms[UNIFORM_FALLOFF_TEXTURE], 1);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, mesh.material->rimlightTexture);
                glBindSampler(2, mesh.material->clampSampler);
                glUniform1i(uniforms[UNIFORM_RIMLIGHT_TEXTURE], 2);
*/
            /*  if (mesh.material->specularTexture != 0)
                {
                  glActiveTexture(GL_TEXTURE3);
                  glBindTexture(GL_TEXTURE_2D, mesh.material->specularTexture);
                  glBindSampler(3, mesh.material->repeatSampler);
                  glUniform1i(uniforms[UNIFORM_SPECULAR_TEXTURE], 3);
                }
*/

            f->glDrawElements(GL_TRIANGLES, (GLsizei)mesh.modelMesh->indexList.size(), GL_UNSIGNED_SHORT, nullptr);
        }
    };



    f->glUseProgram(hand_program->programId());

    if(handinfo->m_righthand) drawFunc(m_meshlist);
}


GLuint FbxRenderer::loadTexture (QOpenGLContext *share, const std::string & filename)
{
    QOpenGLFunctions_3_3_Core* f = 0;
    f = share->versionFunctions<QOpenGLFunctions_3_3_Core>();

    FIBITMAP *dib1 = nullptr;

    if (filename.size() == 0)
    {
        return 0;
    }

    auto it = m_textureDictionary.find(filename);
    if ( it != m_textureDictionary.end())
    {
        return it->second;
    }

    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filename.data());

    dib1 = FreeImage_Load(fif, filename.data(), JPEG_DEFAULT);
    if (!dib1)
    {
        std::cerr << "Erreur ouverture d\'image" << std::endl;
        return 0;
    }

    GLuint tex_id = 0;
    int x, y;
    int height, width;


    RGBQUAD rgbquad;


    FREE_IMAGE_TYPE type;
    BITMAPINFOHEADER *header;

    type = FreeImage_GetImageType(dib1);


    height = FreeImage_GetHeight(dib1);
    width = FreeImage_GetWidth(dib1);

    header = FreeImage_GetInfoHeader(dib1);
    int scanLineWidh = ((3*width)%4 == 0) ? 3*width : ((3*width)/4)*4+4;
    unsigned char * texels= (GLubyte*)calloc(height*scanLineWidh, sizeof(GLubyte));
    for (x=0 ; x<width ; x++)
        for (y=0 ; y<height; y++)
        {
            FreeImage_GetPixelColor(dib1,x,y,&rgbquad);

            texels[(y*scanLineWidh+3*x)]=((GLubyte*)&rgbquad)[2];
            texels[(y*scanLineWidh+3*x)+1]=((GLubyte*)&rgbquad)[1];
            texels[(y*scanLineWidh+3*x)+2]=((GLubyte*)&rgbquad)[0];
        }

    f->glGenTextures (1, &tex_id);
    f->glBindTexture (GL_TEXTURE_2D, tex_id);


    f->glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
                  width, height, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, texels);
    free(texels);

    m_textureDictionary.insert({filename, tex_id});

    return tex_id;
}




