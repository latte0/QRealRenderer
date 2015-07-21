######################################################################
# Automatically generated by qmake (2.01a) ? 4? 5 17:27:21 2015
######################################################################

TEMPLATE = app
TARGET =

CONFIG += c++14

QT       += core gui opengl multimedia network quick qml webengine

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TARGET = Relost
TEMPLATE = app

INCLUDEPATH += /usr/local/include/ \
                /usr/local/include/bullet \
                /usr/local/include/eigen3 \
                /usr/local/include/glm \
                /usr/include/ \
                ./external/LibOVRKernel/Src/ \
                ./external/LibOVR/Src/ \
                ./external/CommonSrc/ \

INCLUDEPATH += .
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2
INCLUDEPATH += /usr/local/include/opencv2/core
INCLUDEPATH += /usr/local/include/opencv2/highgui
INCLUDEPATH += /usr/local/include/opencv2/imgproc
INCLUDEPATH += /usr/local/include/opencv2/flann
INCLUDEPATH += /usr/local/include/opencv2/photo
INCLUDEPATH += /usr/local/include/opencv2/video
INCLUDEPATH += /usr/local/include/opencv2/features2d
INCLUDEPATH += /usr/local/include/opencv2/objdetect
INCLUDEPATH += /usr/local/include/opencv2/calib3d
INCLUDEPATH += /usr/local/include/opencv2/ml
INCLUDEPATH += /usr/local/include/opencv2/contrib
LIBS += `pkg-config opencv --cflags --libs`




LIBS += -L"/usr/local/lib"\
        -L"/usr/lib/x86_64-linux-gnu" \
    	-lBulletDynamics \
	-lBulletCollision \
	-lBulletSoftBody \
        -lLinearMath \
        -lfbxsdk \
        -ldl \
        -lm \
        -lOVR \
        -L"./external/lib/x64/" \
        -L"./external/lib/" \
        -lLeap \
        -L"./external/external_opengl/FreeImage/linux64/" \
        -lfreeimage \
        -lX11 \
        -lXrandr \
        -lrt \
        -lpthread \
        -lopencv_highgui \
        -lopencv_core \
        -lopencv_imgproc \
        -lnsl \
        -lresolv \
        -lstdc++ \
        -lboost_system \

CONFIG += link_pkgconfig
PKGCONFIG += opencv




DEPENDPATH += . \
              external \
              external/bullet \
              external/cppitertools \
              external/cpptoml \
              external/bullet/BulletCollision \
              external/bullet/BulletDynamics \
              external/bullet/BulletSoftBody \
              external/bullet/LinearMath \
              external/bullet/vectormath \
              external/external_opengl/FreeImage \
              external/external_opengl/GL \
              external/external_opengl/glm \
              external/bullet/BulletCollision/BroadphaseCollision \
              external/bullet/BulletCollision/CollisionDispatch \
              external/bullet/BulletCollision/CollisionShapes \
              external/bullet/BulletCollision/Gimpact \
              external/bullet/BulletCollision/NarrowPhaseCollision \
              external/bullet/BulletDynamics/Character \
              external/bullet/BulletDynamics/ConstraintSolver \
              external/bullet/BulletDynamics/Dynamics \
              external/bullet/BulletDynamics/Featherstone \
              external/bullet/BulletDynamics/MLCPSolvers \
              external/bullet/BulletDynamics/Vehicle \
              external/bullet/vectormath/scalar \
              external/bullet/vectormath/sse \
              external/external_opengl/glm/detail \
              external/external_opengl/glm/gtc \
              external/external_opengl/glm/gtx
INCLUDEPATH += . \
               external/external_opengl/glm \
               external/external_opengl/glm/detail

# Input
HEADERS += \
           adminwidget.h \
           mainwindow.h \
           external/catch.hpp \
           external/cpplinq.hpp \
           external/Leap.h \
           external/LeapMath.h \
           external/bullet/btBulletCollisionCommon.h \
           external/bullet/btBulletDynamicsCommon.h \
           external/bullet/Bullet-C-Api.h \
           external/cppitertools/accumulate.hpp \
           external/cppitertools/chain.hpp \
           external/cppitertools/combinations.hpp \
           external/cppitertools/combinations_with_replacement.hpp \
           external/cppitertools/compress.hpp \
           external/cppitertools/count.hpp \
           external/cppitertools/cycle.hpp \
           external/cppitertools/dropwhile.hpp \
           external/cppitertools/enumerate.hpp \
           external/cppitertools/filter.hpp \
           external/cppitertools/filterfalse.hpp \
           external/cppitertools/groupby.hpp \
           external/cppitertools/grouper.hpp \
           external/cppitertools/imap.hpp \
           external/cppitertools/iteratoriterator.hpp \
           external/cppitertools/iterbase.hpp \
           external/cppitertools/itertools.hpp \
           external/cppitertools/permutations.hpp \
           external/cppitertools/powerset.hpp \
           external/cppitertools/product.hpp \
           external/cppitertools/range.hpp \
           external/cppitertools/repeat.hpp \
           external/cppitertools/reversed.hpp \
           external/cppitertools/slice.hpp \
           external/cppitertools/sliding_window.hpp \
           external/cppitertools/sorted.hpp \
           external/cppitertools/takewhile.hpp \
           external/cppitertools/unique_everseen.hpp \
           external/cppitertools/unique_justseen.hpp \
           external/cppitertools/zip.hpp \
           external/cppitertools/zip_longest.hpp \
           external/bullet/BulletCollision/btBulletCollisionCommon.h \
           external/bullet/BulletDynamics/btBulletDynamicsCommon.h \
           external/bullet/BulletSoftBody/btDefaultSoftBodySolver.h \
           external/bullet/BulletSoftBody/btSoftBody.h \
           external/bullet/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.h \
           external/bullet/BulletSoftBody/btSoftBodyData.h \
           external/bullet/BulletSoftBody/btSoftBodyHelpers.h \
           external/bullet/BulletSoftBody/btSoftBodyInternals.h \
           external/bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h \
           external/bullet/BulletSoftBody/btSoftBodySolvers.h \
           external/bullet/BulletSoftBody/btSoftBodySolverVertexBuffer.h \
           external/bullet/BulletSoftBody/btSoftRigidCollisionAlgorithm.h \
           external/bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h \
           external/bullet/BulletSoftBody/btSoftSoftCollisionAlgorithm.h \
           external/bullet/BulletSoftBody/btSparseSDF.h \
           external/bullet/LinearMath/btAabbUtil2.h \
           external/bullet/LinearMath/btAlignedAllocator.h \
           external/bullet/LinearMath/btAlignedObjectArray.h \
           external/bullet/LinearMath/btConvexHull.h \
           external/bullet/LinearMath/btConvexHullComputer.h \
           external/bullet/LinearMath/btDefaultMotionState.h \
           external/bullet/LinearMath/btGeometryUtil.h \
           external/bullet/LinearMath/btGrahamScan2dConvexHull.h \
           external/bullet/LinearMath/btHashMap.h \
           external/bullet/LinearMath/btIDebugDraw.h \
           external/bullet/LinearMath/btList.h \
           external/bullet/LinearMath/btMatrix3x3.h \
           external/bullet/LinearMath/btMatrixX.h \
           external/bullet/LinearMath/btMinMax.h \
           external/bullet/LinearMath/btMotionState.h \
           external/bullet/LinearMath/btPolarDecomposition.h \
           external/bullet/LinearMath/btPoolAllocator.h \
           external/bullet/LinearMath/btQuadWord.h \
           external/bullet/LinearMath/btQuaternion.h \
           external/bullet/LinearMath/btQuickprof.h \
           external/bullet/LinearMath/btRandom.h \
           external/bullet/LinearMath/btScalar.h \
           external/bullet/LinearMath/btSerializer.h \
           external/bullet/LinearMath/btStackAlloc.h \
           external/bullet/LinearMath/btTransform.h \
           external/bullet/LinearMath/btTransformUtil.h \
           external/bullet/LinearMath/btVector3.h \
           external/bullet/vectormath/vmInclude.h \
           external/cpptoml/cpptoml.h \
           external/external_opengl/FreeImage/FreeImage.h \
           external/external_opengl/GL/glew.h \
           external/external_opengl/GL/glext.h \
           external/external_opengl/GL/glxew.h \
           external/external_opengl/GL/glxext.h \
           external/external_opengl/GL/wglew.h \
           external/external_opengl/GL/wglext.h \
           external/external_opengl/glm/common.hpp \
           external/external_opengl/glm/exponential.hpp \
           external/external_opengl/glm/ext.hpp \
           external/external_opengl/glm/fwd.hpp \
           external/external_opengl/glm/geometric.hpp \
           external/external_opengl/glm/glm.hpp \
           external/external_opengl/glm/integer.hpp \
           external/external_opengl/glm/mat2x2.hpp \
           external/external_opengl/glm/mat2x3.hpp \
           external/external_opengl/glm/mat2x4.hpp \
           external/external_opengl/glm/mat3x2.hpp \
           external/external_opengl/glm/mat3x3.hpp \
           external/external_opengl/glm/mat3x4.hpp \
           external/external_opengl/glm/mat4x2.hpp \
           external/external_opengl/glm/mat4x3.hpp \
           external/external_opengl/glm/mat4x4.hpp \
           external/external_opengl/glm/matrix.hpp \
           external/external_opengl/glm/packing.hpp \
           external/external_opengl/glm/trigonometric.hpp \
           external/external_opengl/glm/vec2.hpp \
           external/external_opengl/glm/vec3.hpp \
           external/external_opengl/glm/vec4.hpp \
           external/external_opengl/glm/vector_relational.hpp \
           external/bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.h \
           external/bullet/BulletCollision/BroadphaseCollision/btBroadphaseInterface.h \
           external/bullet/BulletCollision/BroadphaseCollision/btBroadphaseProxy.h \
           external/bullet/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h \
           external/bullet/BulletCollision/BroadphaseCollision/btDbvt.h \
           external/bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h \
           external/bullet/BulletCollision/BroadphaseCollision/btDispatcher.h \
           external/bullet/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.h \
           external/bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h \
           external/bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h \
           external/bullet/BulletCollision/BroadphaseCollision/btQuantizedBvh.h \
           external/bullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h \
           external/bullet/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.h \
           external/bullet/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h \
           external/bullet/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.h \
           external/bullet/BulletCollision/CollisionDispatch/btBoxBoxDetector.h \
           external/bullet/BulletCollision/CollisionDispatch/btCollisionConfiguration.h \
           external/bullet/BulletCollision/CollisionDispatch/btCollisionCreateFunc.h \
           external/bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h \
           external/bullet/BulletCollision/CollisionDispatch/btCollisionObject.h \
           external/bullet/BulletCollision/CollisionDispatch/btCollisionObjectWrapper.h \
           external/bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h \
           external/bullet/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.h \
           external/bullet/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.h \
           external/bullet/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h \
           external/bullet/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.h \
           external/bullet/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.h \
           external/bullet/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.h \
           external/bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h \
           external/bullet/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h \
           external/bullet/BulletCollision/CollisionDispatch/btGhostObject.h \
           external/bullet/BulletCollision/CollisionDispatch/btHashedSimplePairCache.h \
           external/bullet/BulletCollision/CollisionDispatch/btInternalEdgeUtility.h \
           external/bullet/BulletCollision/CollisionDispatch/btManifoldResult.h \
           external/bullet/BulletCollision/CollisionDispatch/btSimulationIslandManager.h \
           external/bullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h \
           external/bullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h \
           external/bullet/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h \
           external/bullet/BulletCollision/CollisionDispatch/btUnionFind.h \
           external/bullet/BulletCollision/CollisionDispatch/SphereTriangleDetector.h \
           external/bullet/BulletCollision/CollisionShapes/btBox2dShape.h \
           external/bullet/BulletCollision/CollisionShapes/btBoxShape.h \
           external/bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h \
           external/bullet/BulletCollision/CollisionShapes/btCapsuleShape.h \
           external/bullet/BulletCollision/CollisionShapes/btCollisionMargin.h \
           external/bullet/BulletCollision/CollisionShapes/btCollisionShape.h \
           external/bullet/BulletCollision/CollisionShapes/btCompoundShape.h \
           external/bullet/BulletCollision/CollisionShapes/btConcaveShape.h \
           external/bullet/BulletCollision/CollisionShapes/btConeShape.h \
           external/bullet/BulletCollision/CollisionShapes/btConvex2dShape.h \
           external/bullet/BulletCollision/CollisionShapes/btConvexHullShape.h \
           external/bullet/BulletCollision/CollisionShapes/btConvexInternalShape.h \
           external/bullet/BulletCollision/CollisionShapes/btConvexPointCloudShape.h \
           external/bullet/BulletCollision/CollisionShapes/btConvexPolyhedron.h \
           external/bullet/BulletCollision/CollisionShapes/btConvexShape.h \
           external/bullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h \
           external/bullet/BulletCollision/CollisionShapes/btCylinderShape.h \
           external/bullet/BulletCollision/CollisionShapes/btEmptyShape.h \
           external/bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h \
           external/bullet/BulletCollision/CollisionShapes/btMaterial.h \
           external/bullet/BulletCollision/CollisionShapes/btMinkowskiSumShape.h \
           external/bullet/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.h \
           external/bullet/BulletCollision/CollisionShapes/btMultiSphereShape.h \
           external/bullet/BulletCollision/CollisionShapes/btOptimizedBvh.h \
           external/bullet/BulletCollision/CollisionShapes/btPolyhedralConvexShape.h \
           external/bullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h \
           external/bullet/BulletCollision/CollisionShapes/btShapeHull.h \
           external/bullet/BulletCollision/CollisionShapes/btSphereShape.h \
           external/bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.h \
           external/bullet/BulletCollision/CollisionShapes/btStridingMeshInterface.h \
           external/bullet/BulletCollision/CollisionShapes/btTetrahedronShape.h \
           external/bullet/BulletCollision/CollisionShapes/btTriangleBuffer.h \
           external/bullet/BulletCollision/CollisionShapes/btTriangleCallback.h \
           external/bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h \
           external/bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.h \
           external/bullet/BulletCollision/CollisionShapes/btTriangleInfoMap.h \
           external/bullet/BulletCollision/CollisionShapes/btTriangleMesh.h \
           external/bullet/BulletCollision/CollisionShapes/btTriangleMeshShape.h \
           external/bullet/BulletCollision/CollisionShapes/btTriangleShape.h \
           external/bullet/BulletCollision/CollisionShapes/btUniformScalingShape.h \
           external/bullet/BulletCollision/Gimpact/btBoxCollision.h \
           external/bullet/BulletCollision/Gimpact/btClipPolygon.h \
           external/bullet/BulletCollision/Gimpact/btCompoundFromGimpact.h \
           external/bullet/BulletCollision/Gimpact/btContactProcessing.h \
           external/bullet/BulletCollision/Gimpact/btGenericPoolAllocator.h \
           external/bullet/BulletCollision/Gimpact/btGeometryOperations.h \
           external/bullet/BulletCollision/Gimpact/btGImpactBvh.h \
           external/bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h \
           external/bullet/BulletCollision/Gimpact/btGImpactMassUtil.h \
           external/bullet/BulletCollision/Gimpact/btGImpactQuantizedBvh.h \
           external/bullet/BulletCollision/Gimpact/btGImpactShape.h \
           external/bullet/BulletCollision/Gimpact/btQuantization.h \
           external/bullet/BulletCollision/Gimpact/btTriangleShapeEx.h \
           external/bullet/BulletCollision/Gimpact/gim_array.h \
           external/bullet/BulletCollision/Gimpact/gim_basic_geometry_operations.h \
           external/bullet/BulletCollision/Gimpact/gim_bitset.h \
           external/bullet/BulletCollision/Gimpact/gim_box_collision.h \
           external/bullet/BulletCollision/Gimpact/gim_box_set.h \
           external/bullet/BulletCollision/Gimpact/gim_clip_polygon.h \
           external/bullet/BulletCollision/Gimpact/gim_contact.h \
           external/bullet/BulletCollision/Gimpact/gim_geom_types.h \
           external/bullet/BulletCollision/Gimpact/gim_geometry.h \
           external/bullet/BulletCollision/Gimpact/gim_hash_table.h \
           external/bullet/BulletCollision/Gimpact/gim_linear_math.h \
           external/bullet/BulletCollision/Gimpact/gim_math.h \
           external/bullet/BulletCollision/Gimpact/gim_memory.h \
           external/bullet/BulletCollision/Gimpact/gim_radixsort.h \
           external/bullet/BulletCollision/Gimpact/gim_tri_collision.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btConvexCast.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btDiscreteCollisionDetectorInterface.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btGjkEpa2.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btManifoldPoint.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btPersistentManifold.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btPointCollector.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btSimplexSolverInterface.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.h \
           external/bullet/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h \
           external/bullet/BulletDynamics/Character/btCharacterControllerInterface.h \
           external/bullet/BulletDynamics/Character/btKinematicCharacterController.h \
           external/bullet/BulletDynamics/ConstraintSolver/btConeTwistConstraint.h \
           external/bullet/BulletDynamics/ConstraintSolver/btConstraintSolver.h \
           external/bullet/BulletDynamics/ConstraintSolver/btContactConstraint.h \
           external/bullet/BulletDynamics/ConstraintSolver/btContactSolverInfo.h \
           external/bullet/BulletDynamics/ConstraintSolver/btFixedConstraint.h \
           external/bullet/BulletDynamics/ConstraintSolver/btGearConstraint.h \
           external/bullet/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h \
           external/bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h \
           external/bullet/BulletDynamics/ConstraintSolver/btHinge2Constraint.h \
           external/bullet/BulletDynamics/ConstraintSolver/btHingeConstraint.h \
           external/bullet/BulletDynamics/ConstraintSolver/btJacobianEntry.h \
           external/bullet/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h \
           external/bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h \
           external/bullet/BulletDynamics/ConstraintSolver/btSliderConstraint.h \
           external/bullet/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.h \
           external/bullet/BulletDynamics/ConstraintSolver/btSolverBody.h \
           external/bullet/BulletDynamics/ConstraintSolver/btSolverConstraint.h \
           external/bullet/BulletDynamics/ConstraintSolver/btTypedConstraint.h \
           external/bullet/BulletDynamics/ConstraintSolver/btUniversalConstraint.h \
           external/bullet/BulletDynamics/Dynamics/btActionInterface.h \
           external/bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h \
           external/bullet/BulletDynamics/Dynamics/btDynamicsWorld.h \
           external/bullet/BulletDynamics/Dynamics/btRigidBody.h \
           external/bullet/BulletDynamics/Dynamics/btSimpleDynamicsWorld.h \
           external/bullet/BulletDynamics/Featherstone/btMultiBody.h \
           external/bullet/BulletDynamics/Featherstone/btMultiBodyConstraint.h \
           external/bullet/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.h \
           external/bullet/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h \
           external/bullet/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.h \
           external/bullet/BulletDynamics/Featherstone/btMultiBodyJointMotor.h \
           external/bullet/BulletDynamics/Featherstone/btMultiBodyLink.h \
           external/bullet/BulletDynamics/Featherstone/btMultiBodyLinkCollider.h \
           external/bullet/BulletDynamics/Featherstone/btMultiBodyPoint2Point.h \
           external/bullet/BulletDynamics/Featherstone/btMultiBodySolverConstraint.h \
           external/bullet/BulletDynamics/MLCPSolvers/btDantzigLCP.h \
           external/bullet/BulletDynamics/MLCPSolvers/btDantzigSolver.h \
           external/bullet/BulletDynamics/MLCPSolvers/btMLCPSolver.h \
           external/bullet/BulletDynamics/MLCPSolvers/btMLCPSolverInterface.h \
           external/bullet/BulletDynamics/MLCPSolvers/btPATHSolver.h \
           external/bullet/BulletDynamics/MLCPSolvers/btSolveProjectedGaussSeidel.h \
           external/bullet/BulletDynamics/Vehicle/btRaycastVehicle.h \
           external/bullet/BulletDynamics/Vehicle/btVehicleRaycaster.h \
           external/bullet/BulletDynamics/Vehicle/btWheelInfo.h \
           external/bullet/vectormath/scalar/boolInVec.h \
           external/bullet/vectormath/scalar/floatInVec.h \
           external/bullet/vectormath/scalar/mat_aos.h \
           external/bullet/vectormath/scalar/quat_aos.h \
           external/bullet/vectormath/scalar/vec_aos.h \
           external/bullet/vectormath/scalar/vectormath_aos.h \
           external/bullet/vectormath/sse/boolInVec.h \
           external/bullet/vectormath/sse/floatInVec.h \
           external/bullet/vectormath/sse/mat_aos.h \
           external/bullet/vectormath/sse/quat_aos.h \
           external/bullet/vectormath/sse/vec_aos.h \
           external/bullet/vectormath/sse/vecidx_aos.h \
           external/bullet/vectormath/sse/vectormath_aos.h \
           external/external_opengl/glm/detail/_features.hpp \
           external/external_opengl/glm/detail/_fixes.hpp \
           external/external_opengl/glm/detail/_noise.hpp \
           external/external_opengl/glm/detail/_swizzle.hpp \
           external/external_opengl/glm/detail/_swizzle_func.hpp \
           external/external_opengl/glm/detail/_vectorize.hpp \
           external/external_opengl/glm/detail/func_common.hpp \
           external/external_opengl/glm/detail/func_exponential.hpp \
           external/external_opengl/glm/detail/func_geometric.hpp \
           external/external_opengl/glm/detail/func_integer.hpp \
           external/external_opengl/glm/detail/func_matrix.hpp \
           external/external_opengl/glm/detail/func_noise.hpp \
           external/external_opengl/glm/detail/func_packing.hpp \
           external/external_opengl/glm/detail/func_trigonometric.hpp \
           external/external_opengl/glm/detail/func_vector_relational.hpp \
           external/external_opengl/glm/detail/intrinsic_common.hpp \
           external/external_opengl/glm/detail/intrinsic_exponential.hpp \
           external/external_opengl/glm/detail/intrinsic_geometric.hpp \
           external/external_opengl/glm/detail/intrinsic_integer.hpp \
           external/external_opengl/glm/detail/intrinsic_matrix.hpp \
           external/external_opengl/glm/detail/intrinsic_trigonometric.hpp \
           external/external_opengl/glm/detail/intrinsic_vector_relational.hpp \
           external/external_opengl/glm/detail/precision.hpp \
           external/external_opengl/glm/detail/setup.hpp \
           external/external_opengl/glm/detail/type_float.hpp \
           external/external_opengl/glm/detail/type_gentype.hpp \
           external/external_opengl/glm/detail/type_half.hpp \
           external/external_opengl/glm/detail/type_int.hpp \
           external/external_opengl/glm/detail/type_mat.hpp \
           external/external_opengl/glm/detail/type_mat2x2.hpp \
           external/external_opengl/glm/detail/type_mat2x3.hpp \
           external/external_opengl/glm/detail/type_mat2x4.hpp \
           external/external_opengl/glm/detail/type_mat3x2.hpp \
           external/external_opengl/glm/detail/type_mat3x3.hpp \
           external/external_opengl/glm/detail/type_mat3x4.hpp \
           external/external_opengl/glm/detail/type_mat4x2.hpp \
           external/external_opengl/glm/detail/type_mat4x3.hpp \
           external/external_opengl/glm/detail/type_mat4x4.hpp \
           external/external_opengl/glm/detail/type_vec.hpp \
           external/external_opengl/glm/detail/type_vec1.hpp \
           external/external_opengl/glm/detail/type_vec2.hpp \
           external/external_opengl/glm/detail/type_vec3.hpp \
           external/external_opengl/glm/detail/type_vec4.hpp \
           external/external_opengl/glm/gtc/bitfield.hpp \
           external/external_opengl/glm/gtc/constants.hpp \
           external/external_opengl/glm/gtc/epsilon.hpp \
           external/external_opengl/glm/gtc/integer.hpp \
           external/external_opengl/glm/gtc/matrix_access.hpp \
           external/external_opengl/glm/gtc/matrix_integer.hpp \
           external/external_opengl/glm/gtc/matrix_inverse.hpp \
           external/external_opengl/glm/gtc/matrix_transform.hpp \
           external/external_opengl/glm/gtc/noise.hpp \
           external/external_opengl/glm/gtc/packing.hpp \
           external/external_opengl/glm/gtc/quaternion.hpp \
           external/external_opengl/glm/gtc/random.hpp \
           external/external_opengl/glm/gtc/reciprocal.hpp \
           external/external_opengl/glm/gtc/round.hpp \
           external/external_opengl/glm/gtc/type_precision.hpp \
           external/external_opengl/glm/gtc/type_ptr.hpp \
           external/external_opengl/glm/gtc/ulp.hpp \
           external/external_opengl/glm/gtc/vec1.hpp \
           external/external_opengl/glm/gtx/associated_min_max.hpp \
           external/external_opengl/glm/gtx/bit.hpp \
           external/external_opengl/glm/gtx/closest_point.hpp \
           external/external_opengl/glm/gtx/color_space.hpp \
           external/external_opengl/glm/gtx/color_space_YCoCg.hpp \
           external/external_opengl/glm/gtx/common.hpp \
           external/external_opengl/glm/gtx/compatibility.hpp \
           external/external_opengl/glm/gtx/component_wise.hpp \
           external/external_opengl/glm/gtx/dual_quaternion.hpp \
           external/external_opengl/glm/gtx/euler_angles.hpp \
           external/external_opengl/glm/gtx/extend.hpp \
           external/external_opengl/glm/gtx/extented_min_max.hpp \
           external/external_opengl/glm/gtx/fast_exponential.hpp \
           external/external_opengl/glm/gtx/fast_square_root.hpp \
           external/external_opengl/glm/gtx/fast_trigonometry.hpp \
           external/external_opengl/glm/gtx/gradient_paint.hpp \
           external/external_opengl/glm/gtx/handed_coordinate_space.hpp \
           external/external_opengl/glm/gtx/integer.hpp \
           external/external_opengl/glm/gtx/intersect.hpp \
           external/external_opengl/glm/gtx/io.hpp \
           external/external_opengl/glm/gtx/log_base.hpp \
           external/external_opengl/glm/gtx/matrix_cross_product.hpp \
           external/external_opengl/glm/gtx/matrix_decompose.hpp \
           external/external_opengl/glm/gtx/matrix_interpolation.hpp \
           external/external_opengl/glm/gtx/matrix_major_storage.hpp \
           external/external_opengl/glm/gtx/matrix_operation.hpp \
           external/external_opengl/glm/gtx/matrix_query.hpp \
           external/external_opengl/glm/gtx/matrix_transform_2d.hpp \
           external/external_opengl/glm/gtx/mixed_product.hpp \
           external/external_opengl/glm/gtx/multiple.hpp \
           external/external_opengl/glm/gtx/norm.hpp \
           external/external_opengl/glm/gtx/normal.hpp \
           external/external_opengl/glm/gtx/normalize_dot.hpp \
           external/external_opengl/glm/gtx/number_precision.hpp \
           external/external_opengl/glm/gtx/optimum_pow.hpp \
           external/external_opengl/glm/gtx/orthonormalize.hpp \
           external/external_opengl/glm/gtx/perpendicular.hpp \
           external/external_opengl/glm/gtx/polar_coordinates.hpp \
           external/external_opengl/glm/gtx/projection.hpp \
           external/external_opengl/glm/gtx/quaternion.hpp \
           external/external_opengl/glm/gtx/range.hpp \
           external/external_opengl/glm/gtx/raw_data.hpp \
           external/external_opengl/glm/gtx/rotate_normalized_axis.hpp \
           external/external_opengl/glm/gtx/rotate_vector.hpp \
           external/external_opengl/glm/gtx/scalar_multiplication.hpp \
           external/external_opengl/glm/gtx/scalar_relational.hpp \
           external/external_opengl/glm/gtx/simd_mat4.hpp \
           external/external_opengl/glm/gtx/simd_quat.hpp \
           external/external_opengl/glm/gtx/simd_vec4.hpp \
           external/external_opengl/glm/gtx/spline.hpp \
           external/external_opengl/glm/gtx/std_based_type.hpp \
           external/external_opengl/glm/gtx/string_cast.hpp \
           external/external_opengl/glm/gtx/transform.hpp \
           external/external_opengl/glm/gtx/transform2.hpp \
           external/external_opengl/glm/gtx/type_aligned.hpp \
           external/external_opengl/glm/gtx/vector_angle.hpp \
           external/external_opengl/glm/gtx/vector_query.hpp \
           external/external_opengl/glm/gtx/wrap.hpp \
           external/external_opengl/glm/detail/type_vec2.inl \
           external/external_opengl/glm/detail/type_vec3.inl \
           external/external_opengl/glm/detail/type_vec4.inl \
           external/external_opengl/glm/detail/type_vec4_sse2.inl \
           external/external_opengl/glm/detail/type_vec4_avx.inl \
           external/external_opengl/glm/detail/type_vec4_avx2.inl \
           external/external_opengl/glm/detail/type_mat2x2.inl \
           external/external_opengl/glm/detail/type_mat2x3.inl \
           external/external_opengl/glm/detail/type_mat2x4.inl \
           external/external_opengl/glm/detail/type_mat3x2.inl \
           external/external_opengl/glm/detail/type_mat3x3.inl \
           external/external_opengl/glm/detail/type_mat3x4.inl \
           external/external_opengl/glm/detail/type_mat4x2.inl \
           external/external_opengl/glm/detail/type_mat4x3.inl \
           external/external_opengl/glm/detail/type_mat4x4.inl \
           external/external_opengl/glm/detail/func_trigonometric.inl \
           external/external_opengl/glm/detail/type_vec1.inl \
           external/external_opengl/glm/detail/func_exponential.inl \
           external/external_opengl/glm/detail/func_vector_relational.inl \
           external/external_opengl/glm/detail/func_common.inl \
           external/external_opengl/glm/detail/func_packing.inl \
           external/external_opengl/glm/detail/type_half.inl \
           external/external_opengl/glm/detail/func_geometric.inl \
           external/external_opengl/glm/detail/func_matrix.inl \
           external/external_opengl/glm/detail/func_integer.inl \
            external/cpptoml.h \
         eigenutil.h \
    environmentreader.h \
    actortreeitem.h \
    actortreemodel.h \
    mathhelper.h \
    handinfo.h \
    handFBXLoader.h \
    colormodulation.h \
    vertexbufferobject.h \
    rendertexture.h \
    scenerendermodel.h \
    shaderobject.h \
    actor.h \
    camera.h \
    scene.h \
    light.h \
    fbxloader.h \
    player.h \
    scenerender.h \
    networkconfirm.h \
    imagereceiver.h \
    ovrcondition.h \
    hmdinfo.h \
    lineobject.h \
    actorfactory.h \
    renderwindow.h \
    cuberenderer.h \
    backgroundrenderer.h \
    mouserenderer.h \
    glutil.h \
    fbxrenderer.h \
    fbxstruct.h \
    scenario.h \
    urlutil.h \
    singleton.h

SOURCES += adminwidget.cpp \
           main.cpp \
           mainwindow.cpp \
           external/external_opengl/glm/detail/glm.cpp \
    eigenutil.cpp \
    actortreeitem.cpp \
    actortreemodel.cpp \
    mathhelper.cpp \
    handinfo.cpp \
    handFBXLoader.cpp \
    vertexbufferobject.cpp \
    rendertexture.cpp \
    scenerendermodel.cpp \
    shaderobject.cpp \
    actor.cpp \
    camera.cpp \
    scene.cpp \
    light.cpp \
    fbxloader.cpp \
    player.cpp \
    scenerender.cpp \
    networkconfirm.cpp \
    imagereceiver.cpp \
    ovrcondition.cpp \
    lineobject.cpp \
    actorfactory.cpp \
    renderwindow.cpp \
    cuberenderer.cpp \
    backgroundrenderer.cpp \
    mouserenderer.cpp \
    glutil.cpp \
    fbxrenderer.cpp \
    scenario.cpp \
    urlutil.cpp

DISTFILES += \
    Test.qml \
    youtube.qml \
    Paint.qml \
    imageview.qml \
    flickr.qml \
    quickwindow.qml \
    web.qml
