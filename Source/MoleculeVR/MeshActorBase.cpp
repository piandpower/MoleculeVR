// Fill out your copyright notice in the Description page of Project Settings.

#include "MoleculeVR.h"
#include "MeshActorBase.h"


// Sets default values
AMeshActorBase::AMeshActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RuntimeMesh = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("Runtime Mesh"));
	RootComponent = RuntimeMesh;

	GetVertexColorMaterial();
}

void AMeshActorBase::CreateMesh(string path)
{
	MeshImporter importer(path.c_str());
	vector<Mesh> meshes = importer.GetMeshes();

	TArray<FRuntimeMeshVertexSimple> vertices;
	TArray<int32> triangles;
	
	// Combine all imported meshes into on Section
	int face_index = 0;
	for (int i = 0; i < meshes.size(); ++i) {
		Mesh mesh = meshes[i];
		FRuntimeMeshTangent tangent;

		for (int j = 0; j < mesh.vertices.size(); ++j) {
			vertices.Add(FRuntimeMeshVertexSimple(mesh.vertices[j], mesh.normals[j], tangent, mesh.colors[j]));
		}

		for (uint32_t face : mesh.faces) {
			triangles.Add(face += face_index);
		}
		face_index += mesh.vertices.size();
	}

	RuntimeMesh->CreateMeshSection(0, vertices, triangles);
	RuntimeMesh->SetMaterial(0, VertexColorMaterial);
}

// Called when the game starts or when spawned
void AMeshActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeshActorBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AMeshActorBase::GetVertexColorMaterial()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/VirtualReality/Materials/VertexColor.VertexColor'"));

	if (Material.Object != NULL)
	{
		VertexColorMaterial = (UMaterial*)Material.Object;
	}

	// Maybe later use for dynamic materials
	//UMaterialInstanceDynamic* dynamic_vertex_color_material = UMaterialInstanceDynamic::Create(vertex_color_material, NULL);

}
