#include "PostEffect.h"

using namespace DirectX;

PostEffect::PostEffect()
{
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList, TexManager* texture, ID3D12Device* dev)
{
	cmdList->SetPipelineState(SpritePipelinestate.Get());

	cmdList->SetGraphicsRootSignature(SpriteRootsignature.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	ID3D12DescriptorHeap* ppHeaps[] = { texture->descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &spriteVBView);

	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, spriteConstBuff->GetGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1,
		texture->FindReturnSRV(texnumber, dev));

	//�`��R�}���h
	cmdList->DrawInstanced(4, 1, 0, 0);
}
