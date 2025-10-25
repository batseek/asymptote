<old_str>void AsyVkRender::recreateSwapChain()
{
  device->waitIdle();

  try {
    // Reset timeline semaphore values to avoid timeout issues
    currentTimelineValue = 0;
    for (auto& frameObj : frameObjects) {
      frameObj.timelineValue = 0;
      frameObj.computeTimelineValue = 0;  // Also reset compute timeline value
    }

    // Recreate the timeline semaphore to ensure clean state after resize
    // This prevents semaphore wait timeouts when View=true, Opaque=false
    if (timelineSemaphoreSupported) {
      renderTimelineSemaphore.reset();
      renderTimelineSemaphore = createTimelineSemaphore(0);
    }

    resetDepth=true;
    createSwapChain();

    if (fxaa)
      setupPostProcessingComputeParameters();

    createDependentBuffers();
    createImmediateRenderTargets();

    if (fxaa) {
      preImageInGeneralLayout.resize(backbufferImages.size(), true);
      transitionFXAAImages();

      // Recreate the post-process descriptor sets from scratch
      postProcessDescSet.clear();

      // Reallocate descriptor sets with the new layout
      std::vector<vk::DescriptorSetLayout> postProcessDescLayouts(backbufferImages.size(), *postProcessDescSetLayout);
      try {
        postProcessDescSet = device->allocateDescriptorSetsUnique({*postProcessDescPool, VEC_VIEW(postProcessDescLayouts)});

        // Write the new descriptor sets with the new image views
        writePostProcessDescSets();
      } catch (const std::exception& e) {
        runtimeError("Failed to allocate post-process descriptor sets: " +
                     std::string(e.what()));
      }
    }

    writeDescriptorSets();
    writeMaterialAndLightDescriptors();
    createImageViews();
    createSyncObjects();
    createCountRenderPass();
    createGraphicsRenderPass();
    createGraphicsPipelines();
    createAttachments();
    createFramebuffers();
    createExportResources();
  } catch (const vk::OutOfDeviceMemoryError& e) {
    outOfMemory();
  }

  redisplay=true;
  waitEvent=false;
}
</old_str>
<new_str>void AsyVkRender::recreateSwapChain()
{
  device->waitIdle();

  try {
    // Reset timeline semaphore values to avoid timeout issues
    currentTimelineValue = 0;
    for (auto& frameObj : frameObjects) {
      frameObj.timelineValue = 0;
      frameObj.computeTimelineValue = 0;  // Also reset compute timeline value
    }

    // Recreate the timeline semaphore to ensure clean state after resize
    // This prevents semaphore wait timeouts when View=true, Opaque=false
    if (timelineSemaphoreSupported) {
      renderTimelineSemaphore.reset();
      renderTimelineSemaphore = createTimelineSemaphore(0);
    }

    resetDepth=true;
    createSwapChain();

    if (fxaa)
      setupPostProcessingComputeParameters();

    createDependentBuffers();
    createImmediateRenderTargets();

    if (fxaa) {
      preImageInGeneralLayout.resize(backbufferImages.size(), true);
      transitionFXAAImages();

      // Recreate the post-process descriptor sets from scratch
      postProcessDescSet.clear();

      // Reallocate descriptor sets with the new layout
      std::vector<vk::DescriptorSetLayout> postProcessDescLayouts(backbufferImages.size(), *postProcessDescSetLayout);
      try {
        postProcessDescSet = device->allocateDescriptorSetsUnique({*postProcessDescPool, VEC_VIEW(postProcessDescLayouts)});

        // Write the new descriptor sets with the new image views
        writePostProcessDescSets();
      } catch (const std::exception& e) {
        runtimeError("Failed to allocate post-process descriptor sets: " +
                     std::string(e.what()));
      }
    }

    writeDescriptorSets();
    writeMaterialAndLightDescriptors();
    createImageViews();
    createSyncObjects();
    createCountRenderPress();
    createGraphicsRenderPass();
    createGraphicsPipelines();
    createAttachments();
    createFramebuffers();
    createExportResources();
  } catch (const vk::OutOfDeviceMemoryError& e) {
    outOfMemory();
  }

  redisplay=true;
  waitEvent=false;
  
  // Reset events to ensure proper synchronization after buffer recreation
  // This ensures that we wait for fresh compute shader results after resize
  for (auto& frameObj : frameObjects) {
    device->resetEvent(*frameObj.sumFinishedEvent);
  }
}
</new_str>