<script setup lang="ts">
  const colorMode = useColorMode();

  function toggleColorMode() {
    colorMode.preference = colorMode.preference === 'dark' ? 'light' : 'dark';
  }
</script>

<template>
  <ClientOnly>
    <UButton
      @click="toggleColorMode"
      :icon="
        colorMode.value === 'dark'
          ? 'i-solar-moon-outline'
          : 'i-solar-sun-outline'
      "
    />

    <template #fallback>
      <UButton @click="toggleColorMode" icon="i-solar-palette-outline" />
    </template>
  </ClientOnly>
</template>
