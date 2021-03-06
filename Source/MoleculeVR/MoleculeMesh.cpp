// Fill out your copyright notice in the Description page of Project Settings.

#include "MoleculeVR.h"
#include "MoleculeMesh.h"
#include "MoleculePicker.h"

// Sets default values
AMoleculeMesh::AMoleculeMesh()
{
}

void AMoleculeMesh::OnConstruction(const FTransform & Transform)
{
	RuntimeMesh->bUseComplexAsSimpleCollision = false;
}

void AMoleculeMesh::CreateMesh(FString path) {
	MeshImporter importer(TCHAR_TO_ANSI(*path));
	vector<Mesh> meshes = importer.GetMeshes();

	// Split molecule core and surface into different meshes
	vector<Mesh> molecule_core;
	vector<Mesh> molecule_surface;
	for (Mesh mesh : meshes) {
		(mesh.vertices.size() > 18) ? molecule_surface.push_back(mesh) : molecule_core.push_back(mesh);
	}

	// Render core and surfaces meshes as different sections
	TArray<FRuntimeMeshVertexSimple> core_vertices = ExtractSectionVertices(molecule_core);
	TArray<int32> core_faces = ExtractSectionFaces(molecule_core);
	RuntimeMesh->CreateMeshSection(0, core_vertices, core_faces);
	RuntimeMesh->SetMaterial(0, VertexColorMaterial);

	TArray<FRuntimeMeshVertexSimple> sur_vertices = ExtractSectionVertices(molecule_surface);
	TArray<int32> sur_faces = ExtractSectionFaces(molecule_surface);
	RuntimeMesh->CreateMeshSection(1, sur_vertices, sur_faces);
	RuntimeMesh->SetMaterial(1, VertexColorMaterial);

	SetCollisionConvexMesh(molecule_surface);
}

void AMoleculeMesh::SetCollisionConvexMesh(vector<Mesh> collision_meshes) {
	TArray<FVector> collision_mesh;
	for (Mesh mesh : collision_meshes) {
		for (FVector vertex : mesh.vertices) {
			collision_mesh.Add(vertex);
		}
	}
	RuntimeMesh->AddCollisionConvexMesh(collision_mesh);
}

TArray<FRuntimeMeshVertexSimple> AMoleculeMesh::ExtractSectionVertices(vector<Mesh> meshes) {
	TArray<FRuntimeMeshVertexSimple> vertices;
	FRuntimeMeshTangent tangent;
	
	for (Mesh mesh : meshes) {
		for (int j = 0; j < mesh.vertices.size(); ++j) {
			vertices.Add(FRuntimeMeshVertexSimple(mesh.vertices[j], mesh.normals[j], tangent, mesh.colors[j]));
		}
	}
	return vertices;
}

TArray<int32> AMoleculeMesh::ExtractSectionFaces(vector<Mesh> meshes) {
	TArray<int32> triangles;
	int face_index = 0;
	
	for (Mesh mesh : meshes) {
		for (int32 face : mesh.faces) {
			triangles.Add(face += face_index);
		}
		face_index += mesh.vertices.size();
	}
	return triangles;
}

void AMoleculeMesh::SetSimulatePhysics(bool simulate) {
	RuntimeMesh->SetSimulatePhysics(simulate);
}

void AMoleculeMesh::ToggleSurface()
{
	bool isVisible = RuntimeMesh->IsMeshSectionVisible(1);
	RuntimeMesh->SetMeshSectionVisible(1, !isVisible);
}

void AMoleculeMesh::ScaleMolecule(float step)
{
	float current_scale = RuntimeMesh->GetComponentScale().X;
	current_scale = FMath::Clamp(current_scale, 0.5f, 2.5f);
	RuntimeMesh->SetRelativeScale3D(FVector(current_scale + step));
}

// Called when the game starts or when spawned
void AMoleculeMesh::BeginPlay()
{
	Super::BeginPlay();
	SetSimulatePhysics(false);
}

// Called every frame
void AMoleculeMesh::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

