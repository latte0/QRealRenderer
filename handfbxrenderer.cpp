#include "handfbxrenderer.h"

handFbxRenderer::handFbxRenderer()
{

}

handFbxRenderer::~handFbxRenderer()
{

}


void handFbxRenderer::update( UniformVs uniformvs){
    m_uniformVs.lightDirection = uniformvs.lightDirection;
    m_uniformVs.normalMatrix = uniformvs.normalMatrix;
    m_uniformVs.modelViewMatrix = uniformvs.modelViewMatrix;
    m_uniformVs.projectionMatrix = uniformvs.projectionMatrix;


}

void handFbxRenderer::init(std::shared_ptr<QOpenGLContext>& share, const std::string &filename)
{
    m_context = share;

    QOpenGLFunctions_3_3_Core* f = 0;
    f = share->versionFunctions<QOpenGLFunctions_3_3_Core>();

    m_handfbxLoader.Initialize("./resources/model/hand_rig2.fbx");
    qDebug() << "initialize ?" << m_handfbxLoader.GetMaterialCount() ;
    m_handfbxLoader.LoadAnimation("./resources/model/hand_rig2.fbx");
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
    for(int i = 0; i < m_handfbxLoader.GetMaterialCount(); ++i)
    {
        auto& modelMaterial = m_handfbxLoader.GetMaterial(i);

        AppMaterial material;
        material.modelMaterial = &modelMaterial;

        std::string home = "/Textures/";
        // read texture
        material.diffuseTexture = loadTexture(share, home + modelMaterial.diffuseTextureName);
        material.falloffTexture = loadTexture(share, home + modelMaterial.falloffTextureName);
        material.specularTexture = loadTexture(share, home + modelMaterial.specularTextureName);
        material.rimlightTexture = 0;

            // repatsampler
            f->glGenSamplers(1, &material.repeatSampler);
            f->glSamplerParameteri(material.repeatSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
            f->glSamplerParameteri(material.repeatSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
            f->glSamplerParameteri(material.repeatSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            f->glSamplerParameteri(material.repeatSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // clump sampler
            f->glGenSamplers(1, &material.clampSampler);
            f->glSamplerParameteri(material.clampSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            f->glSamplerParameteri(material.clampSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            f->glSamplerParameteri(material.clampSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            f->glSamplerParameteri(material.clampSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            m_materialList.push_back(material);

    }
*/

    for (int i = 0; i < m_handfbxLoader.GetMeshCount(); ++i)
    {
        auto& modelMesh = m_handfbxLoader.GetMesh(i);
        auto& modelVertexList = modelMesh.vertexList;
        auto& modelIndexList = modelMesh.indexList;

        AppMesh mesh;
        mesh.modelMeshId = i;
        mesh.modelMesh = &modelMesh;
        mesh.material = 0;//&m_materialList[m_handfbxLoader.GetMaterialId(modelMesh.materialName)];

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

void handFbxRenderer::render(std::shared_ptr<QOpenGLContext>& share, UniformVs uniformvs){
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

            m_handfbxLoader.GetMeshMatrix(m_frame, mesh.modelMeshId, m_uniformVs.meshMatrix);
            m_handfbxLoader.GetBoneMatrix(m_frame, mesh.modelMeshId, m_uniformVs.boneMatrixList, MAX_BONE_COUNT);

            // uniform buffer copy
            f->glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferVs);
            f->glBufferData(GL_UNIFORM_BUFFER, sizeof(m_uniformVs), &m_uniformVs, GL_DYNAMIC_DRAW);
            f->glBindBufferBase(GL_UNIFORM_BUFFER, uniforms[UNIFORM_VS], m_uniformBufferVs);

            f->glBindVertexArray(mesh.vertexArray);


            /*
                f->glActiveTexture(GL_TEXTURE0);
                f->glBindTexture(GL_TEXTURE_2D, mesh.material->diffuseTexture);
                f->glBindSampler(0, mesh.material->repeatSampler);
                f->glUniform1i(uniforms[UNIFORM_DIFFUSE_TEXTURE], 0);

                f->glActiveTexture(GL_TEXTURE1);
                f->glBindTexture(GL_TEXTURE_2D, mesh.material->falloffTexture);
                f->glBindSampler(1, mesh.material->clampSampler);
                f->glUniform1i(uniforms[UNIFORM_FALLOFF_TEXTURE], 1);

                f->glActiveTexture(GL_TEXTURE2);
                f->glBindTexture(GL_TEXTURE_2D, mesh.material->rimlightTexture);
                f->glBindSampler(2, mesh.material->clampSampler);
                f->glUniform1i(uniforms[UNIFORM_RIMLIGHT_TEXTURE], 2);

              if (mesh.material->specularTexture != 0)
                {
                  f->glActiveTexture(GL_TEXTURE3);
                  f->glBindTexture(GL_TEXTURE_2D, mesh.material->specularTexture);
                  f->glBindSampler(3, mesh.material->repeatSampler);
                  f->glUniform1i(uniforms[UNIFORM_SPECULAR_TEXTURE], 3);
                }

            */
            f->glDrawElements(GL_TRIANGLES, (GLsizei)mesh.modelMesh->indexList.size(), GL_UNSIGNED_SHORT, nullptr);
        }
    };



    f->glUseProgram(hand_program->programId());

    if(HandInfo::singleton().m_righthand) drawFunc(m_meshlist);
}
