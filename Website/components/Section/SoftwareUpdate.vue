<script setup>
  // Selecting a hex file
  const fileInput = ref(null);
  const hexFile = ref(null);
  const toast = useToast();
  function selectFile(event) {
    const newSelectedFile = event.target.files[0];

    // Validate if it's a hex file
    if (newSelectedFile.name.slice(-4) !== '.hex') {
      fileInput.value = null;
      hexFile.value = null;
      toast.add({
        title: 'Invalid file type',
        description: 'Only ".hex" files are supported',
        icon: 'i-simple-line-icons-close',
      });
      return;
    }

    // Saving file content to `hexFile`
    const reader = new FileReader();
    reader.readAsBinaryString(newSelectedFile);
    reader.onload = () => (hexFile.value = reader.result);
    reader.onerror = (e) => {
      hexFile.value = null;
      console.error(e);
    };
  }

  // Verifying the selected file
  useHead({ script: [{ src: './fernetBrowser.js' }] });
  const { sendHexToEcu } = useSocketClient();
  const { hexVerification } = useStatus();

  // Flashing the selected file
  const { flashFileToEcu } = useSocketClient();
  const { hexFlashing } = useStatus();
</script>

<template>
  <section>
    <div class="app-heading mb-2">
      <UIcon name="i-solar-refresh-outline" class="text-lg" />
      <h1>Software Update</h1>
    </div>

    <div class="flex items-center justify-between gap-4">
      <div class="flex gap-4 w-full">
        <AppCard class="gap-2 w-full flex-col !items-start">
          <UInput
            @change="selectFile"
            v-model="fileInput"
            type="file"
            accept=".hex"
            class="w-full"
          />
          <p class="app-caption">Only '.hex' files are supported</p>
        </AppCard>
      </div>

      <div class="flex items-center gap-2">
        <UButton
          @click="sendHexToEcu(hexFile)"
          :loading="hexVerification === 2"
          :disabled="hexFile === null"
        >
          {{ hexVerification === 2 ? 'Verifying' : 'Verify' }}
        </UButton>

        <UButton
          @click="flashFileToEcu"
          :disabled="hexVerification !== 1"
          :loading="hexFlashing !== null && hexFlashing !== 100"
        >
          {{
            hexFlashing !== null && hexFlashing !== 100 ? 'Flashing' : 'Flash'
          }}
        </UButton>
      </div>
    </div>

    <div class="mt-2 flex items-center gap-2">
      <p class="app-caption pt-[1.7rem]">Progress:</p>
      <UProgress :value="Number(hexFlashing)" indicator />
    </div>
  </section>
</template>
