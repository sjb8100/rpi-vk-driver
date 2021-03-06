#include "common.h"

#include "kernel/vc4_packet.h"

VkResult vkCreateShaderModuleFromRpiAssemblyKHR(VkDevice device, VkRpiShaderModuleAssemblyCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
	assert(device);
	assert(pCreateInfo);
	assert(pShaderModule);
	assert(pCreateInfo->byteStreamArray);
	assert(pCreateInfo->numBytesArray);

	assert(pAllocator == 0); //TODO

	_shaderModule* shader = malloc(sizeof(_shaderModule));

	if(!shader)
	{
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	for(int c = 0; c < VK_RPI_ASSEMBLY_TYPE_MAX; ++c)
	{
		if(pCreateInfo->byteStreamArray[c])
		{
			uint32_t size = pCreateInfo->numBytesArray[c];
			shader->bos[c] = vc4_bo_alloc_shader(controlFd, pCreateInfo->byteStreamArray[c], &size);
			shader->sizes[c] = size;
		}
		else
		{
			shader->bos[c] = 0;
		}
	}

	*pShaderModule = shader;

	return VK_SUCCESS;
}

/*
 * https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#vkCreateShaderModule
 */
VkResult vkCreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
	//TODO
	return VK_SUCCESS;
}

void vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
{
	assert(device);
	assert(shaderModule);

	assert(pAllocator == 0);

	_shaderModule* shader = shaderModule;

	for(int c = 0; c < VK_RPI_ASSEMBLY_TYPE_MAX; ++c)
	{
		if(shader->bos[c])
		{
			vc4_bo_free(controlFd, shader->bos[c], 0, shader->sizes[c]);
		}
	}

	free(shader);
}
