#include "fbxdrawer.h"
#include "environmentreader.h"

fbxDrawer::fbxDrawer()
{
    m_eworld = Matrix4f::Identity();
}

fbxDrawer::~fbxDrawer()
{

}

void fbxDrawer::Initialize(ModelUpdate &&updatedata)
{

    m_pid = m_shaderobj.GenProgram(QString( (Relost::Environment::shader_path + updatedata.sh_filename).data() ), QString( (Relost::Environment::shader_path + updatedata.sh_filename).data() ) );

    uniformsSkin[UNIFORM_VS] = glGetUniformBlockIndex(m_pid, "UniformVs");
    uniformsSkin[UNIFORM_DIFFUSE_TEXTURE] = glGetUniformBlockIndex(m_pid, "diffuseTexture");
    uniformsSkin[UNIFORM_FALLOFF_TEXTURE] = glGetUniformBlockIndex(m_pid, "falloffTexture");
    uniformsSkin[UNIFORM_RIMLIGHT_TEXTURE] = glGetUniformBlockIndex(m_pid, "rimlightTexture");
    uniformsSkin[UNIFORM_SPECULAR_TEXTURE] = glGetUniformBlockIndex(m_pid, "specularTexture");


    m_fbxLoader.Initialize( (Relost::Environment::model_path + "hand_rig_fixed.fbx").data() );

    qDebug() << "initialize ?" << m_fbxLoader.GetMaterialCount() ;


    m_fbxLoader.LoadAnimation( (Relost::Environment::model_path + "hand_rig_fixed.fbx").data() );


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

            glGenVertexArrays(1, &mesh.vertexArray);
            glBindVertexArray(mesh.vertexArray);

            glGenBuffers(1, &mesh.vertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(modelVertexList[0]) * modelVertexList.size(), modelVertexList.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(ATTRIB_VERTEX);
            glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, position)));
            glEnableVertexAttribArray(ATTRIB_NORMAL);
            glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, normal)));
            glEnableVertexAttribArray(ATTRIB_TEXCOORD0);
            glVertexAttribPointer(ATTRIB_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, uv0)));
            glEnableVertexAttribArray(ATTRIB_BONE_INDEX);
            glVertexAttribIPointer(ATTRIB_BONE_INDEX, 4, GL_UNSIGNED_BYTE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, boneIndex)));
            glEnableVertexAttribArray(ATTRIB_BONE_WEIGHT);
            glVertexAttribPointer(ATTRIB_BONE_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, boneWeight)));

            glGenBuffers(1, &mesh.indexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(modelIndexList[0]) * modelIndexList.size(), modelIndexList.data(), GL_STATIC_DRAW);

            glBindVertexArray(0);


            m_meshlist.push_back(mesh);
          }
}

void fbxDrawer::ReloadShader(std::string sh_filename)
{

}

void fbxDrawer::render(){

    auto drawFunc = [=](const std::vector<AppMesh>& meshlist)
    {
        for (int i = 0; i< meshlist.size(); ++i)
        {
            auto& mesh = meshlist[i];

            GLint* uniforms = nullptr;
            uniforms = uniformsSkin;

            glLinkProgram(m_pid);

            m_fbxLoader.GetMeshMatrix(0, mesh.modelMeshId, m_uniformVs.meshMatrix);
            m_fbxLoader.GetBoneMatrix(0, mesh.modelMeshId, m_uniformVs.boneMatrixList,m_bonemaxcount);

                // uniform buffer copy
            glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferVs);
                glBufferData(GL_UNIFORM_BUFFER, sizeof(m_uniformVs), &m_uniformVs, GL_DYNAMIC_DRAW);
                glBindBufferBase(GL_UNIFORM_BUFFER, uniforms[UNIFORM_VS], m_uniformBufferVs);

                glBindVertexArray(mesh.vertexArray);
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

                glDrawElements(GL_TRIANGLES, (GLsizei)mesh.modelMesh->indexList.size(), GL_UNSIGNED_SHORT, nullptr);
        }
    };

    glDisable(GL_BLEND);
    drawFunc(m_meshlist);
}

void fbxDrawer::update(ModelUpdate &&updatedata){
    m_uniformVs.lightDirection = EigenVector4fMake(0.0f, 0.0f, 500.0f, 0.0f);
    m_uniformVs.lightDirection = m_uniformVs.lightDirection.normalized();

    m_uniformVs.cameraPosition = EigenVector4fMake(0.0f, 10.0f, 60.0f, 0.0f);

    Eigen::Vector3f position = EigenVector3fMake(0.0f, 10.0f, 60.0f);

    Eigen::Quaternionf quat;
     Eigen::Vector3f axis;
     axis<<0,0,1;
    Eigen::Vector3f m_eye = position;
    Eigen::Vector3f m_center= RegenUtil::EigenVector3fMake(0.0f, 0.0f, 0.0f);
    Eigen::Vector3f m_eyeUp= RegenUtil::EigenVector3fMake(0.0f, 1.0f , 0.0f);


    m_uniformVs.normalMatrix = m_eworld.inverse();
    m_uniformVs.modelViewMatrix = m_ecamera * m_eworld;
    m_uniformVs.projectionMatrix = m_eproj;


    //m_rotation += self.timeSinceLastUpdate * 0.5f;


}

void fbxDrawer::set(){

}

GLuint fbxDrawer::loadTexture (const std::string & filename)
{
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

      glGenTextures (1, &tex_id);
      glBindTexture (GL_TEXTURE_2D, tex_id);


      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
            width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, texels);
      free(texels);

      m_textureDictionary.insert({filename, tex_id});

  return tex_id;
}

